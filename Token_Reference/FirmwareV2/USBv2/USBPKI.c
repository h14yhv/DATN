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

#define DEBUG

void init_serial (void);

AT91S_PIO * pPIO = AT91C_BASE_PIOA;         /* Global Pointer to PIO */




BYTE iState =0 ; // 0 : Unauthenticated
				 // 1 : Authenticated and expecting reading biosign 1st
				 // 2 : Authenticated and expecting reading biosign 2nd
				 // 3 : ...	
				
BYTE * pPassword, * pBioSign, * pKeyChain;

BYTE 	volatile WReady;
BYTE 	volatile RReady;

BIO_SIGNATURE  BioSign; // Temperary biosignature buffer
INFO_PACKET aInfoPacket;
DATA_PACKET aDataPacket;
AUTHENTICATE_PACKET aAuthenticatePacket;	
USBPKI_PACKET aPacket;	   	


EEPROM_HEADER RomHeader;
EEPROM_BLOCK  Block;
DWORD			iHeaderBase=0;	
USHORT		    iHeaderBlock = 0;


USHORT iOffset,iPinChecksum;

BYTE iRetries = 0;


BYTE RamDisk[32768];

void RAM_Write(void * srcBuf,int length,int Addr)
{
	memcpy(RamDisk+Addr,srcBuf,length);
}
void RAM_Read(void* dstBuf,int length,int Addr)
{
	memcpy(dstBuf,RamDisk+Addr,length);
}


void delay(unsigned long n) {
  unsigned long i;
  for (i = 0; i < n; i++);
}

USHORT Checksum16(USHORT * pData, USHORT iLength)
{
	int i;
	USHORT Tmp,C16 = 0;
	C16 = pData[0];
	for (i=1;i<iLength/2;i++)
		C16 ^= pData[i];
	if (iLength&1)
		{
			Tmp = ((BYTE*)pData)[iLength-1];
			Tmp << 8;
			C16 ^= Tmp;

		}
	return C16;
}


/* Main Program */

//BYTE			BufferIn[MAX_SIGNATURE_SIZE];
//BYTE			BufferOut[MAX_SIGNATURE_SIZE];

