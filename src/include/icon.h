#ifndef __ICON_H
#define __ICON_H

#define ICONDIRECTORY_SIZE          6
#define ICONENTRY_SIZE              16

//typedef UINT32 PICON;

typedef struct _ICONDIRECTORY
{
    UINT16 Reserved;		        // Always 0.
    UINT16 ImageType;		        // 1 for Icon, 2 for Cursor, other values are invalid.
    UINT16 ImageCount;		        // Numer of images in the file.
} __attribute__((packed)) ICONDIRECTORY, *PICONDIRECTORY;

typedef struct _ICONDIRECTORYENTRY
{
    UINT8  Width;			        // Width in pixels. 0 means 256 pixels.
    UINT8  Height;			        // Height in pixels. 0 means 256 pixels.
    UINT8  NumberOfColors;  	    // Number of colors in the color palette. 0 if does not use palette.
    UINT8  Reserved;		        // Always 0.
    UINT16 NumberOfColorPlanes;	    // Specifies the color planes. Should be 0 or 1.
    UINT16 NumberOfBitsPerPixel;	// Specifies bits per pixel.
    UINT32 Length;			        // Specifies the image data in bytes.
    UINT32 Offset;			        // Specifies the offset of BMP or PNG data from the beginning of the ICO file
} __attribute__((packed)) ICONDIRECTORYENTRY, *PICONDIRECTORYENTRY;

typedef struct _ICON
{
    UINT32 Width;
    UINT32 Height;
    UINT16 NumberOfColorPlanes;
    UINT16 NumberOfBitsPerPixel;
    HBITMAP hBitmapAndMask;
    HBITMAP hBitmapXorMask;
} __attribute__((packed)) ICON, *PICON;

PVOID pLogoIcon;
PVOID pPenIcon;
PVOID pDesktopIcon;
PVOID pProgramsIcon;
PVOID pFloppyIcon;
PVOID pArrowCursor;

VOID LogoIconInit();

#endif /* __ICON_H */