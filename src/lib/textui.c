/*
  - Date: 2018-03-14
  - Author: Joel
  - Purpose: UI implementation:
			 Higher level UI function implementation for Text UI interface.
*/

#include <kldr.h>

#define TEXT_VIDEO_BUFFER_ADDRESS						0xb8000

UINT32 TextUiCursorX;
UINT32 TextUiCursorY;

VOID TextUiScrollUp(UINT32 numberOfLines);

VOID TextUiInit(BOOL fullInit)
{
	if (!fullInit)
	{
		UiScreenWidth = 80;
		UiScreenHeight = 25;
		UiScreenBytesPerScanLine = 160;
		UiScreenBufferSize = 80 * 25 * 2;

		UiVtbl.UiUninit = (PVOID) TextUiUninit;

		UiVtbl.UiPutChar = (PVOID) TextUiPutChar;
		UiVtbl.UiPutCharEx = (PVOID) TextUiPutCharEx;
		UiVtbl.UiGetCursorPosition = (PVOID) TextUiGetCursorPosition;
		UiVtbl.UiSetCursorPosition = (PVOID) TextUiSetCursorPosition;

		UiVtbl.UiDrawText = (PVOID) TextUiDrawText;
		UiVtbl.UiDrawTextEx = (PVOID) TextUiDrawTextEx;
		UiVtbl.UiFillArea = (PVOID) TextUiFillArea;
		UiVtbl.UiDrawFrame = (PVOID) TextUiDrawFrame;
	
		UiVtbl.UiDrawWelcomeScreen = (PVOID) TextUiDrawWelcomeScreen;
	
		UiVtbl.UiDrawBox = (PVOID) TextUiDrawBox;
		UiVtbl.UiDrawMessageBox = (PVOID) TextUiDrawMessageBox;
		UiVtbl.UiDrawInfoBox = (PVOID) TextUiDrawInfoBox;
		UiVtbl.UiDrawCriticalMessageBox = (PVOID) TextUiDrawCriticalMessageBox;

		UiVtbl.UiDrawStatusBar = (PVOID) TextUiDrawStatusBar;
		UiVtbl.UiDrawButton = (PVOID) TextUiDrawButton;
	}
	else
	{
		MachineConsoleSetDisplayMode(UI_TEXT);
		MachineConsoleShowCursor(FALSE);
	}
	
	TextUiCursorX = 0;
	TextUiCursorY = 0;

	TextUiFillArea(0, 0, UiScreenWidth, UiScreenHeight, COLOR_BLACK << 4 | COLOR_LIGHT_GRAY, ' ');
}

VOID TextUiUninit()
{
	MachineConsoleSetDisplayMode(UI_TEXT);
	MachineConsoleShowCursor(TRUE);
}

VOID TextUiPutChar(CHAR ch)
{
	PCHAR pVideo = (PCHAR) TEXT_VIDEO_BUFFER_ADDRESS;
	
	pVideo += (TextUiCursorY * UiScreenWidth + TextUiCursorX) * 2;
	
	*pVideo = ch;
	
	TextUiCursorX++;

	if (TextUiCursorX > UiScreenWidth)
	{
		TextUiCursorY++;
		TextUiCursorX = 0;
	}	
}

VOID TextUiPutCharEx(UINT32 x, UINT32 y, UINT8 colorAttribute, CHAR ch)
{
	PCHAR pVideo = (PCHAR) TEXT_VIDEO_BUFFER_ADDRESS;
	
	UINT32 CursorX = x;
	UINT32 CursorY = y;
	
	pVideo += (CursorY * UiScreenWidth + CursorX) * 2;
	
	*pVideo = ch;
	
	if (colorAttribute != COLOR_DO_NOT_CARE)
	{
		pVideo++;
		*pVideo = colorAttribute;
	}
}

/* Standard drawing functions */
VOID TextUiDrawText(PCSTR pText)
{

	//return;

	while(*pText!='\0')
	{
		TextUiPutChar(*pText);		
		pText++;
	}

	TextUiCursorY++;
	TextUiCursorX = 0;

	if (TextUiCursorY >= UiScreenHeight)
	{
		TextUiScrollUp(1);
	}
}

