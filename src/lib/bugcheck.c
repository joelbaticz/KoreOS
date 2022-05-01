#include "kldr.h"

static CHAR BcTextSystemName[]          = "Kore OS Exception Handler";
static CHAR BcTextException[]           = "An unhandled exception occured.";
static CHAR BcTextExceptionDetails[]    = "Exception details:";

static CHAR BcTextRegisters[]           = "Registers:";

static CHAR buffer[256];


VOID TextBugCheck(PINTERRUPTFRAME pFrame, PCHAR pExceptionType, PCHAR pFile, PCHAR pFunction, UINT32 lineNumber, PREGISTERS pRegs);
VOID GraphicalBugCheck(PINTERRUPTFRAME pFrame, PCHAR pExceptionType, PCHAR pFile, PCHAR pFunction, UINT32 lineNumber, PREGISTERS pRegs);

PCHAR BcGetFormattedText(PCHAR pDestination, char* Format, ...);

VOID BugCheckInit()
{
    // Nothing to init
}

VOID BugCheck(PINTERRUPTFRAME pFrame, PCHAR pExceptionType, PCHAR pFile, PCHAR pFunction, UINT32 lineNumber, PCHAR pExtra, UINT32 extraValue)
{
    MachineDisableInterrupts();

    // Gather information
    REGISTERS regs;

    if (pFrame)
    {
        regs.EIP = pFrame->Ip;
    }
    else
    {
        regs.EIP = 0;
    }

    asm volatile(
        "mov %%eax, %0;" : "=a"(regs.EAX)
    );

    asm volatile(
        "mov %%ebx, %0;" : "=b"(regs.EBX)
    );

    asm volatile(
        "mov %%ecx, %0;" : "=c"(regs.ECX)
    );

    asm volatile(
        "mov %%edx, %0;" : "=d"(regs.EDX)
    );

    asm volatile(
        "mov %%esi, %0;" : "=S"(regs.ESI)
    );

    asm volatile(
        "mov %%edi, %0;" : "=D"(regs.EDI)
    );

    asm volatile(
        "mov %%eax, %0;" : "=a"(regs.EAX)
    );

    asm volatile(
        "movl %%cs, %0;" : "=a"(regs.CS)
    );

    asm volatile(
        "movl %%ds, %0;" : "=a"(regs.DS)
    );

    asm volatile(
        "movl %%es, %0;" : "=a"(regs.ES)
    );

    asm volatile(
        "movl %%fs, %0;" : "=a"(regs.FS)
    );

    asm volatile(
        "movl %%gs, %0;" : "=a"(regs.GS)
    );

    // //if (!IsWindowingInitialized)
    // //if (IsWindowingInitialized == FALSE)
    // //if (IsWindowingInitialized == 0)
    // {
    //     //ERROR(pExceptionType);
    //     TextBugCheck(pFrame, pExceptionType, pFile, pFunction, lineNumber, &regs);
    //     //MemoryDisplayFreeList();
    // 	//MemoryDisplayUsedList();

    //     //HALT;
    // }

    // Display
    HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hDesktopWindow);
    RECTANGLE clientRectangle;
    GetClientRectangle(hDesktopWindow, &clientRectangle);
    UINT32 textHeight = GetTextHeight(hDeviceContext, (PCHAR) &"");

    SetBackgroundColor(hDeviceContext, 0xFF000080);
    SetTextColor(hDeviceContext, 0xFFFFFFFF);
    DrawFilledRectangleC(hDeviceContext, &clientRectangle);

    clientRectangle.Left += 30;
    clientRectangle.Right -= 30;
    UINT32 textFlags = DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_BOLD;

    SetBackgroundColor(hDeviceContext, 0xFFC0C0C0);
    SetTextColor(hDeviceContext, 0xFF000000);

    clientRectangle.Top += 50;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    clientRectangle.Left += 100;
    clientRectangle.Right -= 100;

    // SYSTEM NAME
    DrawFilledRectangleC(hDeviceContext, &clientRectangle);
    DrawText(hDeviceContext, (PCHAR) &BcTextSystemName, &clientRectangle, textFlags);
    clientRectangle.Left -= 70;
    clientRectangle.Right += 70;

    SetBackgroundColor(hDeviceContext, 0xFF000080);
    SetTextColor(hDeviceContext, 0xFFFFFFFF);

    // EXCEPTION
    clientRectangle.Top += 30;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, (PCHAR) &BcTextException, &clientRectangle, textFlags);

    // EXCEPTION DETAILS
    textFlags &= ~DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER;
    clientRectangle.Top += 60;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, (PCHAR) &BcTextExceptionDetails, &clientRectangle, textFlags);

    // EXCEPTION TYPE
    clientRectangle.Top += 30;
    clientRectangle.Left += 30;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;

    if (pExceptionType)
    {
        DrawText(hDeviceContext, (PCHAR) pExceptionType, &clientRectangle, textFlags);   
    }    
    
    // LOCATION
    clientRectangle.Left -= 30;
    clientRectangle.Top += 20;

    if (pFile)
    {
        clientRectangle.Top += 20;
        clientRectangle.Bottom = clientRectangle.Top + textHeight;
        DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "FILE:") , &clientRectangle, textFlags);   

        clientRectangle.Left += 100;
        DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "%s", pFile) , &clientRectangle, textFlags);   
        clientRectangle.Left -= 100;
    }

    if (pFunction)
    {
        clientRectangle.Top += 20;
        clientRectangle.Bottom = clientRectangle.Top + textHeight;
        DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "FUNCTION:") , &clientRectangle, textFlags);   

        clientRectangle.Left += 100;
        DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "%s", pFunction) , &clientRectangle, textFlags);   
        clientRectangle.Left -= 100;
    }

    if (lineNumber)
    {
        clientRectangle.Top += 20;
        clientRectangle.Bottom = clientRectangle.Top + textHeight;
        DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "LINE:", lineNumber) , &clientRectangle, textFlags);   

        clientRectangle.Left += 100;
        DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "%u", lineNumber) , &clientRectangle, textFlags);   
        clientRectangle.Left -= 100;
    }

    if (pExtra)
    {
        clientRectangle.Top += 20;
        clientRectangle.Bottom = clientRectangle.Top + textHeight;
        DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "%s", pExtra) , &clientRectangle, textFlags);   

        clientRectangle.Left += 100;
        DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "%u (%x)", extraValue, extraValue) , &clientRectangle, textFlags);   
        clientRectangle.Left -= 100;
    }

    // REGISTERS
    clientRectangle.Top += 30;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, (PCHAR) &BcTextRegisters, &clientRectangle, textFlags);   

    // REGISTERS COLUMN 1
    clientRectangle.Left += 100;
    clientRectangle.Top += 30;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "EIP = %x", regs.EIP) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "CS = %x", regs.CS) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "DS = %x", regs.DS) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "ES = %x", regs.ES) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "FS = %x", regs.FS) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "GS = %x", regs.GS) , &clientRectangle, textFlags);   

    // REGISTERS COLUMN 2
    clientRectangle.Left += 150;
    clientRectangle.Top -= 100;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "EAX = %x", regs.EAX) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "EBX = %x", regs.EBX) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "ECX = %x", regs.ECX) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "EDX = %x", regs.EDX) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "ESI = %x", regs.ESI) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Bottom = clientRectangle.Top + textHeight;
    DrawText(hDeviceContext, GetFormattedText((PCHAR) &buffer[0], "EDI = %x", regs.EDI) , &clientRectangle, textFlags);   

    clientRectangle.Top += 20;
    clientRectangle.Left -= 180;


    HALT;
}

