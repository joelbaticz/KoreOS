/*
  - Date: 2018-03-10
  - Author: Joel
  - Purpose: PC Input/Output implementation:
			 Machine specific video/keyboard functions.
*/

/* INCLUDES */
#include <kldr.h>

/* PRIVATE DEFINITIONS */
#define TEXT_CURSOR_SHAPE_ROW_START					0x06
#define TEXT_CURSOR_SHAPE_ROW_END					0x07

/* PRIVATE VARIABLES */
UINT8 TextCursorShapeRowStart = TEXT_CURSOR_SHAPE_ROW_START;
UINT8 TextCursorShapeRowEnd = TEXT_CURSOR_SHAPE_ROW_END;

/* PRIVATE FUNCTIONS */
PCHAR MyConvert(PCHAR dest, unsigned int num);

BOOL PcConsoleSetCursorShape(UINT8 rowStart, UINT8 rowEnd)
{
	/*
		INT 10h - Video Bios Services.
		
		On entry:
			AH = 01h set text-mode cursor shape
			CH = scan row start
			CL = scan row end
		On return:
			Nothing
	*/	
	
	REGISTERS regs;
	PREGISTERS pRegs = &regs;
		
	regs.EAX = (0x01) << 8;
	regs.EBX = 0;
	regs.ECX = (rowStart) << 8 | rowEnd;
	regs.EDX = 0;
	regs.ESI = 0;
	regs.EDI = 0;
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = 0;
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
	
	PcBiosCallInterrupt(0x10, pRegs, pRegs);	
}

VOID PcConsoleSetBiosVideoMode(UINT8 requestedBiosVideoMode)
{
	/*
		INT 10h - Video Bios Services.
		
		On entry:
			AH = 00h set video mode
			AL = video mode
		On return:
			AL = video mode flag / CRT controller mode byte
	*/	
	
	REGISTERS regs;
	PREGISTERS pRegs = &regs;
		
	// AH = 0x00 - Set Video Mode Function, AL = VideoModeNumber
	regs.EAX = (0x00) << 8 | requestedBiosVideoMode;
	//regs.EAX = requestedBiosVideoMode;
	regs.EBX = 0;
	regs.ECX = 0;
	regs.EDX = 0;
	regs.ESI = 0;
	regs.EDI = 0;
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = 0;
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
	
	PcBiosCallInterrupt(0x10, pRegs, pRegs);
}

/* GLOBAL FUNCTIONS */
VOID PcConsolePutChar(CHAR ch)
{	
	//TRACE("PcIoPutChar() called.");
	
	/*
		INT 10h - Video Bios Services.
		
		On entry:
			AH = 0ah write character only at cursor position (no attribute byte)
			AL = character
			BH = page number
			CX = number of times to print character
		On return:
			Nothing
	*/	
	
	REGISTERS regs;
	PREGISTERS pRegs = &regs;
		
	regs.EAX = (0x0a) << 8 | ch;
	regs.EBX = 0;
	regs.ECX = 1;
	regs.EDX = 0;
	regs.ESI = 0;
	regs.EDI = 0;
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = 0;
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
		
	PcBiosCallInterrupt(0x10, pRegs, pRegs);
	
	//TRACE("RETURNED!!!");
}

VOID PcConsoleWaitForKeyPress()
{
	
}

CHAR PcConsoleGetChar()
{
	
}

UINT8 PcConsoleDetectBestDisplayMode()
{
	UINT8 detectedVideoMode = UI_TEXT;

	return detectedVideoMode;
}

VOID PcConsoleEnableBlinking(BOOL enableBlinking)
{
	REGISTERS regs;
	PREGISTERS pRegs = &regs;
		
	regs.EAX = (0x10) << 8 | 0x03;
	if (enableBlinking)
		regs.EBX = 1;
	else
		regs.EBX = 0;

	regs.ECX = 0;
	regs.EDX = 0;
	regs.ESI = 0;
	regs.EDI = 0;
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = 0;
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
		
	PcBiosCallInterrupt(0x10, pRegs, pRegs);
}

VOID PcConsoleSetPaletteColor(UINT8 colorToChange, UINT8 red, UINT8 green, UINT8 blue)
{
	REGISTERS regs;
	PREGISTERS pRegs = &regs;
		
	regs.EAX = (0x10) << 8 | 0x10;
	regs.EBX = colorToChange;
	regs.ECX = green << 4 | blue;
	regs.EDX = red << 4;
	regs.ESI = 0;
	regs.EDI = 0;
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = 0;
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
		
	PcBiosCallInterrupt(0x10, pRegs, pRegs);
}

