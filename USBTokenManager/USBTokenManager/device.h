#pragma once
#ifndef __DEVICE_H

//
// Define below GUIDs
//
#include <initguid.h>
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <winusb.h>
#include <usb.h>


//
// Device Interface GUID.
// Used by all WinUsb devices that this application talks to.
// Must match "DeviceInterfaceGUIDs" registry value specified in the INF file.
// 6d22450a-fe34-46d2-b019-65dba1ae0aaa
//
DEFINE_GUID(GUID_DEVINTERFACE_USBHIDManager,
    0x6d22450a,0xfe34, 0x46d2,0xb0,0x19,0x65,0xdb,0xa1,0xae,0x0a,0xaa);

//{88bae032 - 5a81 - 49f0 - bc3d - a4ff138216d6}
DEFINE_GUID(ClassGUID, 0x88bae032, 0x5a81, 0x49f0, 0xbc, 0x3d, 0xa4, 0xff, 0x13, 0x82, 0x16, 0xd6);

#define WAIT_TIME 500
#define CHECK_CLEAN_TIME 200

typedef struct _DEVICE_DATA {

	BOOL                    bIsHandlesOpen;
	WINUSB_INTERFACE_HANDLE hWinUSBInterfaceHandle;
	HANDLE                  hFileDeviceHandle;
	TCHAR                   DevicePath[MAX_PATH];
	UCHAR					uBulkInPipeId;
	UCHAR					uBulkOutPipeId;
	UCHAR					uInterruptPipeId;
} DEVICE_DATA, *PDEVICE_DATA;

enum USB_CMD
{
	USB_CMD_READ = 0x01,
	USB_CMD_WRITE,
	USB_CMD_INFO,
	USB_CMD_AUTHENTICATE,
	USB_CMD_ADD,
	USB_CMD_SETPASSWORD,
	USB_CMD_ACK,
	USB_CMD_FAIL
};

#define MAX_PACKET_SIZE 64
#define USERNAME_SIZE 30
#define PASSWORD_SIZE 33
#define MAX_NUMBER_SIGNATURES		31
#define MAX_DATA_SIGN_SIZE		  	58
#define MAX_SIGNATURE_SIZE			0x4000

typedef struct _SIGN_MESSAGE
{
	BYTE   iCmd;
	BYTE   iIndex;  //Index of signature to process, ignore for 2,3,4 command
	USHORT iLenSign;
	USHORT iOffset;//Length and offset of data	
	BYTE   SignData[MAX_DATA_SIGN_SIZE];
} SIGN_MESSAGE , *PSIGN_MESSAGE;

typedef struct _AUTHENTICATE_PACKET   
{
	BYTE	Username[USERNAME_SIZE];
	BYTE	Password[PASSWORD_SIZE];
} AUTHENTICATE_PACKET;

typedef struct _INFO_PACKET
{
	//Number of signature stored in device
	BYTE   iCmd;
	BYTE   iCount;
	USHORT aSignSizes[MAX_NUMBER_SIGNATURES];
} INFO_PACKET;

typedef struct _USBPKI_PACKET
{
	BYTE iCmd;		//1: read a signature
					//2: write a signature
					//3: inquire number of signatures
					//4: authenticate
					//5: add new signature
					//etc	...
	BYTE aData[63];

} USBPKI_PACKET;

typedef struct _DATA_MESSAGE {
	BYTE iCmd;
	union
	{
		AUTHENTICATE_PACKET AuthenticatePacket;
		//DATA_PACKET DataPacket; 
	} Data;

} DATA_MESSAGE, *PDATA_MESSAGE;


/**********************************************
Function Prototype
**********************************************/
HRESULT OpenDevice( PDEVICE_DATA DeviceData,PBOOL FailureDeviceNotFound );
VOID CloseDevice( PDEVICE_DATA DeviceData);
BOOL GetConfigDevice();
BOOL AuthenticateDevice(PCHAR szPassword);
BOOL FlushDevice();
BOOL SetPasswordDevice(PCHAR szPassword);
BOOL WriteSignature(PBYTE pSignature, USHORT usSignSize);
BOOL ReadSignature(PBYTE pSignature, PUSHORT usSignSizeTransferred);

HRESULT
RetrieveDevicePath(
	_Out_bytecap_(BufLen) LPTSTR DevicePath,
	_In_                  ULONG  BufLen,
	_Out_opt_             PBOOL  FailureDeviceNotFound
);
BOOL ReadFromDevice(PBYTE pDataMessage, ULONG ulBufferLength, PULONG pulLenghTransferred, LPOVERLAPPED pOverlapppedSync, DWORD dwTimeOut);
BOOL WriteToDevice(PBYTE pDataMessage, ULONG ulBufferLength, PULONG pulLenghTransferred, LPOVERLAPPED pOverlapppedSync, DWORD dwTimeOut);

#endif
