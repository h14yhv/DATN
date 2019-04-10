#include "pch.h"
#include "Common.h"

#include <SetupAPI.h>
extern DEVICE_DATA g_DeviceData;

HRESULT
RetrieveDevicePath(
	_Out_bytecap_(BufLen) LPTSTR DevicePath,
	_In_                  ULONG  BufLen,
	_Out_opt_             PBOOL  FailureDeviceNotFound
);

HRESULT
OpenDevice(
	_Out_     PDEVICE_DATA DeviceData,
	_Out_opt_ PBOOL        FailureDeviceNotFound
)
/*++

Routine description:

	Open all needed handles to interact with the device.

	If the device has multiple USB interfaces, this function grants access to
	only the first interface.

	If multiple devices have the same device interface GUID, there is no
	guarantee of which one will be returned.

Arguments:

	DeviceData - Struct filled in by this function. The caller should use the
		WinusbHandle to interact with the device, and must pass the struct to
		CloseDevice when finished.

	FailureDeviceNotFound - TRUE when failure is returned due to no devices
		found with the correct device interface (device not connected, driver
		not installed, or device is disabled in Device Manager); FALSE
		otherwise.

Return value:

	HRESULT

--*/
{
	HRESULT hr = S_OK;
	BOOL    bResult;

	DeviceData->HandlesOpen = FALSE;

	hr = RetrieveDevicePath(DeviceData->DevicePath,
		sizeof(DeviceData->DevicePath),
		FailureDeviceNotFound);

	if (FAILED(hr)) {

		return hr;
	}

	DeviceData->DeviceHandle = CreateFile(DeviceData->DevicePath,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (INVALID_HANDLE_VALUE == DeviceData->DeviceHandle) {

		hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	bResult = WinUsb_Initialize(DeviceData->DeviceHandle,
		&DeviceData->WinUsbHandle);

	if (FALSE == bResult) {

		hr = HRESULT_FROM_WIN32(GetLastError());
		CloseHandle(DeviceData->DeviceHandle);
		return hr;
	}

	DeviceData->HandlesOpen = TRUE;
	return hr;
}

VOID
CloseDevice(
	_Inout_ PDEVICE_DATA DeviceData
)
/*++

Routine description:

	Perform required cleanup when the device is no longer needed.

	If OpenDevice failed, do nothing.

Arguments:

	DeviceData - Struct filled in by OpenDevice

Return value:

	None

--*/
{
	if (FALSE == DeviceData->HandlesOpen) {

		//
		// Called on an uninitialized DeviceData
		//
		return;
	}

	WinUsb_Free(DeviceData->WinUsbHandle);
	CloseHandle(DeviceData->DeviceHandle);
	DeviceData->HandlesOpen = FALSE;

	return;
}

HRESULT
RetrieveDevicePath(
	_Out_bytecap_(BufLen) LPTSTR DevicePath,
	_In_                  ULONG  BufLen,
	_Out_opt_             PBOOL  FailureDeviceNotFound
)
/*++

Routine description:

	Retrieve the device path that can be used to open the WinUSB-based device.

	If multiple devices have the same device interface GUID, there is no
	guarantee of which one will be returned.

Arguments:

	DevicePath - On successful return, the path of the device (use with CreateFile).

	BufLen - The size of DevicePath's buffer, in bytes

	FailureDeviceNotFound - TRUE when failure is returned due to no devices
		found with the correct device interface (device not connected, driver
		not installed, or device is disabled in Device Manager); FALSE
		otherwise.

Return value:

	HRESULT

--*/
{
	BOOL                             bResult = FALSE;
	HDEVINFO                         deviceInfo;
	SP_DEVICE_INTERFACE_DATA         interfaceData;
	PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = NULL;
	ULONG                            length;
	ULONG                            requiredLength = 0;
	HRESULT                          hr;

	if (NULL != FailureDeviceNotFound) {

		*FailureDeviceNotFound = FALSE;
	}

	//
	// Enumerate all devices exposing the interface
	//
	deviceInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_USBHIDManager,
		NULL,
		NULL,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (deviceInfo == INVALID_HANDLE_VALUE) {

		hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	//
	// Get the first interface (index 0) in the result set
	//
	bResult = SetupDiEnumDeviceInterfaces(deviceInfo,
		NULL,
		&GUID_DEVINTERFACE_USBHIDManager,
		0,
		&interfaceData);

	if (FALSE == bResult) {

		//
		// We would see this error if no devices were found
		//
		if (ERROR_NO_MORE_ITEMS == GetLastError() &&
			NULL != FailureDeviceNotFound) {

			*FailureDeviceNotFound = TRUE;
		}

		hr = HRESULT_FROM_WIN32(GetLastError());
		SetupDiDestroyDeviceInfoList(deviceInfo);
		return hr;
	}

	//
	// Get the size of the path string
	// We expect to get a failure with insufficient buffer
	//
	bResult = SetupDiGetDeviceInterfaceDetail(deviceInfo,
		&interfaceData,
		NULL,
		0,
		&requiredLength,
		NULL);

	if (FALSE == bResult && ERROR_INSUFFICIENT_BUFFER != GetLastError()) {

		hr = HRESULT_FROM_WIN32(GetLastError());
		SetupDiDestroyDeviceInfoList(deviceInfo);
		return hr;
	}

	//
	// Allocate temporary space for SetupDi structure
	//
	detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)
		LocalAlloc(LMEM_FIXED, requiredLength);

	if (NULL == detailData)
	{
		hr = E_OUTOFMEMORY;
		SetupDiDestroyDeviceInfoList(deviceInfo);
		return hr;
	}

	detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	length = requiredLength;

	//
	// Get the interface's path string
	//
	bResult = SetupDiGetDeviceInterfaceDetail(deviceInfo,
		&interfaceData,
		detailData,
		length,
		&requiredLength,
		NULL);

	if (FALSE == bResult)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		LocalFree(detailData);
		SetupDiDestroyDeviceInfoList(deviceInfo);
		return hr;
	}

	//
	// Give path to the caller. SetupDiGetDeviceInterfaceDetail ensured
	// DevicePath is NULL-terminated.
	//
	hr = StringCbCopy(DevicePath,
		BufLen,
		detailData->DevicePath);

	LocalFree(detailData);
	SetupDiDestroyDeviceInfoList(deviceInfo);

	return hr;
}

BOOL GetConfigDevice()
{
	BOOL bResult;
	USB_INTERFACE_DESCRIPTOR ifaceDescriptor = {0};
	WINUSB_PIPE_INFORMATION pipeInfo;
	UCHAR speed;
	ULONG length;

	length = sizeof(UCHAR);
	bResult = WinUsb_QueryDeviceInformation(g_DeviceData.WinUsbHandle,
		DEVICE_SPEED,
		&length,
		&speed);
	if (bResult == FALSE)
	{
		PrintError(L"Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bResult, FALSE);
	}
	//[2]
	if (bResult)
	{
		g_DeviceData.DeviceSpeed = speed;
		bResult = WinUsb_QueryInterfaceSettings(g_DeviceData.WinUsbHandle, 0, &ifaceDescriptor);
		DebugPrintW(L"DeviceSpeed: %d", g_DeviceData.DeviceSpeed);
	}
	if (bResult)
	{
		for (int i = 0; i < ifaceDescriptor.bNumEndpoints; i++)
		{
			//[3]
			bResult = WinUsb_QueryPipe(g_DeviceData.WinUsbHandle,
				0,
				(UCHAR)i,
				&pipeInfo);
			//[4]
			if (pipeInfo.PipeType == UsbdPipeTypeBulk &&
				USB_ENDPOINT_DIRECTION_IN(pipeInfo.PipeId))
			{
				g_DeviceData.uBulkInPipeId = pipeInfo.PipeId;
				DebugPrintW(L"BulkInPipeId: %d", g_DeviceData.uBulkInPipeId);
			}
			else if (pipeInfo.PipeType == UsbdPipeTypeBulk &&
				USB_ENDPOINT_DIRECTION_OUT(pipeInfo.PipeId))
			{
				g_DeviceData.uBulkOutPipeId = pipeInfo.PipeId;
				DebugPrintW(L"uBulkOutPipeId: %d", g_DeviceData.uBulkOutPipeId);
			}
			else if (pipeInfo.PipeType == UsbdPipeTypeInterrupt)
			{
				g_DeviceData.uInterruptPipeId = pipeInfo.PipeId;
				DebugPrintW(L"uInterruptPipeId: %d", g_DeviceData.uInterruptPipeId);
			}
			else
			{
				bResult = FALSE;
				break;
			}
		}
	}
RET_LABEL:
	return bResult;
}

BOOL WriteToDevice()
{
//	USHORT bufSize = 12;
	BYTE szBuffer[64];
	BOOL bResult;
	ULONG bytesWritten;
	//[1]
/*	SendMessage(hwndWriteEdit, EM_GETLINE, 0, (LPARAM)szBuffer);*/

	//[2]
	bResult = WinUsb_WritePipe(g_DeviceData.WinUsbHandle,
		g_DeviceData.uBulkOutPipeId,
		szBuffer,
		64,
		&bytesWritten,
		NULL);
	if (bResult == FALSE)
	{
		PrintError(L"Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
	}
	else
	{
		DebugPrintW(L"Write OK");
	}
	return bResult;
}

BOOL ReadFromDevice()
{
//	USHORT bufSize = 12;
	CHAR szBuffer[64];
	BOOL bResult;
	ULONG bytesRead = 0;

 	memset(szBuffer, 0, 64);
 	strcpy_s(szBuffer, 64, "Hello from User");
 	DebugPrint("String Got: %s", szBuffer);

	bResult = WinUsb_ReadPipe(g_DeviceData.WinUsbHandle,
		g_DeviceData.uBulkInPipeId,
		(BYTE*)szBuffer,
		64,
		&bytesRead,
		NULL);
	if (bResult == FALSE)
	{
		PrintError(L"Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
	}
	else
	{
		DebugPrintW(L"Read OK, %d"), bytesRead;
		DebugPrint("\nString Got: %s", szBuffer);
	}
	return bResult;
}