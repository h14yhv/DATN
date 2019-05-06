#include "Common.h"
#include <iostream>
#include "CommunicateDLL.h"
#include "device.h"

typedef int(__stdcall *OpenDevice_ptr)(PDEVICE_DATA DeviceData, PBOOL FailureDeviceNotFound);
typedef int(__stdcall *CloseDevice_ptr)(PDEVICE_DATA DeviceData);
typedef int(__stdcall *GetConfigDevice_ptr)();
typedef int(__stdcall *AuthenticateDevice_ptr)(PCHAR szUserName, PCHAR szPasswordHashed);
typedef int(__stdcall *FlushDevice_ptr)();
typedef int(__stdcall *SetPasswordDevice_ptr)(PCHAR szPasswordHashed);
typedef int(__stdcall *WriteSignature_ptr)(PBYTE pSignature, USHORT usSignSize);
typedef int(__stdcall *ReadSignature_ptr)(PBYTE pSignature, PUSHORT usSignSizeTransferred);

INT GetProcAddressFromDLL()
{
	HINSTANCE hGetProcIDDLL = LoadLibrary(L"USBTokenManagerDLL.dll");

	if (!hGetProcIDDLL) {
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		return FALSE;
	}

	// resolve function address here
	OpenDevice_ptr ptrOpenDevice = (OpenDevice_ptr)GetProcAddress(hGetProcIDDLL, "OpenDevice");
	if (!ptrOpenDevice) 
	{
		PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		return FALSE;
	}
	return TRUE;
}