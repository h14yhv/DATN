/*----------------------------------------------------------------------------
 * Name:    demo.c
 * Purpose: USB HID Demo
 * Version: V1.20
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC23xx.H> /* LPC23xx/LPC24xx definitions */

#include "demo.h"
#include "LCD.h"
#include "Flash.h"

#define DEBUG

extern void init_serial(void);
extern int sendchar(int ch);

U8 InReport; /* HID Input Report    */
/*   Bit0   : Buttons  */
/*   Bit1..7: Reserved */

U8 OutReport; /* HID Out Report      */
			  /*   Bit0..7: LEDs     */

/***************************************************
Global Variables
***************************************************/

BYTE iState = 0; // 0 : Unauthenticated
				 // 1 : Authenticated and expecting reading biosign 1st
				 // 2 : Authenticated and expecting reading biosign 2nd
				 // 3 : ...

extern BYTE *pPassword, *pBioSign, *pKeyChain;

extern BYTE volatile WReady;
extern BYTE volatile RReady;

extern BIO_SIGNATURE BioSign; // Temperary biosignature buffer
extern INFO_PACKET aInfoPacket;
extern DATA_PACKET aDataPacket;
extern AUTHENTICATE_PACKET aAuthenticatePacket;
extern USBPKI_PACKET aPacket;

extern BYTE g_Password[PASSWORD_SIZE];

extern EEPROM_HEADER RomHeader;
extern EEPROM_BLOCK Block;
extern DWORD iHeaderBase;
extern USHORT iHeaderBlock;

extern USHORT iOffset, iPinChecksum;

extern BYTE iRetries;

/*------------------------------------------------------------------------------
  Get HID Input Report -> InReport
 *------------------------------------------------------------------------------*/
void GetInReport(void)
{

	if ((FIO2PIN & PB_INT0) == 0)
	{ /* Check if PBINT is pressed */
		InReport = 0x01;
	}
	else
	{
		InReport = 0x00;
	}
}

/*------------------------------------------------------------------------------
  Set HID Output Report <- OutReport
 *------------------------------------------------------------------------------*/
void SetOutReport(void)
{

	FIO2CLR = LED_MSK;
	FIO2SET = OutReport;
}

/* Main Program */
int main(void)
{
	char szPath[256] = "Check flash firmware";
	char read[256] = {0};
	BOOL bStatus;

	PINSEL10 = 0;		/* Disable ETM interface */
	FIO2DIR |= LED_MSK; /* LEDs, port 2, bit 0~7 output only */
	init_serial();

	lcd_init();
	lcd_clear();
	lcd_print("MCB2300 HID Demo");
	set_cursor(0, 1);
	lcd_print("    huyhv8  ");

	printf("\r\n*****************************************\r\n\r\n");
	printf("Hoang Viet Huy\r\n");
	printf("SOICT-HUT-Vietnam\r\n");
	printf("\r\n*****************************************\r\n");
	printf("USB Initializing...\r\n");

	USB_Init();			 /* USB Initialization */
	USB_Connect(__TRUE); /* USB Connect */

	WReady = 1;
	RReady = 0; //Nothing to read;
	printf("\r\nEToken ready!");
	iState = 0; // Unauthenticated

	// Init(0x00058000,4000000,2); //4096000

	//Init(0x00058000,4096000,2);

	__disable_irq();

	printf("\nBuffer write: %s", szPath);
	bStatus = flash_write(0x00058000, (U8 *)szPath);
	if (bStatus == 1)
	{
		lcd_print("Write failed");
		printf("\nWrite failed");
	}

	flash_read(0x00058000, (U8 *)read, 256);
	if (bStatus == 1)
	{
		lcd_print("Read failed");
		printf("\nRead failed");
	}

	printf("\nRead: %s", read);

	__enable_irq();

	while (1)
	{
		BOOL bStatus = __TRUE;
		if (RReady) // Checking whether there is new data
		{
			RReady = 0;
			switch (aPacket.iCmd)
			{
			case USB_CMD_AUTHENTICATE:
				memset(&aAuthenticatePacket, 0, sizeof(AUTHENTICATE_PACKET));
				memcpy(&aAuthenticatePacket, aPacket.aData, 63);
				bStatus = AuthenticatePIN(aAuthenticatePacket);
				if (bStatus == __FALSE)
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
				if (bStatus == __FALSE)
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
				if (bStatus == __FALSE)
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
				if (bStatus == __FALSE)
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
				if (bStatus == __FALSE)
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