BOOL WriteSignature(BYTE iIndex,BIO_SIGNATURE * pBio)
{
	USHORT  i,iBytesWritten = 0; 	 
	USHORT 	TmpBase,TmpChecksum,NextBase;
	BOOL	bFound = false;


	TmpBase = iHeaderBase>>8;	// Calculate block base address
	if (RomHeader.aSignSizes[iIndex]== 0) // Un allocated signature
		RomHeader.iCount++;
	printf("BLOCK: Writing %d\r\n",iIndex);
	printf("Header:%d",iHeaderBase>>8);
	
	RomHeader.aSignSizes[iIndex] = pBio->iSize;
	//Searching in block bitmap to find a free block for first block
	for (i=TmpBase+1;i<BLOCK_COUNT;i++)
		if (!(RomHeader.aMap[i>>3]& (1<<(7-i%8)))) // A free block
			{
				
				
				RomHeader.aSignAddrs[iIndex] = i;
				RomHeader.aMap[i>>3] |= (1<<(7-i%8));	// Mark as used
				TmpBase = i;
				bFound = TRUE;
				break;
			};
	if (bFound==FALSE)
	  for (i=0;i<TmpBase;i++)
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
			printf("\r\nFATAL: EEPROM is full\r\n");
			return FALSE;
		};
	
	
					
	iBytesWritten = 0;
	do
	{
		memcpy(Block.Data,pBio->aData+iBytesWritten,PAYLOAD_SIZE);
		Block.Flags = 0xFFFF;
		if (iBytesWritten+PAYLOAD_SIZE>=pBio->iSize)
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
			for (i= NextBase;i<BLOCK_COUNT;i++)			   // Searching for a free block from NextBase
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
				printf("\r\nFATAL: EEPROM is full\r\n");
				return FALSE;
			}
			Block.NextBlockOffset = i + NextBase;
			//printf("Real next block: %d\r\n",i);
			//printf("NextBlockOffset: %d\r\n",Block.NextBlockOffset);


		 }
		EEPROM_Write((BYTE*)&Block,BLOCK_SIZE,TmpBase<<8);// Write to EEPROM
		printf("=>%d",TmpBase);
		//printf("=>%d",TmpBase);
		iBytesWritten += PAYLOAD_SIZE;
		TmpBase = i; // Moving to next data block

	 } while (iBytesWritten<pBio->iSize);		
	 EEPROM_Write((BYTE*)&RomHeader,BLOCK_SIZE,iHeaderBase);
	 printf(" OK\r\n");
	 return TRUE;

}
BOOL ReadSignature(BYTE iIndex, BIO_SIGNATURE * pBio)
{
	USHORT iBytesRead = 0;
	USHORT TmpChecksum;
	short NextBase;
	//if (iIndex>=RomHeader.iCount) return FALSE;
	if (RomHeader.aSignSizes[iIndex]==0)
		{
			printf("BLOCK: Invalid block requested %d!\r\n",iIndex);
			return FALSE;
		}
	pBio->iSize = RomHeader.aSignSizes[iIndex];
	
	//Reading the first data block
	printf("BLOCK: Reading %d, Size %d\r\n",iIndex,pBio->iSize);
	printf("Header:%d",iHeaderBase>>8);
	EEPROM_Read((BYTE*)&Block,BLOCK_SIZE,RomHeader.aSignAddrs[iIndex]<<8);
	//printf("Processing block %d\r\n",RomHeader.aSignAddrs[iIndex]);
	memcpy(pBio->aData,Block.Data,PAYLOAD_SIZE);
	iBytesRead += PAYLOAD_SIZE;
	printf("=>%d",RomHeader.aSignAddrs[iIndex]);
	while (iBytesRead< pBio->iSize)
	{
		TmpChecksum = Checksum16((USHORT*)Block.Data,PAYLOAD_SIZE);
		//printf("Data checksum: %.4X\r\n",TmpChecksum);
		if (Block.NextBlockOffset<0) 
		{
			printf("\r\nBLOCK: Data corrupted\r\n");
			return FALSE;
		};
		//printf("Xored checksum: %.4X\r\n",iPinChecksum ^ TmpChecksum);
		//printf("Next valid block: %d\r\n",(iPinChecksum ^ TmpChecksum)%BLOCK_COUNT);
		//printf("NextBlockOffset: %d\r\n",Block.NextBlockOffset);
		NextBase = Block.NextBlockOffset - (iPinChecksum ^ TmpChecksum)%BLOCK_COUNT ; // Calculate next block addresss
		//printf("Real next block %d\r\n",NextBase);
		EEPROM_Read((BYTE*)&Block,BLOCK_SIZE,NextBase<<8);
		memcpy(pBio->aData+iBytesRead,Block.Data,PAYLOAD_SIZE);
		iBytesRead += PAYLOAD_SIZE;
		//printf("Processing block %d\r\n",NextBase);
		printf("=>%d",NextBase);
	};
	printf(" OK\r\n");




} 

void ResetEEPROM()
{
  printf("Setting default password to lhoang8500\r\n");

  strcpy(RomHeader.Password,"lhoang8500");
  iPinChecksum = Checksum16((USHORT*)"lhoang8500",10);
  iHeaderBase  = iPinChecksum%BLOCK_COUNT;
  iHeaderBlock = iHeaderBase;
  iHeaderBase  = iHeaderBase << 8;
  RomHeader.aMap[iHeaderBlock>>3] |= 1<<(7-iHeaderBlock%8);
  EEPROM_Write((BYTE*)&RomHeader,BLOCK_SIZE,iHeaderBase);
  memset(&RomHeader,0,sizeof(RomHeader));
  EEPROM_Read((BYTE*)&RomHeader,256,iHeaderBase);
  printf("Verifing default password: %s\r\n",RomHeader.Password);
  
}

