/*
  - Date: 2018-03-14
  - Author: Joel
  - Purpose: Text UI definitions
*/

/* GLOBAL DEFINITIONS */
#define LINE_STYLE_SINGLE				1
#define LINE_STYLE_DOUBLE				2

#define CORNER_UPPER_LEFT				(0xda)
#define CORNER_UPPER_RIGHT				(0xbf)
#define CORNER_LOWER_LEFT				(0xc0)
#define CORNER_LOWER_RIGHT				(0xd9)

#define CORNER_UPPER_LEFT_DOUBLE		(0xc9)
#define CORNER_UPPER_RIGHT_DOUBLE		(0xbb)
#define CORNER_LOWER_LEFT_DOUBLE		(0xc8)
#define CORNER_LOWER_RIGHT_DOUBLE		(0xbc)

#define LINE_HORIZONTAL					(0xc4)
#define LINE_VERTICAL					(0xb3)

#define LINE_HORIZONTAL_DOUBLE			(0xcd)
#define LINE_VERTICAL_DOUBLE			(0xba)

//HORIZONTAL DOUBLE, VERTICAL SINGLE
#define HD_UL    (0xd5)
#define HD_UR    (0xb8)
#define HD_LL    (0xd4)
#define HD_LR    (0xbe)

//VERTICAL DOUBLE, HORIZ SINGLE
#define VD_UL    (0xd6)
#define VD_UR    (0xb7)
#define VD_LL    (0xd3)
#define VD_LR    (0xbd)

/* GLOBAL FUNCTIONS */
VOID TextUiInit(BOOL fullInit);
VOID TextUiUninit();

/* Basic drawing functions */
VOID TextUiPutChar(CHAR ch);
VOID TextUiPutCharEx(UINT32 x, UINT32 y, UINT8 colorAttribute, CHAR ch);
VOID TextUiGetCursorPosition(OUT PUINT8 pX, OUT PUINT8 pY);
VOID TextUiSetCursorPosition(UINT8 x, UINT8 y);

/* Standard drawing functions - requires basic drawing functions */
VOID TextUiDrawText(PCSTR pText);
VOID TextUiDrawTextEx(UINT32 x, UINT32 y, UINT8 colorAttribute, PCSTR pText);
VOID TextUiFillArea(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, CHAR fillChar);
VOID TextUiDrawFrame(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, UINT8 lineStyle);

/* Complex drawing functions - require other standard drawing functions */
VOID TextUiDrawWelcomeScreen();

VOID TextUiDrawBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, UINT8 colorAttribute, UINT8 lineStyle, PCSTR pCaption);
VOID TextUiDrawMessageBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption);
VOID TextUiDrawInfoBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption);
VOID TextUiDrawCriticalMessageBox(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption);

VOID TextUiDrawStatusBar(PCSTR pStatusText, PCSTR pAdditionalStatusText);
VOID TextUiDrawButton(UINT32 left, UINT32 top, UINT32 right, UINT32 bottom, PCSTR pCaption, BOOL isEnabled, BOOL isDefault, BOOL isSelected, BOOL isPressed);
