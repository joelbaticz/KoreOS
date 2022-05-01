/*
  - Date: 2021-08-22
  - Author: Joel
  - Purpose: ResourceManager
*/

#include <kldr.h>

typedef UINT32 OBJECT_TYPE;

typedef struct _OBJECT_DIRECTORY_ENTRY
{
    PCHAR pObjectName;      // Fully qualified name of the object (like: "\VideoAdapter0" or "\HardDiskVolume0\Directory\File.Ext")
    OBJECT_TYPE ObjectType; // The type of the object (file, driver, device, etc.)
    PVOID pObject;          // Pointer to the actual object description (device, driver, symbolic link etc.)
    PVOID pNext;            // Pointer to the next object in the chain
} OBJECT_DIRECTORY_ENTRY, *POBJECT_DIRECTORY_ENTRY;

#define OBJECT_TYPE_DRIVER          0x0001
#define OBJECT_TYPE_DEVICE          0x0002
#define OBJECT_TYPE_FILE            0x0003
#define OBJECT_TYPE_SYMBOLIC_LINK   0x0004
#define OBJECT_TYPE_PROCESS         0x0005
#define OBJECT_TYPE_THREAD          0x0006
#define OBJECT_TYPE_TIMER           0x0007

HANDLE hObjectDirectoryHead;
POBJECT_DIRECTORY_ENTRY pObjectDirectoryHead;

KSTATUS InitObjectDirectory();
KSTATUS AddToObjectDirectory(PCHAR pObjectName, OBJECT_TYPE objectType, PVOID pObjectDescriptor);

VOID ResourceManagerInit()
{
    if (InitObjectDirectory() != STATUS_SUCCESS)
    {
        ERROR("Could not initialize Object Directory!");
        HALT;
    }

    UiDrawText("ResourceManagerInit..........DONE");
}

KSTATUS InitObjectDirectory()
{
    // Create head of linked list

    HANDLE hObjectDirectoryHead = MemoryAllocate(sizeof(OBJECT_DIRECTORY_ENTRY), MEMORY_FLAG_ZERO_INIT, (PCHAR)&"ObjectDirHead");

    if (hObjectDirectoryHead == (HANDLE)NULL)
    {
        return STATUS_ERROR_OUT_OF_MEMORY;
    }

    //TRACE("HANDLE = %x", hObjectDirectoryHead);

    pObjectDirectoryHead = (POBJECT_DIRECTORY_ENTRY) MemoryLock(hObjectDirectoryHead);

    //TRACE("POBJDIR = %x", pObjectDirectoryHead);

    pObjectDirectoryHead->pNext = NULL;
    pObjectDirectoryHead->pObject = NULL;

    return STATUS_SUCCESS;
}

KSTATUS AddToObjectDirectory(PCHAR pObjectName, OBJECT_TYPE objectType, PVOID pObjectDescriptor)
{
    UiDrawText("AddToObjectDirectory()");
    POBJECT_DIRECTORY_ENTRY pNextObject = pObjectDirectoryHead;

    // Find the last entry in the Object Directory

    //while(pNextObject->pObject != NULL)
    while(pNextObject->pNext != NULL)
    {
        pNextObject = pNextObject->pNext;
    }

    // Allocate memory and fill the Object

    PCHAR pObjectType;

    if (objectType == OBJECT_TYPE_DRIVER)
    {
        pObjectType = (PCHAR) &"DriverObject";
    }
    if (objectType == OBJECT_TYPE_DEVICE)
    {
        pObjectType = (PCHAR) &"DeviceObject";
    }
    if (objectType == OBJECT_TYPE_FILE)
    {
        pObjectType = (PCHAR) &"FileObject";
    }

    HANDLE hNewEntry = MemoryAllocate(sizeof(OBJECT_DIRECTORY_ENTRY), MEMORY_FLAG_ZERO_INIT, pObjectType);

    if (hNewEntry == (HANDLE)NULL)
    {
        UiDrawText("Out of mem5.\0");
        return STATUS_ERROR_OUT_OF_MEMORY;
    }

    POBJECT_DIRECTORY_ENTRY pNewObjectEntry = (POBJECT_DIRECTORY_ENTRY) MemoryLock(hNewEntry);

    if (pNewObjectEntry == NULL)
    {
        UiDrawText("Could not lock memory.\0");
        return STATUS_ERROR_OUT_OF_MEMORY;
    }

    pNewObjectEntry->pObjectName = pObjectName;
    pNewObjectEntry->ObjectType = objectType;
    pNewObjectEntry->pObject = pObjectDescriptor;
    pNewObjectEntry->pNext = NULL;

    // Add new entry to previous object
    pNextObject->pNext = pNewObjectEntry;    

    return STATUS_SUCCESS;
}