VOID PcConsoleSetDisplayMode(UINT8 requestedDisplayMode)
{
	if (requestedDisplayMode == UI_TEXT)
	{
		PcConsoleSetBiosVideoMode(0x03);
		UiScreenWidth = 80;
		UiScreenHeight = 25;
		UiScreenBytesPerScanLine = 160;
		UiScreenBufferSize = UiScreenBytesPerScanLine * UiScreenHeight;
		PcConsoleEnableBlinking(FALSE);
		//PcConsoleSetPaletteColor(15, 63, 63, 0);
		PcConsoleShowCursor(FALSE);
		PcConsoleSetCursorPosition(0, 0);
	}
	else if (requestedDisplayMode == UI_GRAPHICS)
	{
		NOT_IMPLEMENTED;
		HALT;
	}	
	else if (requestedDisplayMode == UI_VESA_GRAPHICS)
	{
		NOT_IMPLEMENTED;
		HALT;
	}	
}	

BOOL PcConsoleShowCursor(BOOL doShowCursor)
{
	/* Use SetCursorShape to Show or Hide the cursor */
	if (doShowCursor)
		PcConsoleSetCursorShape(TextCursorShapeRowStart, TextCursorShapeRowEnd);
	else
		PcConsoleSetCursorShape(TextCursorShapeRowEnd, TextCursorShapeRowStart);
}


VOID PcConsoleGetCursorPosition(OUT PUINT8 pX, OUT PUINT8 pY)
{
	/*
		INT 10h - Video Bios Services.
		
		On entry:
			AH = 03h get cursor position and shape
			BH = video page number
		On return:
			CH = start scan line (for cursor shape)
			CL = end scan line (for cursor shape)
			DH = row (Y)
			DL = column (X)
	*/	
	
	
	REGISTERS regs;
	PREGISTERS pRegs = &regs;
		
	regs.EAX = (0x03) << 8;
	regs.EBX = 0;
	regs.ECX = 0;
	regs.EDX = 0;
	regs.ESI = 0;
	regs.EDI = 0;
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = 0;
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
	
	PcBiosCallInterrupt(0x10, pRegs, pRegs);	

	*pX = regs.EDX & 0xFF;
	*pY = regs.EDX >> 8 & 0xFF;

	CHAR text[80] = {0};

	//  PcConsoleSetCursorPosition(0, 0);
	//  DebugPrintF("---------------");

	// PcConsoleSetCursorPosition(0, 1);
	// DebugPrintF("EAX = ");
	// MyConvert(&text[0], regs.EAX);
	// DebugPrintF(&text[0]);

	// PcConsoleSetCursorPosition(0, 2);
	// DebugPrintF("EBX = ");
	// MyConvert(&text[0], regs.EBX);
	// DebugPrintF(&text[0]);

	// PcConsoleSetCursorPosition(0, 3);
	// DebugPrintF("ECX = ");
	// MyConvert(&text[0], regs.ECX);
	// DebugPrintF(&text[0]);

	// PcConsoleSetCursorPosition(0, 4);
	// DebugPrintF("EDX = ");
	// MyConvert(&text[0], regs.EDX);
	// DebugPrintF(&text[0]);

	// UINT8 xxx = regs.EDX & 0xFF;
	// UINT8 yyy = regs.EDX >> 8 & 0xFF;

	// UINT8 x = *pX;
	// UINT8 y = *pY;

	// DebugPrintF("EDX = ");
	// MyConvert(&text[0], (unsigned int) regs.EDX);
	// DebugPrintF(&text[0]);

	// DebugPrintF("PX = ");
	// MyConvert(&text[0], (unsigned int) x);
	// DebugPrintF(&text[0]);

	// DebugPrintF("PY = ");
	// MyConvert(&text[0], (unsigned int) y);
	// DebugPrintF(&text[0]);


// HALT;

	//*pY++;
	//*pY = &pY ++;
}

VOID PcConsoleSetCursorPosition(UINT8 x, UINT8 y)
{
	/*
		INT 10h - Video Bios Services.
		
		On entry:
			AH = 02h set cursor position
			BH = video page number
			DH = row
			DL = column
		On return:
			Nothing
	*/	
	
	REGISTERS regs;
	PREGISTERS pRegs = &regs;
		
	regs.EAX = (0x02) << 8;
	regs.EBX = (0x00) << 8;
	regs.ECX = 0;
	regs.EDX = (y) << 8 | x;
	regs.ESI = 0;
	regs.EDI = 0;
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = 0;
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
	
	PcBiosCallInterrupt(0x10, pRegs, pRegs);	
}



PCHAR MyConvert(PCHAR dest, unsigned int num)
{
	CHAR temp[20] = {0};
	PCHAR pTemp = &temp[0];
	PCHAR pText = dest;
		
	uitoa(num, pTemp, 16);
	strcpy(pText, pTemp);
	pText += strlen(pTemp);
	*pText++ = ' ';
	*pText++ = '(';
	uitoa(num, pTemp, 10);
	strcpy(pText, pTemp);
	pText += strlen(pTemp);
	*pText++ = ')';
	*pText++ = '\0';
	
	return dest;
}