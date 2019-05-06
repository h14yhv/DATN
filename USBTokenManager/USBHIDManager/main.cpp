#include "pch.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include "Common.h"

DEVICE_DATA g_DeviceData = {0};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved
)
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(lpReserved);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


LONG __cdecl _tmain(
    LONG     Argc,
    LPTSTR * Argv
    )
{
    HRESULT               hr;
    USB_DEVICE_DESCRIPTOR deviceDesc;
    BOOL                  bResult;
    BOOL                  noDevice;
    ULONG                 lengthReceived;

//	ULONG ulBytesTransferred = 0;
	CHAR szBuffer[MAX_PATH] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbcccccccccccccccccccccccccdddddddddddddddddddddddddddddddddddeeeeeeeeeeeeeeeeeeeeeeeeeeee"};
	OVERLAPPED OverlapppedSync = {0};
	OverlapppedSync.hEvent = CreateEventW(NULL, FALSE, FALSE, L"AuthenticateEvent");
	if (OverlapppedSync.hEvent == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		return FALSE;
	}

	DebugPrint("Size of CMD: %d", sizeof(SIGN_MESSAGE));

    UNREFERENCED_PARAMETER(Argc);
    UNREFERENCED_PARAMETER(Argv);

    hr = OpenDevice(&g_DeviceData, &noDevice);

    if (FAILED(hr)) {

        if (noDevice) {

            printf(_T("Device not connected or driver not installed\n"));

        } else {

            printf(_T("Failed looking for device, HRESULT 0x%x\n"), hr);
        }
		RET_THIS;
    }

    bResult = WinUsb_GetDescriptor(g_DeviceData.hWinUSBInterfaceHandle,
                                   USB_DEVICE_DESCRIPTOR_TYPE,
                                   0,
                                   0,
                                   (PBYTE) &deviceDesc,
                                   sizeof(deviceDesc),
                                   &lengthReceived);

    if (FALSE == bResult || lengthReceived != sizeof(deviceDesc)) {

        printf(_T("Error among LastError %d or lengthReceived %d\n"),
               FALSE == bResult ? GetLastError() : 0,
               lengthReceived);
        CloseDevice(&g_DeviceData);
		RET_THIS;
    }

    DebugPrint("Device found: VID_%04X&PID_%04X; bcdUsb %04X\n",
           deviceDesc.idVendor,
           deviceDesc.idProduct,
           deviceDesc.bcdUSB);

	GetConfigDevice();

/*	strcpy_s(szBuffer, MAX_PATH, "ETOKENV200");*/

	bResult = AuthenticateDevice("ETOKENV200");
	if (bResult == FALSE)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS;
	}

// 	bResult = SetPasswordDevice("huyhv8");
// 	if (bResult == FALSE)
// 	{
// 		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
// 		RET_THIS;
// 	}


	bResult = WriteSignature((PBYTE)szBuffer, MAX_PATH);
	if (bResult == FALSE)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS;
	}
	
	CHAR szBufferRead[500] = { 0 };
	USHORT usBufferRead = 0;
	bResult = ReadSignature((PBYTE)szBufferRead, &usBufferRead);
	if (bResult == FALSE || usBufferRead == 0)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS;
	}

	CloseDevice(&g_DeviceData);
RET_LABEL:

	CLOSE_HANDLE(OverlapppedSync.hEvent);
	system("pause");
    return 0;
}