KSTATUS DriverCreate(PCHAR pObjectName, PCHAR pDriverName, DRIVER_FLAGS flags, PVOID pDriverInit, PVOID pDriverUnload, PVOID pDriverDispatchFunction, OUT PDRIVER_OBJECT* pDriverObject)
{
    // Allocate memory and fill in the Driver Object

    UiDrawText("DriverCreate() called.");
    UiDrawText(pObjectName);

    HANDLE hDriverObject = MemoryAllocate(sizeof(DRIVER_OBJECT), MEMORY_FLAG_ZERO_INIT, pDriverName);

    if (hDriverObject == (HANDLE)NULL)
    {
        UiDrawText("Out of mem1.\0");
        HALT;
        return STATUS_ERROR_OUT_OF_MEMORY;
    }
    
    *pDriverObject = (PDRIVER_OBJECT) MemoryLock(hDriverObject);

    if (pDriverObject == NULL)
    {
        UiDrawText("Out of mem2.\0");
        HALT;
        return STATUS_ERROR_OUT_OF_MEMORY;
    }

    (*pDriverObject)->pDeviceObject = NULL;
    (*pDriverObject)->pDriverName = pDriverName;
    (*pDriverObject)->Flags = flags;
    (*pDriverObject)->pDriverInit = pDriverInit;
    (*pDriverObject)->pDriverUnload = pDriverUnload;
    (*pDriverObject)->pDriverDispatchFunction = pDriverDispatchFunction;

    // Add to object directory
    KSTATUS result = AddToObjectDirectory(pObjectName, OBJECT_TYPE_DRIVER, *pDriverObject);

    return result;
}

KSTATUS DriverDelete(PDRIVER_OBJECT pDriverObject)
{
    NOT_IMPLEMENTED;
    HALT;
}

KSTATUS DeviceCreate(PCHAR pObjectName, PCHAR pDeviceName, PDRIVER_OBJECT pDriver, DEVICE_TYPE deviceType, DEVICE_CLASS deviceClass, OUT PDEVICE_OBJECT* pDeviceObject)
{
    // Allocate memory and fill in the Device Object

    UiDrawText("DeviceCreate() called.");
    UiDrawText(pObjectName);

    HANDLE hDeviceObject = MemoryAllocate(sizeof(DEVICE_OBJECT), MEMORY_FLAG_ZERO_INIT, pDeviceName);

    if (hDeviceObject == (HANDLE)NULL)
    {
        UiDrawText("Out of mem3.\0");
        return STATUS_ERROR_OUT_OF_MEMORY;
    }
    
    *pDeviceObject = (PDEVICE_OBJECT) MemoryLock(hDeviceObject);

    if (pDeviceObject == NULL)
    {
        UiDrawText("Out of mem4.\0");
        return STATUS_ERROR_OUT_OF_MEMORY;
    }

    (*pDeviceObject)->DeviceType = deviceType;
    (*pDeviceObject)->DeviceClass = deviceClass;
    (*pDeviceObject)->pDeviceName = pDeviceName;
    (*pDeviceObject)->pDriverObject = (PDRIVER_OBJECT)pDriver;
    (*pDeviceObject)->pNextDevice = NULL;
    (*pDeviceObject)->ReferenceCount = 0;
    (*pDeviceObject)->Flags = DEVICE_NONE;
    (*pDeviceObject)->SectorSize = 0;    

    // Add this newly created device to the driver's device chain

    //PDEVICE_OBJECT pCurrentDevice = pDriver->pDeviceObject;

    // while(pCurrentDevice->pNextDevice == NULL)
    // {
    //     pCurrentDevice = pCurrentDevice->pNextDevice;
    // }

    // pCurrentDevice->pNextDevice = pDeviceObject;

    // Add to object directory
    KSTATUS result = AddToObjectDirectory(pObjectName, OBJECT_TYPE_DEVICE, *pDeviceObject);

    return result;
}

