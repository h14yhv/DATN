#include "USBTokenManagerForm.h"
#include "device.h"
#include "Common.h"

using namespace System;

DEVICE_DATA g_DeviceData = { 0 };

int main()
{
	BOOL                  noDevice;
	HRESULT               hr;

	hr = OpenDevice(&g_DeviceData, &noDevice);
	if (FAILED(hr)) 
	{
		if (noDevice) 
		{
			DebugPrint("Device not connected or driver not installed\n");
		}
		else 
		{
			DebugPrint(("Failed looking for device, HRESULT 0x%x\n"), hr);
		}
		RET_THIS;
	}

	System::Windows::Forms::Application::Run(gcnew USBTokenManager::USBTokenManagerForm());

RET_LABEL:
	return 0;
}