VOID TextBugCheck(PINTERRUPTFRAME pFrame, PCHAR pExceptionType, PCHAR pFile, PCHAR pFunction, UINT32 lineNumber, PREGISTERS pRegs)
{
    for (UINT32 y = 0; y < 25; y++)
    {
        UiDrawTextEx(0, y, 0x17, "                                                                                ");
    }

    UiDrawTextEx(25, 2, COLOR_DO_NOT_CARE, (PCHAR) &BcTextSystemName);
    UiDrawTextEx(5, 4, COLOR_DO_NOT_CARE, (PCHAR) &BcTextException);
    UiDrawTextEx(5, 6, COLOR_DO_NOT_CARE, (PCHAR) &BcTextExceptionDetails);
    UiDrawTextEx(10, 8, COLOR_DO_NOT_CARE, (PCHAR) pExceptionType);
	
    
    if (pFile)
    {
        UiDrawTextEx(10, 10, COLOR_DO_NOT_CARE, GetFormattedText((PCHAR) &buffer[0], "FILE:"));   
        UiDrawTextEx(20, 10, COLOR_DO_NOT_CARE, GetFormattedText((PCHAR) &buffer[0], "%s", pFile));   
    }

    if (pFunction)
    {
        UiDrawTextEx(10, 12, COLOR_DO_NOT_CARE, GetFormattedText((PCHAR) &buffer[0], "FUNCTION:"));   
        UiDrawTextEx(20, 12, COLOR_DO_NOT_CARE, GetFormattedText((PCHAR) &buffer[0], "%s", pFunction));   
    }

    if (lineNumber)
    {
        UiDrawTextEx(10, 14, COLOR_DO_NOT_CARE, GetFormattedText((PCHAR) &buffer[0], "LINE:"));   
        UiDrawTextEx(20, 14, COLOR_DO_NOT_CARE, GetFormattedText((PCHAR) &buffer[0], "%d", lineNumber));   
    }

    HALT;
}

VOID GraphicalBugCheck(PINTERRUPTFRAME pFrame, PCHAR pExceptionType, PCHAR pFile, PCHAR pFunction, UINT32 lineNumber, PREGISTERS pRegs)
{

}
