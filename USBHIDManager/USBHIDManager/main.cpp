#include "pch.h"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include "Common.h"

DEVICE_DATA g_DeviceData = {0};



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

    UNREFERENCED_PARAMETER(Argc);
    UNREFERENCED_PARAMETER(Argv);

    //
    // Find a device connected to the system that has WinUSB installed using our
    // INF
    //
    hr = OpenDevice(&g_DeviceData, &noDevice);

    if (FAILED(hr)) {

        if (noDevice) {

            printf(_T("Device not connected or driver not installed\n"));

        } else {

            printf(_T("Failed looking for device, HRESULT 0x%x\n"), hr);
        }
		RET_THIS;
    }

    //
    // Get device descriptor
    //
    bResult = WinUsb_GetDescriptor(g_DeviceData.WinUsbHandle,
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

    //
    // Print a few parts of the device descriptor
    //
    DebugPrintW(L"Device found: VID_%04X&PID_%04X; bcdUsb %04X\n",
           deviceDesc.idVendor,
           deviceDesc.idProduct,
           deviceDesc.bcdUSB);

	GetConfigDevice();

	ReadFromDevice();
	WriteToDevice();
	
    CloseDevice(&g_DeviceData);

RET_LABEL:
	system("pause");
    return 0;
}
