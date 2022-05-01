#ifndef __FONT_H
#define __FONT_H

typedef struct _FONT
{
    UINT16  dfVersion;              // @0x0000 - 0x0002
    UINT32  dfSize;                 // @0x0002 - 0x0000c00f
    CHAR    dfCopyright[60];        // @0x0006 - "(c) Copyright Microsoft Corp. 1987. All rights reserved"
    UINT16  dfType;                 // @0x0042 - 0x0000 - RasterType 
    UINT16  dfPoints;               // @0x0044 - 0x0008 - Point 8
    UINT16  dfVertRes;              // @0x0046 - 0x0060
    UINT16  dfHorizRes;             // @0x0048 - 0x0060
    UINT16  dfAscent;               // @0x004A - 0x000B
    UINT16  dfInternalLeading;      // @0x004C - 0x0002
    UINT16  dfExternalLeading;      // @0x004E - 0x0000
    UINT8   dfItalic;               // @0x0050 - 0x00
    UINT8   dfUnderline;            // @0x0051 - 0x00
    UINT8   dfStrikeOut;            // @0x0052 - 0x00
    UINT16  dfWeight;               // @0x0053 - 0x0190
    UINT8   dfCharSet;              // @0x0055 - 0x00
    UINT16  dfPixWidth;             // @0x0056 - 0x0000 
    UINT16  dfPixHeight;            // @0x0058 - 0x000d
    BYTE    dfPitchAndFamily;       // @0x005A - 0x21
    UINT16  dfAvgWidth;             // @0x005B - 0x0007
    UINT16  dfMaxWidth;             // @0x005D - 0x000b
    UINT8   dfFirstChar;            // @0x005F - 0x20
    UINT8   dfLastChar;             // @0x0060 - 0xFF
    UINT8   dfDefaultChar;          // @0x0061 - 0x60
    UINT8   dfBreakChar;            // @0x0062 - 0x00
    UINT16  dfWidthBytes;           // @0x0063 - 0x0096
    UINT32  dfDevice;               // @0x0065 - 0x00000000 // Pointer to Device name
    UINT32  dfFace;                 // @0x0069 - 0x000003FA // Point to Face name
    UINT32  dfReserved;             // @0x006D - 0x00000000
    //CHAR    szDeviceName[];         // @0x0071 - 0x00
    //CHAR    szFaceName[];           // @0x0072 - 0x04
} __attribute__((packed)) FONT, *PFONT;

#define DEFAULT_FONT_BUFFER_SIZE            4032
#define LETTER_DESCRIPTIONS_OFFSET          118
#define LETTER_DESCRIPTIONS_OFFSET_EXTRA    30

PCHAR pDefaultFont;

VOID DefaultFontInit();

#endif /* __FONT_H */