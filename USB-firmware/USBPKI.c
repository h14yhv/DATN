/*----------------------------------------------------------------------------
 *      Name:    DEMO.C
 *      Purpose: USB HID Demo
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This file is part of the uVision/ARM development tools.
 *      Copyright (c) 2005-2006 Keil Software. All rights reserved.
 *      This software may only be used under the terms of a valid, current,
 *      end user licence from KEIL for a compatible version of KEIL software
 *      development tools. Nothing else gives you the right to use it.
 *---------------------------------------------------------------------------*/

#include <AT91SAM7S256.H>                    /* AT91SAM7S64 definitions */
#include "Board.h"
#include "type.h"
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "spi.h"
#include "global.h"
#include "USBPKI.h"
#include <stdio.h>
#include <string.h>
#include "crc.h"
#include "demo.h"

void init_serial (void);

AT91S_PIO * pPIO = AT91C_BASE_PIOA;         /* Global Pointer to PIO */

BYTE iState = 0 ;	// 0 : Unauthenticated
									// 1 : Authenticated and expecting reading biosign 1st
									// 2 : Authenticated and expecting reading biosign 2nd
									// 3 : ...	

BYTE * pPassword, * pBioSign, * pKeyChain;

BYTE 	volatile WReady;
BYTE 	volatile RReady;

BIO_SIGNATURE  		BioSign; 	// Temperary biosignature buffer
INFO_PACKET 		aInfoPacket;
DATA_PACKET 		aDataPacket, aDataPacketACK;
AUTHENTICATE_PACKET aAuthenticatePacket;	
USBPKI_PACKET 		aPacket;	   	

EEPROM_HEADER 		RomHeader, RomHeader2Check;
EEPROM_BLOCK  		Block;
DWORD				iHeaderBase = 0;	
USHORT		   		iHeaderBlock = 0;
USHORT 				iOffset, iPinChecksum;
BOOL				checkReadWriteBioSign;

// array hash sha224 "usbtoken"
BYTE PassDefault[28] = {0xda, 0x22,0xb7, 0xf2, 0xeb, 0xfa, 0xe4, 0xc2, 0x60, 0x6a, 0x10, 0xec, 0xa0, 0x84, 0x34, 0x44, 0xa4, 0x91, 0xba, 0x22, 0xc0, 0x83, 0x93, 0x7a, 0xc6, 0xbd, 0x26, 0x7d};
BYTE iRetries = 0;

/* Main Program */

//BYTE			BufferIn[MAX_SIGNATURE_SIZE];
//BYTE			BufferOut[MAX_SIGNATURE_SIZE];

