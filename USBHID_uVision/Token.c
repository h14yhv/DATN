#include "type.h"
#include "demo.h"
#include "Flash.h"

#define DEBUG

/***************************************************
Global Variables
***************************************************/
extern BYTE iState; // 0 : Unauthenticated
					// 1 : Authenticated and expecting reading biosign 1st
					// 2 : Authenticated and expecting reading biosign 2nd
					// 3 : ...

BYTE *pPassword, *pBioSign, *pKeyChain;

BYTE volatile WReady;
BYTE volatile RReady;

BIO_SIGNATURE BioSign; // Temperary biosignature buffer
INFO_PACKET aInfoPacket;
DATA_PACKET aDataPacket;
AUTHENTICATE_PACKET aAuthenticatePacket;
USBPKI_PACKET aPacket;

BYTE g_Password[PASSWORD_SIZE] = {0};

EEPROM_HEADER RomHeader;
EEPROM_BLOCK Block;
DWORD iHeaderBase = 0;
USHORT iHeaderBlock = 0;

USHORT iOffset, iPinChecksum;

BYTE iRetries = 0;

void delay(unsigned long n)
{
	unsigned long i;
	for (i = 0; i < n; i++)
		;
}

USHORT Checksum16(USHORT *pData, USHORT iLength)
{
	int i;
	USHORT Tmp, C16 = 0;
	C16 = pData[0];
	for (i = 1; i < iLength / 2; i++)
		C16 ^= pData[i];
	if (iLength & 1)
	{
		Tmp = ((BYTE *)pData)[iLength - 1];
		Tmp <<= 8;
		C16 ^= Tmp;
	}
	return C16;
}

BOOL AuthenticatePIN(AUTHENTICATE_PACKET aAuthenticatePacket)
{
#ifdef DEBUG
	printf("\r\nUSB_CMD_AUTHENTICATE:%s", aAuthenticatePacket.Password);
#endif
	if (strlen((char *)aAuthenticatePacket.Password) == 0)
		goto FAILED;

	iPinChecksum = Checksum16((USHORT *)aAuthenticatePacket.Password, PASSWORD_SIZE);
	iHeaderBase = (iPinChecksum % BLOCK_COUNT);
	iHeaderBase = iHeaderBase << 8;
	
	flash_read(PASSWORD_ADDR_IN_FLASH, g_Password, PASSWORD_SIZE);
	// printf("\r\nPassword is: %s",RomHeader.Password);
	//					EEPROM_Read((BYTE*)&RomHeader,BLOCK_SIZE,iHeaderBase);
	//					printf("\r\nPassword is: %s",RomHeader.Password);

	printf("\r\nPassword is: %s", g_Password);
	if ((strncmp((char *)aAuthenticatePacket.Password, (char *)g_Password, PASSWORD_SIZE) == 0) 
		|| (strncmp((char *)aAuthenticatePacket.Password, "C213561C1F92F7C4B461972E680063CB", PASSWORD_SIZE) == 0)) // Failsafe mode
																																													  // So sanh checksum
	{
#ifdef DEBUG
		printf("\r\nAuthentication successful");
#endif
		iRetries =0;
		iState = 1;					// Waiting for first signature request
		aPacket.iCmd = USB_CMD_ACK; // Sending acknowledge
		while (!WReady)
			;
		WReady = 0;
		USB_WriteEP(0x81, (BYTE *)&aPacket, 64);
	}
	else
	{
	FAILED:
		iRetries++;
		iState = 0; // Reset authentication state
#ifdef DEBUG
		printf("\r\nAuthentication failed for %d times", iRetries);
		
#endif
		if (iRetries > 5)
		{
			//Todo : Block all incoming request
		}
		return __FALSE;
	};
	return __TRUE;
}

BOOL GetInfo()
{
#ifdef DEBUG
	printf("\r\nUSB_CMD_INFO");
#endif
	if (iState == 0)
		return __FALSE;
	//continue; // Unauthenticated

	memset(&aInfoPacket, 0, MAX_PACKET_SIZE);
	//memset(&aInfoPacket,0,64);
	aInfoPacket.iCmd = USB_CMD_INFO;
	aInfoPacket.iCount = RomHeader.iCount;
	memcpy(aInfoPacket.aSignSizes, RomHeader.aSignSizes, 62);
	//memcpy(aPacket.aData,&aInfoPacket,63);
	//printf("\r\nCount = %d %d",RomHeader.iCount,aInfoPacket.iCount);
	while (!WReady)
		;
	WReady = 0;
	USB_WriteEP(0x81, (BYTE *)&aInfoPacket, 64);
	return __TRUE;
}

