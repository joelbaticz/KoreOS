/*
  - Date: 2018-03-14
  - Author: Joel
  - Purpose: UI init definitions
*/

#ifndef __UI_H
#define __UI_H

/* GLOBAL DEFINITIONS */
#define UI_TEXT											1
#define UI_GRAPHICS										2
#define UI_VESA_GRAPHICS								3

#define COLOR_BLACK										0
#define COLOR_BLUE										1
#define COLOR_GREEN										2
#define COLOR_CYAN										3
#define COLOR_RED										4
#define COLOR_MAGENTA									5
#define COLOR_BROWN										6
#define COLOR_LIGHT_GRAY								7
#define COLOR_GRAY										8
#define COLOR_LIGHT_BLUE								9
#define COLOR_LIGHT_GREEN								10
#define COLOR_LIGHT_CYAN								11
#define COLOR_LIGHT_RED									12
#define COLOR_LIGHT_MAGENTA								13
#define COLOR_YELLOW									14
#define COLOR_WHITE										15

#define COLOR_DO_NOT_CARE								255

#define GRAPHICS_COLOR_DARKBLUE							0x000080

typedef struct _UI_VTBL
{
	VOID (*UiUninit)();

	VOID (*UiPutChar)(CHAR ch);
	VOID (*UiPutCharEx)(UINT32 x, UINT32 y, UINT8 colorAttribute, CHAR ch);
	VOID (*UiGetCursorPosition)(OUT PUINT8 pX, OUT PUINT8 pY);
	VOID (*UiSetCursorPosition)(UINT8 x, UINT8 y);

	VOID (*UiDrawText)(PCSTR pText);
	VOID (*UiDrawTextCenter)(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, PCSTR pText);
	VOID (*UiDrawTextEx)(UINT32 x, UINT32 y, UINT8 colorAttribute, PCSTR pText);
	VOID (*UiFillArea)(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, CHAR fillChar);
	VOID (*UiDrawFrame)(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, UINT8 lineStyle);

	VOID (*UiDrawWelcomeScreen)();

	VOID (*UiDrawBox)(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, UINT8 lineStyle, PCSTR pCaption);
	VOID (*UiDrawMessageBox)(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption);
	VOID (*UiDrawInfoBox)(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption);
	VOID (*UiDrawCriticalMessageBox)(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption);

	VOID (*UiDrawStatusBar)(PCSTR pStatusText, PCSTR pAdditionalStatusText);
	VOID (*UiDrawButton)(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption, BOOL isEnabled, BOOL isDefault, BOOL isSelected, BOOL isPressed);
} UI_VTBL, *PUI_VTBL;

/* GLOBAL VARIABLES */
UI_VTBL UiVtbl;

/* Defaults will be set by the UI init function */
UINT32 UiScreenWidth;
UINT32 UiScreenHeight;
UINT32 UiScreenBytesPerScanLine;
UINT32 UiScreenBufferSize;

// Default Colors
static UINT8 ColorBackground = COLOR_BLUE << 4 | COLOR_LIGHT_GRAY;
static UINT8 ColorStatusBar = COLOR_LIGHT_GRAY << 4 | COLOR_BLACK;

static UINT8 ColorMessageBox = COLOR_BLUE << 4 | COLOR_WHITE;
static UINT8 ColorInfoBox = COLOR_CYAN << 4 | COLOR_LIGHT_CYAN;
static UINT8 ColorCriticalMessageBox = COLOR_RED << 4 | COLOR_WHITE;

static UINT8 ColorButtonNormal = COLOR_LIGHT_GRAY << 4 | COLOR_BLACK;
static UINT8 ColorButtonSelected = COLOR_WHITE << 4 | COLOR_BLACK;
static UINT8 ColorButtonDisabled = COLOR_GRAY << 4 | COLOR_BLACK;
static UINT8 ColorButtonPressed = COLOR_LIGHT_GRAY << 4 | COLOR_LIGHT_BLUE;

static UINT8 ColorEditBox = COLOR_LIGHT_GRAY << 4 | COLOR_BLACK;

/* GLOBAL FUNCTIONS */
VOID UiInit(BOOL fullInit);

/*
	These functions will call the UI specific versions (No UI, Text UI, Graphical UI)
	which will be implemented in noui.c, tui.c, gui.c, files respectively
*/

VOID UiUninit();

/* Basic drawing functions */
VOID UiPutChar(CHAR ch);
VOID UiPutCharEx(UINT32 x, UINT32 y, UINT8 colorAttribute, CHAR ch);
VOID UiGetCursorPosition(OUT PUINT8 pX, OUT PUINT8 pY);
VOID UiSetCursorPosition(UINT8 x, UINT8 y);

/* Standard drawing functions - requires basic drawing functions */
VOID UiDrawText(PCSTR pText);
VOID UiDrawTextCenter(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, PCSTR pText);
VOID UiDrawTextEx(UINT32 x, UINT32 y, UINT8 colorAttribute, PCSTR pText);
VOID UiFillArea(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, CHAR fillChar);
VOID UiDrawFrame(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, UINT8 lineStyle);

/* Complex drawing functions - require other standard drawing functions */
VOID UiDrawWelcomeScreen();

VOID UiDrawBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, UINT8 lineStyle, PCSTR pCaption);
VOID UiDrawMessageBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption);
VOID UiDrawInfoBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption);
VOID UiDrawCriticalMessageBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption);

VOID UiDrawStatusBar(PCSTR pStatusText, PCSTR pAdditionalStatusText);
VOID UiDrawButton(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption, BOOL isEnabled, BOOL isDefault, BOOL isSelected, BOOL isPressed);


#endif /* __UI_H */
