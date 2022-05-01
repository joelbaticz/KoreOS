/*
  - Date: 2021-08-17
  - Author: Joel
  - Purpose: IoManager
*/

#include <kldr.h>

ISR_VTBL isrFunctions;
//struct interrupt_frame;

// #define uword_t unsigned int
// struct interrupt_frame
// {
//      uword_t ip;
//      uword_t cs;
//      uword_t flags;
// };

unsigned int irq0FiredCount;
CHAR text[80];
PCHAR pText;

UINT8 crapColor;
UINT32 locationX;

VOID SetISRFunctionEntries();

VOID  HandleINT0(PINTERRUPTFRAME pFrame);

VOID HandleIRQ0(PINTERRUPTFRAME pFrame);
VOID HandleIRQ1(PINTERRUPTFRAME pFrame);
VOID HandleIRQ2(PINTERRUPTFRAME pFrame);
VOID HandleIRQ3(PINTERRUPTFRAME pFrame);
VOID HandleIRQ4(PINTERRUPTFRAME pFrame);
VOID HandleIRQ5(PINTERRUPTFRAME pFrame);
VOID HandleIRQ6(PINTERRUPTFRAME pFrame);
VOID HandleIRQ7(PINTERRUPTFRAME pFrame);
VOID HandleIRQ8(PINTERRUPTFRAME pFrame);
VOID HandleIRQ9(PINTERRUPTFRAME pFrame);
VOID HandleIRQ10(PINTERRUPTFRAME pFrame);
VOID HandleIRQ11(PINTERRUPTFRAME pFrame);
VOID HandleIRQ12(PINTERRUPTFRAME pFrame);
VOID HandleIRQ13(PINTERRUPTFRAME pFrame);
VOID HandleIRQ14(PINTERRUPTFRAME pFrame);
VOID HandleIRQ15(PINTERRUPTFRAME pFrame);

VOID HandleInvalidInterrupt();

VOID DrawCrap();

VOID IoManagerInit()
{
    irq0FiredCount = 0;
    crapColor = 0;
    locationX = 0;
    
    for(UINT32 i = 0; i < 80; i++)
    {
        text[i] = 0;
    }    

    SetISRFunctionEntries();

    MachineInitializeInterruptVectorTable(&isrFunctions);

    UiDrawText("IoManagerInit................DONE");
}

VOID SetISRFunctionEntries()
{
    isrFunctions.isrCount = 256;

    for(int i = 0; i < isrFunctions.isrCount; i++)
    {
        isrFunctions.isr[i] = (UINT32) (HandleInvalidInterrupt);
    }

    // Exceptions
    isrFunctions.isr[0x0] = (UINT32) (HandleINT0);

    // IRQs
    isrFunctions.isr[0x20] = (UINT32) (HandleIRQ0);
    isrFunctions.isr[0x21] = (UINT32) (HandleIRQ1);
    //isrFunctions.isr[0x22] = (UINT32) (HandleIRQ2);
    isrFunctions.isr[0x23] = (UINT32) (HandleIRQ3);
    isrFunctions.isr[0x24] = (UINT32) (HandleIRQ4);
    isrFunctions.isr[0x25] = (UINT32) (HandleIRQ5);
    isrFunctions.isr[0x26] = (UINT32) (HandleIRQ6);
    isrFunctions.isr[0x27] = (UINT32) (HandleIRQ7);
    isrFunctions.isr[0x28] = (UINT32) (HandleIRQ8);
    isrFunctions.isr[0x29] = (UINT32) (HandleIRQ9);
    isrFunctions.isr[0x2A] = (UINT32) (HandleIRQ10);
    isrFunctions.isr[0x2B] = (UINT32) (HandleIRQ11);
    isrFunctions.isr[0x2C] = (UINT32) (HandleIRQ12);
    isrFunctions.isr[0x2D] = (UINT32) (HandleIRQ13);
    isrFunctions.isr[0x2E] = (UINT32) (HandleIRQ14);
    isrFunctions.isr[0x2F] = (UINT32) (HandleIRQ15);
}

// INT0 - Division by zero
__attribute__ ((interrupt))
VOID  HandleINT0(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();

    BugCheck(pFrame, (PCHAR) &"Division by zero.", NULL, NULL, 0, NULL, 0);
    
    MachineEnableInterrupts();
}

// IRQ0 - Timer - responds on 0x20 interrupt
__attribute__ ((interrupt))
VOID  HandleIRQ0(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();

    //IncrementDateTime();

    MachineAcknowledgeInterrupt(0);
    MachineEnableInterrupts(); 
}

__attribute__ ((interrupt))
VOID HandleIRQ1(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    
    if (keyboardServiceInstalled)
    {
        KeyboardHandler();
    }

    MachineAcknowledgeInterrupt(1);
    MachineEnableInterrupts();     
}

__attribute__ ((interrupt))
VOID HandleIRQ2(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ2\0");
    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(2);
}

__attribute__ ((interrupt))
VOID HandleIRQ3(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ3\0");

//DrawCrap();

    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(3);
}