KSTATUS DeviceDelete(PDEVICE_OBJECT pDeviceObject)
{
    NOT_IMPLEMENTED;
    HALT;
}

HANDLE FileOpen(PCHAR pPathName, FILE_ACCESS_DESCRIPTOR accessDescriptor)
{
    UiDrawText("FileOpen() called.\0");
    UiDrawText(pPathName);

    // Looks up the device named pPathName

    POBJECT_DIRECTORY_ENTRY pNextObject = pObjectDirectoryHead;
    BOOL isDeviceFound = FALSE;
    PDEVICE_OBJECT pDeviceObject = NULL;

    while(pNextObject != NULL && !isDeviceFound)
    {
        //UiDrawText(pNextObject->pObjectName);
        if (pNextObject->pObjectName != NULL && pNextObject->ObjectType == OBJECT_TYPE_DEVICE)
        {
            if (strcmp(pNextObject->pObjectName, pPathName) == 0)
            {
                UiDrawText("DEVICE FOUND.\0");
                isDeviceFound = TRUE;
                pDeviceObject = (PDEVICE_OBJECT) pNextObject->pObject;
            }
        }

        pNextObject = pNextObject->pNext;        
    }

    if (!isDeviceFound)
    {
        UiDrawText("DEVICE NOT FOUND.\0");
        return (HANDLE)NULL;
    }
    
    // Increases the ref count on that device
    // Looks up whether a FileObject with the same name already exists and if the new 
    // FileOpen request can be fulfilled (like if there is already a write access given the next write access will be refused)
    // Creates a FileObject where it stores some data? (like fileName, flags, lastOffset)

    HANDLE hFileObject = MemoryAllocate(sizeof(FILE_OBJECT), MEMORY_FLAG_ZERO_INIT, pPathName);

    if (hFileObject == (HANDLE)NULL)
    {
        UiDrawText("FileOpen() - Out of memory.\0");
        return (HANDLE)NULL;
    }

    PFILE_OBJECT pFileObject = (PFILE_OBJECT) MemoryLock(hFileObject);

    if (pFileObject == NULL)
    {
        UiDrawText("FileOpen() - Out of memory.\0");
        return (HANDLE)NULL;
    }

    pFileObject->pPathName = pPathName;
    pFileObject->pDeviceObject = pDeviceObject;

    pFileObject->CurrentByteOffset = 0;
    pFileObject->AccessDescriptor = accessDescriptor; // TODO: need to check other FileObjects whether read/write access is not denied on this file already

    // Add to object directory
    KSTATUS result = AddToObjectDirectory(pPathName, OBJECT_TYPE_FILE, pFileObject);

    if (result != STATUS_SUCCESS)
    {
        return (HANDLE)NULL;
    }

    pDeviceObject->ReferenceCount++;

    // All OK
    return hFileObject;
}

KSTATUS DeviceIoControl(HANDLE hDevice, UINT32 ioControlCode, PVOID pInBuffer, UINT32 inBufferSize, PVOID pOutBuffer, UINT32 outBufferSize)
{
    UiDrawText("DeviceIoControl()");

    PFILE_OBJECT pFileObject = (PFILE_OBJECT) MemoryLock(hDevice);
    
    if (pFileObject == NULL)
    {
        UiDrawText("File NULL!");
        return STATUS_ERROR_UNKNOWN;
    }

    UiDrawText("PATH:");
    UiDrawText(pFileObject->pPathName);
    
    PDEVICE_OBJECT pDeviceObject = pFileObject->pDeviceObject;

    if (pDeviceObject == NULL)
    {
        UiDrawText("Device NULL!");
        return STATUS_ERROR_UNKNOWN;
    }

    UiDrawText("DEVICE:");
    UiDrawText(pDeviceObject->pDeviceName);

    PDRIVER_OBJECT pDriverObject = (PDRIVER_OBJECT) pDeviceObject->pDriverObject;

    if (pDriverObject == NULL)
    {
        UiDrawText("Driver NULL!");
        return STATUS_ERROR_UNKNOWN;
    }

    UiDrawText("DRIVER:");
    UiDrawText(pDriverObject->pDriverName);

    KSTATUS result = IoCallDriver(pFileObject, pDeviceObject, pDriverObject, ioControlCode, pInBuffer, inBufferSize, pOutBuffer, outBufferSize);

    return result; 
}