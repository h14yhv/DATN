#include "pch.h"
#include "Common.h"

#include <SetupAPI.h>
extern DEVICE_DATA g_DeviceData;

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

	DeviceData->bIsHandlesOpen = FALSE;

	hr = RetrieveDevicePath(DeviceData->DevicePath,
		sizeof(DeviceData->DevicePath),
		FailureDeviceNotFound);

	if (FAILED(hr)) {

		return hr;
	}

	DeviceData->hFileDeviceHandle = CreateFile(DeviceData->DevicePath,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (INVALID_HANDLE_VALUE == DeviceData->hFileDeviceHandle) {

		hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	bResult = WinUsb_Initialize(DeviceData->hFileDeviceHandle,
		&DeviceData->hWinUSBInterfaceHandle);

	if (FALSE == bResult) {

		hr = HRESULT_FROM_WIN32(GetLastError());
		CloseHandle(DeviceData->hFileDeviceHandle);
		return hr;
	}

	DeviceData->bIsHandlesOpen = TRUE;
	return hr;
}

VOID
CloseDevice(
	_Inout_ PDEVICE_DATA DeviceData
)
{
	if (FALSE == DeviceData->bIsHandlesOpen)
	{
		return;
	}

	WinUsb_Free(DeviceData->hWinUSBInterfaceHandle);
	CloseHandle(DeviceData->hFileDeviceHandle);
	DeviceData->hWinUSBInterfaceHandle = NULL;
	DeviceData->hFileDeviceHandle = NULL;
	DeviceData->bIsHandlesOpen = FALSE;

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
	INT								 iDeviceNumber = 0;

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

		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
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

	if (FALSE == bResult)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		if (ERROR_NO_MORE_ITEMS == GetLastError() &&
			NULL != FailureDeviceNotFound) {

			*FailureDeviceNotFound = TRUE;
		}

		hr = HRESULT_FROM_WIN32(GetLastError());
		SetupDiDestroyDeviceInfoList(deviceInfo);
		return hr;
	}

	do
	{
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
		iDeviceNumber++;
		//
		// Give path to the caller. SetupDiGetDeviceInterfaceDetail ensured
		// DevicePath is NULL-terminated.
		//
		hr = StringCbCopy(DevicePath,
			BufLen,
			detailData->DevicePath);
		DebugPrint("Device Path: %s", DevicePath);
	} while (SetupDiEnumDeviceInterfaces(deviceInfo, NULL, &GUID_DEVINTERFACE_USBHIDManager, iDeviceNumber, &interfaceData));


	LocalFree(detailData);
	SetupDiDestroyDeviceInfoList(deviceInfo);

	return hr;
}