VOID TextUiDrawTextEx(UINT32 x, UINT32 y, UINT8 colorAttribute, PCSTR pText)
{

	//return;

	while(*pText!='\0')
	{
		TextUiPutCharEx(x, y, colorAttribute, *pText);		
		pText++;
		x++;
	}
}

VOID TextUiFillArea(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, CHAR fillChar)
{
	
	//return;

	for(size_t y = top; y <= bottom; y++)
	{
		for(size_t x = left; x <= right; x++)
		{
			TextUiPutCharEx(x, y, colorAttribute, fillChar);		
		}
	}	
}

VOID TextUiDrawFrame(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, UINT8 lineStyle)
{
	//TRACE("TextUiDrawFrame() called.");
	
	if (lineStyle == LINE_STYLE_SINGLE)
	{
		TextUiPutCharEx(left, top, colorAttribute, CORNER_UPPER_LEFT);
		TextUiPutCharEx(right, top, colorAttribute, CORNER_UPPER_RIGHT);
		TextUiPutCharEx(left, bottom, colorAttribute, CORNER_LOWER_LEFT);
		TextUiPutCharEx(right, bottom, colorAttribute, CORNER_LOWER_RIGHT);	
		TextUiFillArea(left + 1, top, right - 1, top, colorAttribute, LINE_HORIZONTAL);
		TextUiFillArea(left + 1, bottom, right - 1, bottom, colorAttribute, LINE_HORIZONTAL);
		TextUiFillArea(left, top + 1, left, bottom - 1, colorAttribute, LINE_VERTICAL);
		TextUiFillArea(right, top + 1, right, bottom - 1, colorAttribute, LINE_VERTICAL);
	}
	else if (lineStyle == LINE_STYLE_DOUBLE)
	{
		TextUiPutCharEx(left, top, colorAttribute, CORNER_UPPER_LEFT_DOUBLE);
		TextUiPutCharEx(right, top, colorAttribute, CORNER_UPPER_RIGHT_DOUBLE);
		TextUiPutCharEx(left, bottom, colorAttribute, CORNER_LOWER_LEFT_DOUBLE);
		TextUiPutCharEx(right, bottom, colorAttribute, CORNER_LOWER_RIGHT_DOUBLE);	
		TextUiFillArea(left + 1, top, right - 1, top, colorAttribute, LINE_HORIZONTAL_DOUBLE);
		TextUiFillArea(left + 1, bottom, right - 1, bottom, colorAttribute, LINE_HORIZONTAL_DOUBLE);
		TextUiFillArea(left, top + 1, left, bottom - 1, colorAttribute, LINE_VERTICAL_DOUBLE);
		TextUiFillArea(right, top + 1, right, bottom - 1, colorAttribute, LINE_VERTICAL_DOUBLE);
	} 
}

VOID TextUiDrawShadow(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom)
{
	INT32 x;

	//x = right + 1;
	//y = top + 1;
	
	TextUiFillArea(right + 1, top + 1, right + 1, bottom + 1, 0x07, ' ');
	TextUiFillArea(left + 1, bottom + 1, right + 1, bottom + 1, 0x07, ' ');
	
}

/* Complex drawing functions - require other standard drawing functions */
VOID TextUiDrawWelcomeScreen()
{
	/* Clear screen */
	TextUiFillArea(0, 0, UiScreenWidth - 1, UiScreenHeight - 1, COLOR_BLACK << 4 | COLOR_LIGHT_GRAY, ' ');

	/* Welcome Bar */
	TextUiFillArea(0, 0, UiScreenWidth - 1, 0, COLOR_LIGHT_GRAY << 4 | COLOR_BLACK, ' ');
	
	/* Draw welcome text */
	TextUiDrawTextEx(1, 0, COLOR_DO_NOT_CARE, "Kore OS Starting up...");//UiTextBackgroundColor << 4 | UiTextForegroundColor);

	TextUiCursorX = 0;
	TextUiCursorY = 1;
}

VOID TextUiDrawBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, UINT8 lineStyle, PCSTR pCaption)
{
	UINT32 textPosition;
	
	TextUiFillArea(left, top, right, bottom, colorAttribute, ' ');
	TextUiDrawFrame(left, top, right, bottom, colorAttribute, lineStyle);
	TextUiDrawShadow(left, top, right, bottom);
	
	textPosition = (left + right - strlen(pCaption)) / 2 + 1;	
	TextUiDrawTextEx(textPosition, top, colorAttribute, pCaption);	
}