BOOL WriteSignature(BYTE iIndex, BIO_SIGNATURE * pBio)
{
	USHORT  i,iBytesWritten = 0; 	 
	USHORT 	TmpBase,TmpChecksum,NextBase;
	BOOL	bFound = FALSE;

	if (iIndex >= 31) // tối đa 31 chữ ký, iIndex max = 30
		return FALSE;

	TmpBase = iHeaderBase >> 8;	// Calculate block base address
	if (RomHeader.aSignSizes[iIndex] == 0) // Un allocated signature
		RomHeader.iCount++;
	//printf("BLOCK: Writing %d\r\n", iIndex);
	//printf("Header:%lu", iHeaderBase>>8);
	
	// New add -  nếu RomHeader.aSignSizes[iIndex] != 0, tức ghi đè lên chữ kí cũ và không có code xử lý việc xóa chữ ký cũ
	// nếu ghi đè nhiều lần lên 1 chữ kí sẽ làm hết block, dẫn đến việc ghi chữ kí bị lỗi

	RomHeader.aSignSizes[iIndex] = pBio->iSize;
	//Searching in block bitmap to find a free block for first block
	for (i = TmpBase + 1; i < BLOCK_COUNT; i++)
		if (!(RomHeader.aMap[i>>3] & (1<<(7-i%8)))) // A free block
		{
			RomHeader.aSignAddrs[iIndex] = i;
			RomHeader.aMap[i>>3] |= (1<<(7-i%8));	// Mark as used
			TmpBase = i;
			bFound = TRUE;
			break;
		};

	if (bFound==FALSE)
	  for (i = 0; i < TmpBase; i++)
		if (!(RomHeader.aMap[i>>3]& (1<<(7-i%8)))) // A free block
		{
			RomHeader.aSignAddrs[iIndex] = i;
			RomHeader.aMap[i>>3] |= (1<<(7-i%8));	// Mark as used
			TmpBase = i;
			bFound = TRUE;
			break;
		};
	
	if (bFound == FALSE)
	{
		//printf("\r\nFATAL: EEPROM is full\r\n");
		return FALSE;
	};	
					
	iBytesWritten = 0;
	do
	{
		memcpy(Block.Data, pBio->aData+iBytesWritten, PAYLOAD_SIZE);
		Block.Flags = Checksum16((USHORT*)Block.Data,PAYLOAD_SIZE/2); // Flags la du lieu thua, co the dung bat ki cai nao, miên no ngau nhien
		
		if (iBytesWritten + PAYLOAD_SIZE >= pBio->iSize)
			Block.NextBlockOffset = 0xFFFF; // Last data block in signature
		else
		{
			TmpChecksum = Checksum16((USHORT*)Block.Data,PAYLOAD_SIZE);
			//printf("Data check sum:%.4X\r\n",TmpChecksum);
			TmpChecksum ^= iPinChecksum;
			//printf("Xored checksum:%.4X\r\n",TmpChecksum);
			NextBase = TmpChecksum % BLOCK_COUNT; // Calculate next block base address
			//printf("Next valid block:%d\r\n",NextBase);
			bFound = FALSE;
			for (i = NextBase;i < BLOCK_COUNT; i++)			// Searching for a free block from NextBase
				if (!(RomHeader.aMap[i>>3]& (1<<(7-i%8)))) // A free block
				{
					bFound = TRUE;
					RomHeader.aMap[i>>3] |= (1<<(7-i%8));	// Mark next block address as used
					break;
				};

			if (bFound==FALSE)
			for (i=0;i<NextBase;i++) 						// Searching for a free block from beginning
				if (!(RomHeader.aMap[i>>3]& (1<<(7-i%8)))) // A free block
				{
					bFound = TRUE;
					RomHeader.aMap[i>>3] |= (1<<(7-i%8));	// Mark next block address as used
					break;
				};

 			if (bFound == FALSE)
			{
				//printf("\r\nFATAL: EEPROM is full\r\n");
				return FALSE;
			}
			Block.NextBlockOffset = NextBase + i;
			//printf("Real next block: %d\r\n",i);
			//printf("NextBlockOffset: %d\r\n",Block.NextBlockOffset);
		}

		EEPROM_Write((BYTE*)&Block,BLOCK_SIZE,TmpBase<<8);// Write to EEPROM, << 8 là do mỗi block có kích thước 256 byte
		//printf("=>%d",TmpBase);
		//printf("=>%d",TmpBase);
		iBytesWritten += PAYLOAD_SIZE;
		TmpBase = i; // Moving to next data block

	} while (iBytesWritten < pBio->iSize);

	EEPROM_Write((BYTE*)&RomHeader,BLOCK_SIZE,iHeaderBase); // update table rom - romeheader
	//printf(" OK\r\n");

	return TRUE;
}

BOOL ReadSignature(BYTE iIndex, BIO_SIGNATURE * pBio)
{
	USHORT iBytesRead = 0;
	USHORT TmpChecksum;
	USHORT NextBase;
	
	if (iIndex >= 31) // tối đa 31 chữ ký, iIndex max = 30
		return FALSE;

	if (RomHeader.aSignSizes[iIndex]==0)
	{
		//printf("BLOCK: Invalid block requested %d!\r\n",iIndex);
		return FALSE;
	}
	pBio->iSize = RomHeader.aSignSizes[iIndex];
	
	//Reading the first data block
	//printf("BLOCK: Reading %d, Size %d\r\n", iIndex, pBio->iSize);
	//printf("Header:%lu", iHeaderBase >> 8);
	EEPROM_Read((BYTE*)&Block,BLOCK_SIZE, RomHeader.aSignAddrs[iIndex] << 8);
	//printf("Processing block %d\r\n",RomHeader.aSignAddrs[iIndex]);
	memcpy(pBio->aData,Block.Data,PAYLOAD_SIZE);
	iBytesRead += PAYLOAD_SIZE;

	//printf("=>%d",RomHeader.aSignAddrs[iIndex]);

	while (iBytesRead < pBio->iSize)
	{
		if (Block.NextBlockOffset == 0xFFFF) // Last data block in signature
		{
			//printf("\r\nBLOCK: Data corrupted\r\n");
			return FALSE;
		};

		//printf("Data checksum: %.4X\r\n",TmpChecksum);
		//printf("Xored checksum: %.4X\r\n",iPinChecksum ^ TmpChecksum);
		//printf("Next valid block: %d\r\n",(iPinChecksum ^ TmpChecksum)%BLOCK_COUNT);
		//printf("NextBlockOffset: %d\r\n",Block.NextBlockOffset);

		TmpChecksum = Checksum16((USHORT*)Block.Data,PAYLOAD_SIZE);
		NextBase = Block.NextBlockOffset - (iPinChecksum ^ TmpChecksum)%BLOCK_COUNT ; // Calculate next block addresss
		//printf("Real next block %d\r\n",NextBase);
		EEPROM_Read((BYTE*)&Block,BLOCK_SIZE, NextBase << 8);
		memcpy(pBio->aData+iBytesRead,Block.Data,PAYLOAD_SIZE);
		iBytesRead += PAYLOAD_SIZE;
		//printf("Processing block %d\r\n",NextBase);
		//printf("=>%d",NextBase);
	};

	//printf(" OK\r\n");
	
	return TRUE; // New add
} 