BOOL GetConfigDevice()
{
	BOOL bResult;
	USB_INTERFACE_DESCRIPTOR ifaceDescriptor = { 0 };
	WINUSB_PIPE_INFORMATION pipeInfo;
	UCHAR speed;
	ULONG length;

	length = sizeof(UCHAR);
	bResult = WinUsb_QueryDeviceInformation(g_DeviceData.hWinUSBInterfaceHandle,
		DEVICE_SPEED,
		&length,
		&speed);
	if (bResult == FALSE)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bResult, FALSE);
	}
	bResult = WinUsb_QueryInterfaceSettings(g_DeviceData.hWinUSBInterfaceHandle, 0, &ifaceDescriptor);
	DebugPrint("DeviceSpeed: %d", speed);

	if (bResult)
	{
		for (int i = 0; i < ifaceDescriptor.bNumEndpoints; i++)
		{
			bResult = WinUsb_QueryPipe(g_DeviceData.hWinUSBInterfaceHandle,
				0,
				(UCHAR)i,
				&pipeInfo);

			if (pipeInfo.PipeType == UsbdPipeTypeBulk &&
				USB_ENDPOINT_DIRECTION_IN(pipeInfo.PipeId))
			{
				g_DeviceData.uBulkInPipeId = pipeInfo.PipeId;
				DebugPrint("BulkInPipeId: %d", g_DeviceData.uBulkInPipeId);
			}
			else if (pipeInfo.PipeType == UsbdPipeTypeBulk &&
				USB_ENDPOINT_DIRECTION_OUT(pipeInfo.PipeId))
			{
				g_DeviceData.uBulkOutPipeId = pipeInfo.PipeId;
				DebugPrint("uBulkOutPipeId: %d", g_DeviceData.uBulkOutPipeId);
			}
			else if (pipeInfo.PipeType == UsbdPipeTypeInterrupt)
			{
				g_DeviceData.uInterruptPipeId = pipeInfo.PipeId;
				DebugPrint("uInterruptPipeId: %d", g_DeviceData.uInterruptPipeId);
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

BOOL WriteToDevice(PUCHAR pDataMessage, ULONG ulBufferLength, PULONG pulLenghTransferred, LPOVERLAPPED pOverlapppedSync, DWORD dwTimeOut)
{
	BOOL bResult = FALSE;
	DWORD dwRetWait = 0;

	bResult = WinUsb_WritePipe(g_DeviceData.hWinUSBInterfaceHandle,
		g_DeviceData.uBulkOutPipeId,
		(PUCHAR)pDataMessage,
		ulBufferLength,
		pulLenghTransferred,
		pOverlapppedSync);
	if (bResult == FALSE && GetLastError() != ERROR_IO_PENDING)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		return FALSE;
	}

	if (pOverlapppedSync->hEvent == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		return FALSE;
	}

	dwRetWait = WaitForSingleObject(pOverlapppedSync->hEvent, dwTimeOut);
	if (dwRetWait == WAIT_TIMEOUT || dwRetWait == WAIT_FAILED)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		return FALSE;
	}

	return TRUE;
}

BOOL ReadFromDevice(PUCHAR pDataMessage, ULONG ulBufferLength, PULONG pulLenghTransferred, LPOVERLAPPED pOverlapppedSync, DWORD dwTimeOut)
{
	BOOL bResult;
	DWORD dwRetWait = 0;

	bResult = WinUsb_ReadPipe(g_DeviceData.hWinUSBInterfaceHandle,
		g_DeviceData.uBulkInPipeId,
		(PUCHAR)pDataMessage,
		ulBufferLength,
		pulLenghTransferred,
		pOverlapppedSync);
	if (bResult == FALSE && GetLastError() != ERROR_IO_PENDING)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		return FALSE;
	}

	if (pOverlapppedSync->hEvent == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		return FALSE;
	}

	dwRetWait = WaitForSingleObject(pOverlapppedSync->hEvent, dwTimeOut);
	if (dwRetWait == WAIT_TIMEOUT || dwRetWait == WAIT_FAILED)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		return FALSE;
	}

	DebugPrint("String: %s", pDataMessage);

	return TRUE;
}