__attribute__ ((interrupt))
VOID HandleIRQ4(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ4\0");

//DrawCrap();

    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(4);
}

__attribute__ ((interrupt))
VOID HandleIRQ5(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ5\0");
    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(5);
}

__attribute__ ((interrupt))
VOID HandleIRQ6(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ6\0");
    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(6);
}

__attribute__ ((interrupt))
VOID HandleIRQ7(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ7\0");
    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(7);
}

__attribute__ ((interrupt))
VOID HandleIRQ8(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ8\0");
//DrawCrap();

    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(8);
}

__attribute__ ((interrupt))
VOID HandleIRQ9(PINTERRUPTFRAME pFrame)
{
    //MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ9\0");
    
//IoNotifyClient();
    //DrawCrap();

    MachineAcknowledgeInterrupt(9);
    //MachineEnableInterrupts();     
}

__attribute__ ((interrupt))
VOID HandleIRQ10(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ10\0");
    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(10);
}

__attribute__ ((interrupt))
VOID HandleIRQ11(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ11\0");
    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(11);
}

__attribute__ ((interrupt))
VOID HandleIRQ12(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    
    if (mouseServiceInstalled)
    {
        MouseHandler();
    }
    
    MachineAcknowledgeInterrupt(12);
    MachineEnableInterrupts();
}

__attribute__ ((interrupt))
VOID HandleIRQ13(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ13\0");
    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(13);
}

__attribute__ ((interrupt))
VOID HandleIRQ14(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ14\0");
    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(14);
}

__attribute__ ((interrupt))
VOID HandleIRQ15(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();
    UiDrawBox(0, 12, 30, 22, 0x2b, LINE_STYLE_SINGLE, "IRQ15\0");
    MachineEnableInterrupts(); 
    MachineAcknowledgeInterrupt(15);
}

__attribute__ ((interrupt))
VOID HandleInvalidInterrupt(PINTERRUPTFRAME pFrame)
{
    MachineDisableInterrupts();

    UiDrawBox(32, 12, 62, 22, 0x1b, LINE_STYLE_SINGLE, "Invalid!\0");

    //DrawCrap();

    MachineAcknowledgeInterrupt(12);

    MachineEnableInterrupts(); 
    //HALT;
}

KSTATUS IoCallDriver(PFILE_OBJECT pFile, PDEVICE_OBJECT pDevice, PDRIVER_OBJECT pDriver, UINT32 ioControlCode, PVOID pInBuffer, UINT32 inBufferSize, PVOID pOutBuffer, UINT32 outBufferSize)
{
    HANDLE hIRP = (HANDLE)NULL;
    PIRP pIRP = NULL;
    BOOL isNoLock;
    KSTATUS result = STATUS_ERROR_UNKNOWN;

    //UiDrawText("IoCallDriver() called.");

    // Allocate memory for a new IoRequestPacket
    hIRP = MemoryAllocate(sizeof(IRP), MEMORY_FLAG_ZERO_INIT, (PCHAR) &"IRP");
    pIRP = (PIRP) MemoryLock(hIRP);

    // Fill in the IoRequestPacket with information
    pIRP->pFile = pFile;
    pIRP->pDevice = pDevice;
    pIRP->pDriver = pDriver;
    pIRP->ioControlCode = ioControlCode;
    pIRP->ioStatus = IO_STATUS_ERROR_UKNOWN;
    pIRP->pInBuffer = pInBuffer;
    pIRP->inBufferSize = inBufferSize;
    pIRP->pOutBuffer = pOutBuffer;
    pIRP->outBufferSize = outBufferSize;

    // Send request to driver
    //((PDRIVER_FUNCTIONS)pDriver->pDriverFunctions)->pDriverFunction[DRIVER_FUNCTION_DEVICE_CONTROL](pDevice, pIRP);
    //((PDRIVER_DISPATCH_FUNCTION)pDriver->pDriverDispatchFunction)->pDriverDispatchFunction(pDevice, pIRP);

    //pDriver->pDriverDispatchFunction();
    ((VOID (*)(PDEVICE_OBJECT, PIRP)) pDriver->pDriverDispatchFunction)(pDevice, pIRP);

    if (pIRP->ioStatus == IO_STATUS_SUCCESS)
    {
        result = STATUS_SUCCESS;
    }
    else
    {
        UiDrawText("IO_STATUS: Not successful!");
    }

    // Free memory occupied by the IoRequestPacket
    isNoLock = MemoryUnlock(hIRP);

    if (isNoLock)
    {
        MemoryFree(hIRP);
    }    

    return result;
}

VOID DrawCrap()
{
    PCHAR pFrameBuffer = (PCHAR) 0xA0000;

    UINT32 i = locationX;

    for(UINT32 y = 0; y < 40; y++)
    {
        for(UINT32 x = i; x < i + 40; x++)
        {
            pFrameBuffer[x] = crapColor;
        }

        pFrameBuffer += 320;
    }

locationX++;

    if (crapColor == 50)
    {
        crapColor = 100;
    }
    else
    {
        crapColor = 50;
    }
}