VOID TextUiDrawMessageBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption)
{
	TextUiDrawBox(left, top, right, bottom, ColorMessageBox, LINE_STYLE_DOUBLE, pCaption);
}

VOID TextUiDrawInfoBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption)
{
	TextUiDrawBox(left, top, right, bottom, ColorInfoBox, LINE_STYLE_SINGLE, pCaption);
}

VOID TextUiDrawCriticalMessageBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption)
{
	TextUiDrawBox(left, top, right, bottom, ColorCriticalMessageBox, LINE_STYLE_DOUBLE, pCaption);
}

VOID TextUiDrawStatusBar(PCSTR pStatusText, PCSTR pAdditionalStatusText)
{
	UINT32 textPosition;
	
	TextUiFillArea(0, UiScreenHeight - 1, UiScreenWidth - 1, UiScreenHeight - 1, ColorStatusBar, ' ');
	TextUiDrawTextEx(0, UiScreenHeight - 1, COLOR_DO_NOT_CARE, pStatusText);
	
	textPosition = UiScreenWidth - strlen(pAdditionalStatusText) - 3;
	//TextUiDrawTextEx(textPosition, UiScreenHeight - 1, COLOR_DO_NOT_CARE, " ");
	TextUiPutCharEx(textPosition, UiScreenHeight - 1, COLOR_DO_NOT_CARE, LINE_VERTICAL);
	TextUiDrawTextEx(textPosition + 2, UiScreenHeight - 1, COLOR_DO_NOT_CARE, pAdditionalStatusText);
	
}

VOID TextUiDrawButton(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption, BOOL isEnabled, BOOL isDefault, BOOL isSelected, BOOL isPressed)
{
	UINT32 textPositionX;
	UINT32 textPositionY;
	UINT8 buttonColor;
	
	if (isEnabled)
		buttonColor = ColorButtonNormal;
	else	
		buttonColor = ColorButtonDisabled;
	
	if (isEnabled)
	{
		if (isSelected)
			buttonColor = ColorButtonSelected;
		if (isPressed)
			buttonColor = ColorButtonPressed;		
	}

	TextUiFillArea(left, top, right, bottom, buttonColor, ' ');
	
	if (isDefault)
	{
		TextUiPutCharEx(left, textPositionY, COLOR_DO_NOT_CARE, '<');
		TextUiPutCharEx(right, textPositionY, COLOR_DO_NOT_CARE, '>');
	}
	
	textPositionX = (left + right - strlen(pCaption)) / 2;
	textPositionY = (top + bottom) / 2;
	TextUiDrawTextEx(textPositionX, textPositionY, COLOR_DO_NOT_CARE, pCaption);
}

VOID TextUiGetCursorPosition(OUT PUINT8 pX, OUT PUINT8 pY)
{
	*pX = TextUiCursorX;
	*pY = TextUiCursorY;
}

VOID TextUiSetCursorPosition(UINT8 x, UINT8 y)
{
	TextUiCursorX = x;
	TextUiCursorY = y;
}

VOID TextUiScrollUp(UINT32 numberOfLines)
{
	PCHAR pVideo = (PCHAR) TEXT_VIDEO_BUFFER_ADDRESS;
	PCHAR pFrom;
	PCHAR pTo;

	for(UINT32 fromY = 1 + numberOfLines; fromY < UiScreenHeight; fromY++)
	{
		UINT32 toY = fromY - numberOfLines;

		pFrom = pVideo + (2 * UiScreenWidth) * fromY;
		pTo = pVideo + (2 * UiScreenWidth) * toY;

		for(UINT32 x = 0; x < UiScreenWidth * 2; x++)
		{
			pTo[x] = pFrom[x];
		}
	}

	// Clear last line
	for(UINT32 x = 0; x < UiScreenWidth * 2; x += 2)
	{
		pFrom[x] = ' ';
	}

	// Adjust cursor

	TextUiCursorY -= numberOfLines;
}