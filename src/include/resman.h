/*
  - Date: 2021-08-22
  - Author: Joel
  - Purpose: Definitions for ResourceManager
*/

#ifndef __RESMAN_H
#define __RESMAN_H

VOID ResourceManagerInit();

KSTATUS DriverCreate(PCHAR pObjectName, PCHAR pDriverName, DRIVER_FLAGS flags, PVOID pDriverInit, PVOID pDriverUnload, PVOID pDriverDispatchFunction, OUT PDRIVER_OBJECT* pDriverObject);
KSTATUS DriverDelete(PDRIVER_OBJECT pDriverObject);

KSTATUS DeviceCreate(PCHAR pObjectName, PCHAR pDeviceName, PDRIVER_OBJECT pDriver, DEVICE_TYPE deviceType, DEVICE_CLASS deviceClass, OUT PDEVICE_OBJECT* pDeviceObject);
KSTATUS DeviceDelete(PDEVICE_OBJECT pDeviceObject);
KSTATUS DeviceIoControl(HANDLE hDevice, UINT32 ioControlCode, PVOID pInBuffer, UINT32 inBufferSize, PVOID pOutBuffer, UINT32 outBufferSize);

HANDLE FileOpen(PCHAR pPathName, FILE_ACCESS_DESCRIPTOR accessDescriptor);
VOID FileClose(HANDLE hFile);



// hFile = FileOpen(pPathName, accessFlags);
// 
// accessFlags = OPEN_FILE_READ, OPEN_FILE_WRITE, OPEN_FILE_READWRITE, OPEN_FILE_SHARE_DENY_NONE, OPEN_FILE_SHARE_DENY_READ, OPEN_FILE_SHARE_DENY_WRITE, OPEN_FILE_SHARE_EXCLUSIVE
// If SHARE_DENY_XXX or SHARE_EXCLUSIVE the system checks whether the file is already open for that access, can't acquire a handle if yes, otherwise future accesses will be denied
//
// hFile = FileOpen(&"C:\myfile.txt", OPEN_FILE_READ | OPEN_FILE_SHARE_DENY_WRITE);
//   Call will fail if file does not exist or path could not be found. Handle will be -1
//
// hFile = FileCreate(pPathName, attributeFlags);
//   If the file already exists it will truncate it to 0 length
//   Attribute is Normal File, Read-only File, Hidden File, System File
//
// UINT32 filePosition = FileSeek(hFile, position, method);
//    Method is From beginning of File, From current position, From the end of file (backwards)
//
// UINT32 FileLength(hFile);
//   Will use seek to the end of file and restores current pos
//
// UINT32 bytesWritten = FileWrite(hFile, pBuffer, bytesToWrite);
//   If bytesWritten < bytesToWrite the disk is full, while bytesToWrite is essentiall the length of the buffer if the full buffer is to be written
//
// UINT32 bytesRead = FileRead(hFile, pBuffer, bytesToRead);
//   If bytesRead < bytesToRead then EOF was reached
//
// FileClose(hFile);
//   Close file, release handle, free resources
//
// We might want to use a method to receive the LastError to get more information about the error
// FileGetLastError();
//   The last error will be stored on the FileObject. With this method we just return it to the caller.
//   Could be FileGetStatus();



#endif /* __RESMAN_H */