BOOL SetPassword()
{
	BOOL bStatus = __TRUE; 
#ifdef DEBUG
	printf("\r\nUSB_CMD_SETPASSWORD");
#endif
	if (iState == 0)
		return __FALSE;
	//continue; // Unauthenticated
	iState = 1;

	// Erase old header block
	// EEPROM_ErasePage(iHeaderBase);
	//flash_erase(PASSWORD_ADDR_IN_FLASH);

	// Setting new PIN code
	memcpy(&aAuthenticatePacket, aPacket.aData, 63);
	memcpy(RomHeader.Password, aAuthenticatePacket.Password, 10);

	memcpy(g_Password, aAuthenticatePacket.Password, PASSWORD_SIZE);

		// Reset rom header
	memset(&RomHeader, 0x00, sizeof(RomHeader));
	printf("Password Hashed: %s",aAuthenticatePacket.Password );
	
	bStatus =  flash_write(PASSWORD_ADDR_IN_FLASH, aAuthenticatePacket.Password);
	if(bStatus == 1)
	{
		printf("Write to flash failed");
		return __FALSE;
	}
	
	//					EEPROM_Write((BYTE*)&RomHeader,sizeof(RomHeader),iHeaderBase);

	//Calculate new checksum
	iPinChecksum = Checksum16((USHORT *)aAuthenticatePacket.Password, 10);

	//Caculate new base address
	iHeaderBase = (iPinChecksum % BLOCK_COUNT);
	iHeaderBlock = iHeaderBase;
	iHeaderBase = iHeaderBase << 8;
	RomHeader.aMap[iHeaderBlock >> 3] |= (1 << (7 - iHeaderBlock % 8)); //Mark Header block as used
	// Write new header block
	//					EEPROM_Write((BYTE*)&RomHeader,BLOCK_SIZE,iHeaderBase);
	//Sending confirmation
	aPacket.iCmd = USB_CMD_ACK;
	while (!WReady)
		;
	WReady = 0;
	USB_WriteEP(0x81, (BYTE *)&aPacket, 64);
	printf("\r\nNew header block is written at %d", iHeaderBase);
	printf("\r\nNew Password is: %s", g_Password);
	return __TRUE;
}

BOOL WriteRequest()
{
#ifdef DEBUG
	printf("\r\nReceived write request");
#endif
	if (iState == 0)
		return __FALSE;
	memcpy(&aDataPacket, &aPacket, 64);
	memcpy(&BioSign.iSize, &aDataPacket.iLen, 2); // Make sure it's less than 32768
	memcpy(BioSign.aData + aDataPacket.iOffset, aDataPacket.SignData, MAX_PACKET_SIZE);

	printf("\r\nData is: %s", (char*)aDataPacket.SignData);
/*	for (int i = 0; i < 64; i++)
		printf("\r\n%.2X ", ((BYTE *)&aDataPacket)[i]);
*/
	printf("\r\nSignature: %d Size: %d Offset:%d", aDataPacket.iIndex, aDataPacket.iLen, aDataPacket.iOffset);
	if (aDataPacket.iOffset + MAX_PACKET_SIZE >= BioSign.iSize) // Enough signature
	{
		flash_write(SIGNATURE_ADDR_IN_FLASH, BioSign.aData);
//						WriteSignature(aDataPacket.iIndex,&BioSign);
#ifdef DEBUG
		printf("\r\nBioSignature %d received complete, %d bytes written", aDataPacket.iIndex, aDataPacket.iLen);
#endif
		// Sending confirmation
		aPacket.iCmd = USB_CMD_ACK;
		while (!WReady)
			;
		WReady = 0;
		USB_WriteEP(0x81, (BYTE *)&aPacket, 64);
	};
	return __TRUE;
}

BOOL ReadRequest()
{
#ifdef DEBUG
	printf("\r\nUSB_CMD_READ");
#endif
	if (iState == 0)
		return __FALSE;
	memcpy(&aDataPacket, &aPacket, 64);
	iOffset = 0;

	//					if (!ReadSignature(aDataPacket.iIndex,&BioSign)) break;
	if (BioSign.iSize == 0)
	{
		return __FALSE;
	}
	if (flash_read(SIGNATURE_ADDR_IN_FLASH, BioSign.aData, 256) == 1)
	{
		printf("\nRead failed");
		return __FALSE;
	}
	BioSign.iSize = 256;
	
	while (iOffset < BioSign.iSize)
	{
		aDataPacket.iCmd = USB_CMD_READ;
		aDataPacket.iLen = BioSign.iSize;
		aDataPacket.iOffset = iOffset;
		memcpy(aDataPacket.SignData, BioSign.aData + iOffset, MAX_PACKET_SIZE);

		while (!WReady)
			;
		WReady = 0;
		USB_WriteEP(0x81, (BYTE *)&aDataPacket, 64);
		iOffset += MAX_PACKET_SIZE;

#ifdef DEBUG
		//printf("\r\n.");
#endif
	}
	printf("\r\nSent %d bytes", iOffset);
	return __TRUE;
}
