#using <mscorlib.dll>

#include "USBTokenManagerForm.h"
#include "device.h"
#include "Common.h"

// #define _WIN32_WINNT	_WIN32_WINNT_WIN7
// #define WINVER			_WIN32_WINNT_WIN7

using namespace System;
using namespace System::Windows::Forms;
using namespace USBTokenManager;

DEVICE_DATA g_DeviceData = { 0 };

[STAThread]
int main()
{
 	Application::EnableVisualStyles();
 	Application::SetCompatibleTextRenderingDefault(FALSE);
	System::Windows::Forms::Application::Run(gcnew USBTokenManager::USBTokenManagerForm());

	return 0;
}