void ResetEEPROM()
{
	//printf("Setting default password to TOKEN-KTMT\r\n");
	// delete old header block
	memset(&RomHeader, 0, sizeof(RomHeader));
	EEPROM_Write((BYTE*)&RomHeader, sizeof(RomHeader), iHeaderBase); // New add
	
	
	//strcpy((char*)RomHeader.Password,"TOKEN-KTMT"); New cmt
	memcpy(RomHeader.Password, "TOKEN-KTMT", 10);
	iPinChecksum = Checksum16((USHORT*)"TOKEN-KTMT",10);
	iHeaderBase  = iPinChecksum%BLOCK_COUNT;
	iHeaderBlock = iHeaderBase;
	iHeaderBase  = iHeaderBase << 8;
	RomHeader.aMap[iHeaderBlock >> 3] |= 1<<(7-iHeaderBlock%8);

	EEPROM_Write((BYTE*)&RomHeader,BLOCK_SIZE,iHeaderBase);

	memset(&RomHeader, 0, sizeof(RomHeader));
	EEPROM_Read((BYTE*)&RomHeader,256,iHeaderBase);
	//printf("Verifing default password: %s\r\n",RomHeader.Password);
}

void DeleteAllEEPROM()
{
	EEPROM_BLOCK Block;
	int i = 0;
	memset(&Block, 0, BLOCK_SIZE);
	for(i = 0; i < BLOCK_COUNT; i++)
	{
		EEPROM_Write((BYTE*)&Block,BLOCK_SIZE,i<<8);
	}
}

int main (void) {
	
	/* Enable Clock for PIO */
	AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA)|(1 << AT91C_ID_US1);

	pPIO->PIO_PER  = LED_MASK;                /* Enable PIO for LED1..4  */
	pPIO->PIO_OER  = LED_MASK;                /* LED1..4 are Outputs     */
	pPIO->PIO_SODR = LED_MASK;                /* Turn off LED's ("1")    */

	init_serial();	

	memset(&RomHeader,0,sizeof(RomHeader));

	WReady = 0; // khoi tao host chua yeu cau doc data - New add
	//WReady = 1; // New cmt
	RReady = 0; // Nothing to read;

	USB_Init();                               // USB Initialization 
	USB_Reset();
	USB_Connect(TRUE);                        // USB Connect 

	InitSPI();
	crcInit();

	iState = 0; // Unauthenticated  
	while (1)
	{
		BOOL bStatus = TRUE;
		if (RReady) // Checking whether there is new data
		{
			RReady = 0;
			switch (aPacket.iCmd)
			{
			case USB_CMD_AUTHENTICATE:
				memset(&aAuthenticatePacket, 0, sizeof(AUTHENTICATE_PACKET));
				memcpy(&aAuthenticatePacket, aPacket.aData, 63);
				bStatus = AuthenticatePIN(aAuthenticatePacket);
				if (bStatus == FALSE)
				{
					aPacket.iCmd = USB_CMD_FAIL; // Sending acknowledge
					while (!WReady)
						;
					WReady = 0;
					USB_WriteEP(0x81, (BYTE *)&aPacket, 64);
					continue;
				}
				break;
			case USB_CMD_INFO:
				bStatus = GetInfo();
				if (bStatus == FALSE)
				{
					aPacket.iCmd = USB_CMD_FAIL; // Sending acknowledge
					while (!WReady)
						;
					WReady = 0;
					USB_WriteEP(0x81, (BYTE *)&aPacket, 64);
					continue;
				}
				break;

			case USB_CMD_SETPASSWORD:
				bStatus = SetPassword();
				if (bStatus == FALSE)
				{
					aPacket.iCmd = USB_CMD_FAIL; // Sending acknowledge
					while (!WReady)
						;
					WReady = 0;
					USB_WriteEP(0x81, (BYTE *)&aPacket, 64);
					continue;
				}
				break;
			case USB_CMD_WRITE:
				bStatus = WriteRequest();
				if (bStatus == FALSE)
				{
					aPacket.iCmd = USB_CMD_FAIL; // Sending acknowledge
					while (!WReady)
						;
					WReady = 0;
					USB_WriteEP(0x81, (BYTE *)&aPacket, 64);
					continue;
				}
				break;
			case USB_CMD_READ:
				bStatus = ReadRequest();
				if (bStatus == FALSE)
				{
					aPacket.iCmd = USB_CMD_FAIL; // Sending acknowledge
					while (!WReady)
						;
					WReady = 0;
					USB_WriteEP(0x81, (BYTE *)&aPacket, 64);
					continue;
				}
				break;

			default:
				printf("\r\nUnrecognized command");
			}
		}
	}
}
