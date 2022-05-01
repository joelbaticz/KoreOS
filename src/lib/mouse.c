/*
  - Date: 2021-11-11
  - Author: Joel
  - Purpose: PS/2 mouse driver
*/

#include <kldr.h>

#define MOUSE_WAIT_ON_DATA   0
#define MOUSE_WAIT_ON_SIGNAL 1

BYTE mouseCycle;	    // Helps which byte is being sent from the mouse
INT8 mouseState[3];     // 3 byte data structure that holds the mouse state

VOID MousePerformWait(BYTE waitType);
VOID MouseWrite(BYTE byteToWrite);
BYTE MouseRead();

VOID MouseDriverInit()
{
    mouseCycle = 0;

    BYTE status;

    // Enable mouse device
    MousePerformWait(MOUSE_WAIT_ON_SIGNAL);
    MachineIoWritePortUint8(0x64, 0xa8);
    MouseRead();                   // Acknowledge

    // Enable interrupts
    MousePerformWait(MOUSE_WAIT_ON_SIGNAL);
    MachineIoWritePortUint8(0x64, 0x20);

    // Get status byte
    MousePerformWait(MOUSE_WAIT_ON_DATA);
    status = MachineIoReadPortUint8(0x60);

    // Enable IRQ
    status |= 0x2;

    // Disable mouse clock
    //status &= !0x20;

    // Command: set compaq status
    MousePerformWait(MOUSE_WAIT_ON_SIGNAL);
    MachineIoWritePortUint8(0x64, 0x60);

    // Send status byte
    MousePerformWait(MOUSE_WAIT_ON_SIGNAL);
    MachineIoWritePortUint8(0x60, status);

    // Use default mouse settings
    MouseWrite(0xf6);
    MouseRead();                   // Acknowledge

    // Enable mouse data reporting
    MouseWrite(0xf4);
    MouseRead();                   // Acknowledge

    // Install mouse handler
    //IrqInstallHandler(12, MouseHandler);
    //PicEnableIRQ(12);

    mouseServiceInstalled = TRUE;
}  

VOID MousePerformWait(BYTE waitType)
{
    UINT32 timeOutInCycles = 100000;

    if (waitType == MOUSE_WAIT_ON_DATA)
    {
        while(timeOutInCycles--)
        {
            if ((MachineIoReadPortUint8(0x64) & 1) == 1)
            {
                return;
            }
        }
    }
    else
    {
        while(timeOutInCycles--)
        {
            if ((MachineIoReadPortUint8(0x64) & 2) == 0)
            {
                return;
            }
        }
    }  
}

VOID MouseWrite(BYTE byteToWrite)
{
    MousePerformWait(MOUSE_WAIT_ON_SIGNAL);
    MachineIoWritePortUint8(0x64, 0xd4);

    MousePerformWait(MOUSE_WAIT_ON_SIGNAL);
    MachineIoWritePortUint8(0x60, byteToWrite);
}

BYTE MouseRead()
{
    MousePerformWait(MOUSE_WAIT_ON_DATA);
    
    return MachineIoReadPortUint8(0x60);
}

VOID MouseHandler()
{
    MOUSE_STATE mouseCurrentState;

    mouseState[mouseCycle] = MachineIoReadPortUint8(0x60);
    mouseCycle++;

    if (mouseCycle == 3)
    {
        UINT8 state;
	    INT8 deltaX;
        INT8 deltaY;
        INT8 relativeX = 0;
        INT8 relativeY = 0;

        state = mouseState[0];
        deltaX = mouseState[1];
        deltaY = mouseState[2];
        
        relativeX = deltaX - ((state << 4) & 0x100);
	    relativeY = deltaY - ((state << 3) & 0x100);

        mouseCurrentState.RelativeX = relativeX;
        mouseCurrentState.RelativeY = relativeY;
        mouseCurrentState.IsLeftButtonDown = state & 1;
        mouseCurrentState.IsRightButtonDown = state >> 1 & 1;
        mouseCurrentState.IsMiddleButtonDown = state >> 2 & 1;

        UpdateMouse(mouseCurrentState);
        
        mouseCycle = 0;
    }
}
