/*
  - Date: 2021-08-17
  - Author: Joel
  - Purpose: Definitions for IoManager
*/

#ifndef __IOMAN_H
#define __IOMAN_H

/* Interrupt Service Routine function table */
typedef struct _ISR_VTBL
{	
    UINT32 isrCount;
    UINT32 isr[256];
} ISR_VTBL, *PISR_VTBL;

typedef struct _INTERRUPTFRAME
{
    UINT32 Ip;
    UINT32 Cs;
    UINT32 Eflags;
} INTERRUPTFRAME, *PINTERRUPTFRAME;

VOID IoManagerInit();

KSTATUS IoCallDriver(PFILE_OBJECT pFile, PDEVICE_OBJECT pDevice, PDRIVER_OBJECT pDriver, UINT32 ioControlCode, PVOID pInBuffer, UINT32 inBufferSize, PVOID pOutBuffer, UINT32 outBufferSize);
// It should call the driver's DRIVER_DEVICE_CONTROL function


// IoIrpAllocate - allocates an IoRequestPacket (does the IoMan do it on behalf of the IoCallDriver function or the requestor does this and the IoCallDriver just passes on the IRP?)
// IoFreeIrp     - frees
// IoCallDriver  - call the driver with an IRP

// The IRP just an ties up a big chunk of data together
// Has entries for the DriverObject, DeviceObject, FileObject, buffers
// So the function signature on the Driver side can be only: Dispatch(PDEVICE_OBJECT pDeviceObject, PIRP pIRP)

#endif /* __IOMAN_H */