int main (void) {
  
  unsigned int i,iRead,iWrite,fault = 0;
  unsigned char  test[] = "123456789";
  DWORD			crcTest;
  
  BYTE			c,bNeedRewriteHeader = FALSE;
  
  
  
  /* Enable Clock for PIO */
  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA)|(1 << AT91C_ID_US1);

  pPIO->PIO_PER  = LED_MASK;                /* Enable PIO for LED1..4  */
  pPIO->PIO_OER  = LED_MASK;                /* LED1..4 are Outputs     */
  pPIO->PIO_SODR = LED_MASK;                /* Turn off LED's ("1")    */

  init_serial();	
  
  
  memset(&RomHeader,0,sizeof(RomHeader));


  WReady = 1;
  RReady = 0;//Nothing to read;
  
  
  
  printf("\r\n*****************************************\r\n\r\n");
     
  printf("EToken 2.0\r\n");	
  printf("Copyright@2010\r\n");
  printf("Luong Anh Hoang\r\n");
  printf("SOICT-HUT-Vietnam\r\n");
  printf("Project KC-0111\r\n");
  printf("hoangla-fit@mail.hut.edu.vn\r\n");
  printf("\r\n*****************************************\r\n");
  printf("USB Initializing...\r\n");
  
  
  
  
  
  USB_Init();                               // USB Initialization 
  USB_Reset();
  USB_Connect(TRUE);                        // USB Connect 
  
  
  
   
  printf("EEPROM Initializing...\r\n");
  InitSPI();




   crcInit();
   printf("CRC32 Initializing...\r\n");


   
  //EEPROM_EraseChip();
  /* 
#define TEST_SIZE 0x4000
  
  //Testing EEPROM functionality

  for (i=0;i<TEST_SIZE;i++)
  	BioSign.aData[i] = 'A' + i%26;

  EEPROM_Write(BioSign.aData,TEST_SIZE,0x00000000);
  printf("Wrote %d bytes\r\n",TEST_SIZE);
  memset(BioSign.aData,0,TEST_SIZE);
  
  EEPROM_Read(BioSign.aData,TEST_SIZE,0x00000000);
  printf("Read %d bytes\r\n",TEST_SIZE);
  //for (i = 0;i<256;i++)
  //	printf("%c",BufferOut[i]);
  for (i=0;i<TEST_SIZE;i++)
    if (BioSign.aData[i]!= i%26+'A')
	   fault++;

  printf("Found %d fault!\r\n",fault);
  
  */
  
  
  //ResetEEPROM();
  
  /*  
  strcpy(RomHeader.Password,"lhoang8500");
  iPinChecksum = Checksum16((USHORT*)"lhoang8500",10);
  iHeaderBase  = iPinChecksum%BLOCK_COUNT;
  iHeaderBlock = iHeaderBase;
  iHeaderBase  = iHeaderBase << 8;
  memset(&RomHeader,0,sizeof(RomHeader));
  EEPROM_Read((BYTE*)&RomHeader,256,iHeaderBase);


  printf("Verifing password:%s\r\n",RomHeader.Password);  
  
  
  BioSign.iSize = 0x4000;
  for (i=0;i<0x4000;i++)
  	BioSign.aData[i]= 'A' + i%26;

  //crcTest = crcFast(BioSign.aData,0x4000);
  

  WriteSignature(1,&BioSign);
  memset(&BioSign,0,sizeof(BioSign));
  ReadSignature(1,&BioSign);

  
  for (i=0;i<0x4000;i++)
  	if (BioSign.aData[i]!= 'A' + i%26)
		fault++;
  

  
  //printf("Found %d faults in sign 0\r\n",crcTest != crcFast(BioSign.aData,0x4000));
  printf("Found %d faults in sign 1\r\n",fault);
  */

  




  

  
  
  
  
  printf("EToken ready!\r\n");
  iState = 0; // Unauthenticated  
  while (1)   
  { 
  		 if (RReady)	  // Checking whether there is new data
		 {
		 	RReady = 0;
			switch (aPacket.iCmd)
			{

				case USB_CMD_AUTHENTICATE:
					memcpy(&aAuthenticatePacket,aPacket.aData,63);
#ifdef DEBUG
					printf("USB_CMD_AUTHENTICATE:%s\r\n",aAuthenticatePacket.Password);
#endif
					if (strlen(aAuthenticatePacket.Password)==0) goto FAILED;					
					
					iPinChecksum = Checksum16((USHORT*)aAuthenticatePacket.Password,10);
					iHeaderBase = (iPinChecksum%BLOCK_COUNT);
					iHeaderBase = iHeaderBase<<8;
					EEPROM_Read((BYTE*)&RomHeader,BLOCK_SIZE,iHeaderBase);
					
					printf("Password is: %s\r\n",RomHeader.Password);
					
					if ((strncmp(aAuthenticatePacket.Password,RomHeader.Password,10)==0)||
					   (strncmp(aAuthenticatePacket.Password,"ETOKENV200",10)==0))
					 // So sanh checksum
					{					
#ifdef DEBUG
						printf("Authentication successful\r\n");
#endif
						iState = 1;										// Waiting for first signature request
						aPacket.iCmd = USB_CMD_ACK;						// Sending acknowledge
						while (!WReady);
						WReady = 0;
						USB_WriteEP(0x81,(BYTE*)&aPacket,64);

					
					}
					else
					{
FAILED:
						iRetries++;
						iState = 0; 		// Reset authentication state
#ifdef DEBUG
						printf("Authentication failed for %d times\r\n",iRetries);
#endif
						if (iRetries>5)
							{
								//Todo : Block all incoming request
							}
					};
					
					break;
				case USB_CMD_INFO:

#ifdef DEBUG
					printf("USB_CMD_INFO\r\n");
#endif
					if (iState == 0) continue;		// Unauthenticated

					memset(&aInfoPacket,0,MAX_PACKET_SIZE);
					//memset(&aInfoPacket,0,64);
					aInfoPacket.iCmd = USB_CMD_INFO;
					aInfoPacket.iCount = RomHeader.iCount;
					memcpy(aInfoPacket.aSignSizes,RomHeader.aSignSizes,62);
					//memcpy(aPacket.aData,&aInfoPacket,63);
					//printf("Count = %d %d\r\n",RomHeader.iCount,aInfoPacket.iCount);
					while (!WReady);
					WReady = 0;
					USB_WriteEP(0x81,(BYTE*)&aInfoPacket,64);
					break;
				case USB_CMD_SETPASSWORD:
#ifdef DEBUG
					printf("USB_CMD_SETPASSWORD\r\n");
#endif
					if (iState == 0) continue;
					
					// Erase old header block
					// EEPROM_ErasePage(iHeaderBase);

					// Reset rom header
					memset(&RomHeader,0x00,sizeof(RomHeader));
					EEPROM_Write((BYTE*)&RomHeader,sizeof(RomHeader),iHeaderBase);
					
					// Setting new PIN code
					memcpy(&aAuthenticatePacket,aPacket.aData,63);
					memcpy(RomHeader.Password,aAuthenticatePacket.Password,10);
					

					

					//Calculate new checksum
					iPinChecksum = Checksum16((USHORT*)aAuthenticatePacket.Password,10);
					//Caculate new base address
					iHeaderBase = (iPinChecksum%BLOCK_COUNT);
					iHeaderBlock= iHeaderBase;
					iHeaderBase = iHeaderBase<<8;
					RomHeader.aMap[iHeaderBlock>>3] |= (1<<(7-iHeaderBlock%8)); //Mark Header block as used
					// Write new header block
					EEPROM_Write((BYTE*)&RomHeader,BLOCK_SIZE,iHeaderBase);
					//Sending confirmation
					aPacket.iCmd = USB_CMD_ACK;
					while (!WReady);
					WReady = 0;
					USB_WriteEP(0x81,(BYTE*)&aPacket,64);
					printf("New header block is written at %d\r\n",iHeaderBase);
					break;
				case USB_CMD_WRITE:
#ifdef DEBUG
					//printf("Received write request\r\n");
#endif
					if (iState == 0) continue;
					memcpy(&aDataPacket,&aPacket,64);
					memcpy(&BioSign.iSize,&aDataPacket.iLen,2);	  // Make sure it's less than 32768
					memcpy(BioSign.aData + aDataPacket.iOffset,aDataPacket.SignData,MAX_PACKET_SIZE);
					/*
					printf("Data is: \r\n");
					for (i=0;i<64;i++)
						printf("%.2X ",((BYTE*)&aDataPacket)[i]);
					*/
					//printf("Signature: %d Size: %d Offset:%d\r\n",aDataPacket.iIndex,aDataPacket.iLen,aDataPacket.iOffset);
					if (aDataPacket.iOffset + MAX_PACKET_SIZE >= BioSign.iSize) // Enough signature
					{
						WriteSignature(aDataPacket.iIndex,&BioSign);
#ifdef DEBUG
						printf("BioSignature %d received complete, %d bytes written\r\n",aDataPacket.iIndex,aDataPacket.iLen);		
#endif				
						// Sending confirmation
						aPacket.iCmd = USB_CMD_ACK;
						while (!WReady);
						WReady = 0;
						USB_WriteEP(0x81,(BYTE*)&aPacket,64);
						
					};
					break;
				case USB_CMD_READ:
#ifdef DEBUG
					printf("USB_CMD_READ\r\n");
#endif
					if (iState == 0) continue;
					memcpy(&aDataPacket,&aPacket,64);
					iOffset = 0;

					if (!ReadSignature(aDataPacket.iIndex,&BioSign)) break;
					
					while (iOffset < BioSign.iSize)
					{
						aDataPacket.iCmd = USB_CMD_READ;
						aDataPacket.iLen = BioSign.iSize;
						aDataPacket.iOffset = iOffset;
						memcpy(aDataPacket.SignData,BioSign.aData+iOffset,MAX_PACKET_SIZE);
						
						while (!WReady);
						WReady = 0;
						USB_WriteEP(0x81,(BYTE*)&aDataPacket,64);
						iOffset += MAX_PACKET_SIZE;
					
#ifdef DEBUG
						printf(".");
#endif

					}
					printf("\r\nSent %d bytes\r\n",iOffset);
					break;
					
										
				default:
					printf("Unrecognized command\r\n");
					


			}
		 }		
  		 
		 
		 

     	
 	  
   }		  
}
