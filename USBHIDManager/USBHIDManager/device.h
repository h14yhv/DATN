//
// Define below GUIDs
//
#include <initguid.h>

//
// Device Interface GUID.
// Used by all WinUsb devices that this application talks to.
// Must match "DeviceInterfaceGUIDs" registry value specified in the INF file.
// 6d22450a-fe34-46d2-b019-65dba1ae0aaa
//
DEFINE_GUID(GUID_DEVINTERFACE_USBHIDManager,
    0x6d22450a,0xfe34, 0x46d2,0xb0,0x19,0x65,0xdb,0xa1,0xae,0x0a,0xaa);

typedef struct _DEVICE_DATA {

    BOOL                    HandlesOpen;
    WINUSB_INTERFACE_HANDLE WinUsbHandle;
    HANDLE                  DeviceHandle;
    TCHAR                   DevicePath[MAX_PATH];
	UCHAR					DeviceSpeed;
	UCHAR					uBulkInPipeId;
	UCHAR					uBulkOutPipeId;
	UCHAR					uInterruptPipeId;
} DEVICE_DATA, *PDEVICE_DATA;

HRESULT
OpenDevice(
    _Out_     PDEVICE_DATA DeviceData,
    _Out_opt_ PBOOL        FailureDeviceNotFound
    );

VOID
CloseDevice(
    _Inout_ PDEVICE_DATA DeviceData
    );

BOOL GetConfigDevice();

BOOL ReadFromDevice();

BOOL WriteToDevice();