BOOL FlushDevice()
{
	BOOL bStatus = TRUE;
	ULONG ulDataSizeTransferred = 0;
	OVERLAPPED OverlapppedSync = { 0 };
	PDATA_MESSAGE pDataMessage = NULL;

	pDataMessage = (PDATA_MESSAGE)ALLOC(sizeof(DATA_MESSAGE));
	if (pDataMessage == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	bStatus = WinUsb_FlushPipe(g_DeviceData.hWinUSBInterfaceHandle, g_DeviceData.uBulkInPipeId);
	if (bStatus == FALSE)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	OverlapppedSync.hEvent = CreateEventW(NULL, FALSE, FALSE, L"IsFlushedEvent");
	if (OverlapppedSync.hEvent == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	//Check Is Pipe clean, if read true is fault
	bStatus = ReadFromDevice((PUCHAR)pDataMessage, sizeof(DATA_MESSAGE), &ulDataSizeTransferred, &OverlapppedSync, CHECK_CLEAN_TIME);
	if (bStatus == TRUE || ulDataSizeTransferred != 0)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}
RET_LABEL:
	FREE(pDataMessage);
	CLOSE_HANDLE(OverlapppedSync.hEvent);
	return bStatus;
}

BOOL SetPasswordDevice(BYTE* bPasswordHashed)
{
	BOOL bStatus = TRUE;
	ULONG ulDataSizeTransferred = 0;
	OVERLAPPED OverlapppedSync = { 0 };
	PDATA_MESSAGE pDataMessage = NULL;

	pDataMessage = (PDATA_MESSAGE)ALLOC(sizeof(DATA_MESSAGE));
	if (pDataMessage == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	bStatus = FlushDevice();
	if (bStatus == FALSE)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	OverlapppedSync.hEvent = CreateEventW(NULL, FALSE, FALSE, L"SetPasswordEvent");
	if (OverlapppedSync.hEvent == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	//Set Data Message
	pDataMessage->iCmd = SET_PASSWORD;
	memcpy_s(pDataMessage->szPasswordHashed, PASSWORD_SIZE, bPasswordHashed, sizeof(bPasswordHashed));

	//Send Data Message
	bStatus = WriteToDevice((PUCHAR)pDataMessage, sizeof(DATA_MESSAGE), &ulDataSizeTransferred, &OverlapppedSync, WAIT_TIME);
	if (bStatus == FALSE || ulDataSizeTransferred == 0)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	//Get Data Message from Device
	bStatus = ReadFromDevice((PUCHAR)pDataMessage, sizeof(DATA_MESSAGE), &ulDataSizeTransferred, &OverlapppedSync, WAIT_TIME);
	if (bStatus == FALSE || ulDataSizeTransferred == 0)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	DebugPrint("SetPasswordDevice OK");
RET_LABEL:
	FREE(pDataMessage);
	CLOSE_HANDLE(OverlapppedSync.hEvent);
	return bStatus;
}

BOOL AuthenticateDevice(BYTE* bPasswordHashed)
{
	BOOL bStatus = TRUE;
	PDATA_MESSAGE pDataMessage = NULL;
	ULONG ulDataSizeTransferred = 0;
	OVERLAPPED OverlapppedSync = { 0 };

	pDataMessage = (PDATA_MESSAGE)ALLOC(sizeof(DATA_MESSAGE));
	if (pDataMessage == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	bStatus = FlushDevice();
	if (bStatus == FALSE)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	OverlapppedSync.hEvent = CreateEventW(NULL, FALSE, FALSE, L"AuthenticateEvent");
	if (OverlapppedSync.hEvent == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	pDataMessage->iCmd = AUTHENTICATE; 
	memcpy_s(pDataMessage->szPasswordHashed, PASSWORD_SIZE, bPasswordHashed, sizeof(bPasswordHashed));

	//Send Data Message
	bStatus = WriteToDevice((PUCHAR)pDataMessage, sizeof(DATA_MESSAGE), &ulDataSizeTransferred, &OverlapppedSync, WAIT_TIME);
	if (bStatus == FALSE || ulDataSizeTransferred == 0)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	//Get Data Message from Device
	bStatus = ReadFromDevice((PUCHAR)pDataMessage, sizeof(DATA_MESSAGE), &ulDataSizeTransferred, &OverlapppedSync, WAIT_TIME);
	if (bStatus == FALSE || ulDataSizeTransferred == 0)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	DebugPrint("Authenticate OK");
RET_LABEL:
	FREE(pDataMessage);
	CLOSE_HANDLE(OverlapppedSync.hEvent);
	return bStatus;
}

BOOL ReadSignature()
{
	BOOL bStatus = TRUE;
	PDATA_MESSAGE pDataMessage = NULL;
	ULONG ulDataSizeTransferred = 0;
	OVERLAPPED OverlapppedSync = { 0 };

	pDataMessage = (PDATA_MESSAGE)ALLOC(sizeof(DATA_MESSAGE));
	if (pDataMessage == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	bStatus = FlushDevice();
	if (bStatus == FALSE)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	OverlapppedSync.hEvent = CreateEventW(NULL, FALSE, FALSE, L"ReadSignatureEvent");
	if (OverlapppedSync.hEvent == NULL)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

	pDataMessage->iCmd = AUTHENTICATE;
//	memcpy_s(pDataMessage->szPasswordHashed, PASSWORD_SIZE, bPasswordHashed, sizeof(bPasswordHashed));

	//Send Data Message
	bStatus = WriteToDevice((PUCHAR)pDataMessage, sizeof(DATA_MESSAGE), &ulDataSizeTransferred, &OverlapppedSync, WAIT_TIME);
	if (bStatus == FALSE || ulDataSizeTransferred == 0)
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		RET_THIS_STATUS(bStatus, FALSE);
	}

// 	do 
// 	{
// 	} while ();

RET_LABEL:
	FREE(pDataMessage);
	CLOSE_HANDLE(OverlapppedSync.hEvent);
	return bStatus;
}