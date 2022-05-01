/*
  - Date: 2021-08-22
  - Author: Joel
  - Purpose: Kernel object definitions
*/

#ifndef __KOBJECTS_H
#define __KOBJECTS_H

#define DRIVER_MAX_FUNCTION_NUMBER  12

#define DRIVER_FUNCTION_INITIALIZE           0x0001  // Driver's initialization method (used for Plug & Play drivers only, for boot drivers the driver should do any setup in it's driver entry phase)
#define DRIVER_FUNCTION_TEARDOWN             0x0002  // The tear-down routine where the driver should free up any resources held and reset it's devices if it's meaningful
#define DRIVER_FUNCTION_CREATE               0x0003  // Driver should allocate resources in order to serve subsequent functions of read/write operations
#define DRIVER_FUNCTION_CLOSE                0x0004  // Driver can release resources for this open device
#define DRIVER_FUNCTION_READ                 0x0005  // Read operation
#define DRIVER_FUNCTION_WRITE                0x0006  // Write operation
#define DRIVER_FUNCTION_DEVICE_CONTROL       0x0007  // Device control thru IOCTL codes
#define DRIVER_FUNCTION_FILE_SYSTEM_CONTROL  0x0008  // File System specific control codes via FSCTL codes
#define DRIVER_FUNCTION_PLUG_AND_PLAY        0x0009  // Used when the IoManager calls the driver to identify wheter it can handle a specific device
#define DRIVER_FUNCTION_QUERY_INFORMATION    0x000A  // Get information out of the driver or one of it's devices
#define DRIVER_FUNCTION_SET_INFORMATION      0x000B  // Set information which alter driver settings
#define DRIVER_FUNCTION_SHUT_DOWN            0x000C  // The system is shutting down, the driver should close every device and perform the tear down operation

typedef struct _DEVICE_OBJECT
{
  DEVICE_TYPE DeviceType;       // The device type, like "StandardFloppyDiskDrive"
  DEVICE_CLASS DeviceClass;     // The device class like "DiskDevice"
  PCHAR pDeviceName;            // The device description if any like "3.5" Floppy Disk Drive"
  PVOID pDriverObject;          // Pointer to the driver object that manages this device
  PVOID pNextDevice;            // The next device managed by the same driver
  UINT32 ReferenceCount;        // Reference count
  DEVICE_FLAGS Flags;
  UINT32 SectorSize;            // The sector size for block devices if applicable
} DEVICE_OBJECT, *PDEVICE_OBJECT;

typedef struct _DRIVER_OBJECT
{
  PDEVICE_OBJECT pDeviceObject; // The first device that is managed by this driver
  PCHAR pDriverName;            // The optional driver description
  DRIVER_FLAGS Flags;
  PVOID pDriverInit;            // The driver's init routine
  PVOID pDriverUnload;          // The driver's unload routine
  //PDRIVER_DISPATCH pDriverDispatchFunctions; //[MAX_DISPATCH_FUNCTION - 1]; // The different I/O control functions that the driver supports
  PVOID pDriverDispatchFunction;
} DRIVER_OBJECT, *PDRIVER_OBJECT;

typedef UINT32 FILE_ACCESS_DESCRIPTOR;

#define FILE_OPEN_READ              0x0001
#define FILE_OPEN_WRITE             0x0002
#define FILE_OPEN_READWRITE         0x0003
#define FILE_OPEN_SHARE_DENY_NONE   0x0000
#define FILE_OPEN_SHARE_DENY_READ   0x0010
#define FILE_OPEN_SHARE_DENY_WRITE  0x0020
#define FILE_OPEN_SHARE_EXCLUSIVE   0x0030

typedef struct _FILE_OBJECT
{
  PCHAR pPathName;                          // The file name or device name
  PDEVICE_OBJECT pDeviceObject;             // The device object the file was opened on
  UINT32 CurrentByteOffset;                 // The current offset where the file pointer points to
  FILE_ACCESS_DESCRIPTOR AccessDescriptor;  // Access descriptor
} FILE_OBJECT, *PFILE_OBJECT;

#define IO_STATUS_SUCCESS                     0x0001
#define IO_STATUS_ERROR_UKNOWN                0x0002
#define IO_STATUS_ERROR_INVALID_REQUEST       0x0003
#define IO_STATUS_ERROR_INVALID_PARAMETER     0x0004
#define IO_STATUS_ERROR_INVALID_BUFFER_SIZE   0x0005

/* I/O Request Packet */
typedef struct _IRP
{
  PFILE_OBJECT pFile;
  PDEVICE_OBJECT pDevice;
  PDRIVER_OBJECT pDriver;
  UINT32 ioControlCode;
  UINT32 ioStatus;
  PVOID pInBuffer;
  UINT32 inBufferSize;
  PVOID pOutBuffer;
  UINT32 outBufferSize;
} IRP, *PIRP;

// typedef struct _DRIVER_FUNCTIONS
// {
//   VOID (*pDriverFunction[DRIVER_MAX_FUNCTION_NUMBER])(PDEVICE_OBJECT pDeviceObject, PIRP pIRP);
// } DRIVER_FUNCTIONS, *PDRIVER_FUNCTIONS;

typedef struct _DRIVER_DISPATCH_FUNCTION
{
  VOID (*pDriverDispatchFunction)(PDEVICE_OBJECT pDeviceObject, PIRP pIRP);
} DRIVER_DISPATCH_FUNCTION, *PDRIVER_DISPATCH_FUNCTION;

#endif /* __KOBJECTS_H */