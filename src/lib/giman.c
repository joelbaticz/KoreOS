/*
  - Date: 2021-08-30
  - Author: Joel
  - Purpose: Graphical Interface Manager
*/

#include <kldr.h>

typedef struct _DRAWPIXEL_VTBL
{
    VOID    (*DeviceDrawPixel)(UINT32 deviceX, UINT32 deviceY, COLOR color);
} __attribute__((packed)) DRAWPIXEL_VTBL, *PDRAWPIXEL_VTBL;

#define NUMBER_OF_VIDEO_DEVICES   1
#define BLOCK_WIDTH               8 // 64
#define BLOCK_HEIGHT              8 // 64

#define DeviceDrawPixelOverwrite(deviceX, deviceY, color)						                VideoDeviceFunctionsVtbl.DeviceDrawPixelOverwrite(deviceX, deviceY, color)
#define DeviceDrawPixelAnd(deviceX, deviceY, color)						                        VideoDeviceFunctionsVtbl.DeviceDrawPixelAnd(deviceX, deviceY, color)
#define DeviceDrawPixelOr(deviceX, deviceY, color)						                        VideoDeviceFunctionsVtbl.DeviceDrawPixelOr(deviceX, deviceY, color)
#define DeviceDrawPixelXor(deviceX, deviceY, color)						                        VideoDeviceFunctionsVtbl.DeviceDrawPixelXor(deviceX, deviceY, color)

#define DeviceDrawPixel(deviceX, deviceY, color)						                        DrawPixelVtbl.DeviceDrawPixel(deviceX, deviceY, color)

#define DeviceDrawFilledRectangle(deviceLeft, deviceTop, deviceRight, deviceBottom, color)		VideoDeviceFunctionsVtbl.DeviceDrawFilledRectangle(deviceLeft, deviceTop, deviceRight, deviceBottom, color)

#define DeviceHideMouse()                                                                       VideoDeviceFunctionsVtbl.DeviceHideMouse()
#define DeviceShowMouse()                                                                       VideoDeviceFunctionsVtbl.DeviceShowMouse()
#define DeviceMoveMouse(pMousePosition)                                                         VideoDeviceFunctionsVtbl.DeviceMoveMouse(pMousePosition)
#define DeviceSetMousePosition(pMousePosition)                                                  VideoDeviceFunctionsVtbl.DeviceSetMousePosition(pMousePosition)
#define DeviceGetMousePosition(pMousePosition)                                                  VideoDeviceFunctionsVtbl.DeviceGetMousePosition(pMousePosition)
#define DeviceSetMouseCursor(hCursor)                                                           VideoDeviceFunctionsVtbl.DeviceSetMouseCursor(hCursor)
#define DeviceGetMouseCursor()                                                                  VideoDeviceFunctionsVtbl.DeviceGetMouseCursor()

// NEVER INITIALIZE VARIABLES HERE, INITIALIZE THEM IN THE INIT FUNCTION!

HANDLE                      hGiPrimaryVideoAdapter;
HANDLE                      hGiDeviceCapabilities;
HANDLE                      hGiVideoModeInformation;
HANDLE                      hGiFrameBuffer;
HANDLE                      hGiDirtyBuffer;
HFONT                       hGiDefaultFont;
HANDLE                      hGiSystemCaret;

PDEVICE_CAPABILITIES        pGiDeviceCapabilities;
PVIDEO_MODE_INFORMATION     pGiVideoModeInformation;
PDEVICE_CONTEXT             pGiDeviceContextListHead;
PVOID                       pGiDeviceFrameBuffer;
PPALETTE                    pGiDevicePalette;

BOOL                        GiIsDeviceInitialized;
UINT32                      GiSupportedVideoModesCount;
UINT32                      GiCurrentVideoModeIndex;
UINT32                      GiHorizontalBlockCount;
UINT32                      GiVerticalBlockCount;
UINT32                      GiDirtyBufferSize;
UINT32                      GiScreenWidth;
UINT32                      GiScreenHeight;
UINT32                      GiScreenNumberOfBytesPerPixel;
UINT32                      GiScreenWidthInBytes;
UINT32                      GiDeviceWidthInBytes;

UINT32                      GiRasterOperationMode = DRAW_MODE_NORMAL;

HICON                       hArrowCursor;

INT32                       GiMouseX;
INT32                       GiMouseY;
INT32                       GiMouseXOld;
INT32                       GiMouseYOld;
BOOL                        GiMouseIsLeftButtonDown;
BOOL                        GiMouseIsMiddleButtonDown;
BOOL                        GiMouseIsRightButtonDown;
BOOL                        GiMouseIsLeftButtonDownOld;
BOOL                        GiMouseIsMiddleButtonDownOld;
BOOL                        GiMouseIsRightButtonDownOld;
UINT32                      GiMouseBackground[32 * 32];
BOOL                        GiMouseShown;
BOOL                        GiMouseWasPreviouslyShown;
BOOL                        GiDisableMouseUpdate;
BOOL                        GiDisableUiUpdate;

VIDEODEVICEFUNCTIONS_VTBL VideoDeviceFunctionsVtbl;
DRAWPIXEL_VTBL DrawPixelVtbl;

// PRIVATE

VOID GiPrimaryDisplayDeviceInitialize();
VOID GiVideoInitialize();
VOID GiVideoReset();
VOID GiGetNumberOfVideoModes();
VOID GiGetDeviceCapabilities();
VOID GiGetAllVideoModeInformation();
VOID GiGetDeviceFrameBufferAddress();
VOID GiClearDeviceFrameBuffer();
VOID GiSetVideoMode(UINT32 requestedVideoMode);
VOID GiGetDeviceRectangle(HDEVICE_CONTEXT hDeviceContext, PRECTANGLE pRectangle);
VOID GiGetDeviceFunctions();
VOID GiSetDeviceDrawMode(DRAWMODE drawMode);
VOID GiSetDeviceMouseCursor(HCURSOR hCursor);

VOID GiClearPalette();
VOID GiPaletteGrayscaleInit();
VOID GiPalette555Init();
VOID GiPalette666Init();
VOID GiPalette676Init();
VOID GiPalette884Init();
VOID GiPalette488Init();
VOID GiPalette4416Init();
VOID GiPalette3328Init();
VOID GiPalette16ColorInit();
VOID GiPalette16ColorVGAInit();
VOID GiPalette16ColorGrayInit();
VOID GiPalette16ColorUniqueInit();
VOID GiPalette8ColorInit();
VOID GiPalette4ColorInit();
VOID GiPalette2ColorInit();
VOID GiCreatePalette(UINT32 numberOfEntries, OUT PPALETTE* pPalette);
VOID GiSetPaletteEntry(PPALETTE pPalette, UINT32 entryIndex, UINT8 red, UINT8 green, UINT8 blue);
VOID GiSetDevicePalette(PPALETTE pPalette);
VOID GiDefaultFontInitialize();

VOID GiCursorsInitialize();
VOID GiDrawMouseCursor(INT32 x, INT32 y, HICON hIcon);

VOID GiShowVariableNameAndValue(PCHAR pVariableName, UINT32 variableValue, UINT32 base);

VOID GiDrawFilledRectangleCore(PRECTANGLE pScreenRect, COLOR color);
VOID GiDrawFilledRectangle(PRECTANGLE pClippingRect, INT32 left, INT32 top, INT32 right, INT32 bottom, COLOR color);
VOID GiDrawLineCore(PRECTANGLE pClippingRect, INT32 x1, INT32 y1, INT32 x2, INT32 y2, COLOR color);

// --- NEW ---
// VOID GiDrawDevicePixel2Bit(UINT32 screenX, UINT32 screenY, COLOR color, DRAWMODE drawMode);
// VOID GiDrawDeviceFilledRectangle2bit(UINT32  left, UINT32 top, UINT32 right, UINT32 bottom, COLOR color);
// VOID GiDrawDevicePixel4Bit(UINT32 screenX, UINT32 screenY, COLOR color, DRAWMODE drawMode);
// VOID GiDrawDeviceFilledRectangle4bit(UINT32 left, UINT32 top, UINT32 right, UINT32  bottom, COLOR color);
//VOID GiDrawLine(PRECTANGLE pClippingRect, INT32 xStart, INT32 yStart, INT32 xEnd, INT32 yEnd, COLOR color);
VOID GiDrawPixelCore(INT32 x, INT32 y, COLOR color);

HBITMAP GiCreateCompatibleBitmap1bit(PBITMAPINFOHEADER pInfoHeader);
HBITMAP GiCreateCompatibleBitmap4bit(PBITMAPINFOHEADER pInfoHeader);
HBITMAP GiCreateCompatibleBitmap8bit(PBITMAPINFOHEADER pInfoHeader);
HBITMAP GiCreateCompatibleBitmap24bit(PBITMAPINFOHEADER pInfoHeader);

VOID DrawCaret(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y, UINT32 width, UINT32 height);

VOID GraphicalInterfaceManagerInit()
{
    hGiPrimaryVideoAdapter   = (HANDLE) NULL;
    hGiDeviceCapabilities    = (HANDLE) NULL;
    hGiVideoModeInformation  = (HANDLE) NULL;
    hGiDefaultFont           = (HFONT)  NULL;

    pGiDeviceCapabilities    = NULL;
    pGiVideoModeInformation  = NULL;
    pGiDeviceContextListHead = NULL;
    pGiDeviceFrameBuffer     = NULL;
    pGiDevicePalette         = NULL;

    GiCurrentVideoModeIndex  = 0xFFFF;
    GiMouseShown             = FALSE;

    hGiSystemCaret           = (HANDLE) NULL;

    hGiPrimaryVideoAdapter = FileOpen((PCHAR) &"\\VideoAdapter0", FILE_OPEN_READ | FILE_OPEN_SHARE_EXCLUSIVE);

    if (hGiPrimaryVideoAdapter == (HANDLE)NULL)
    {
      UiDrawText("GiManagerInit................FAIL");
      HALT;
    }

    GiPrimaryDisplayDeviceInitialize();

    UiDrawText("GiManagerInit................DONE");
}

VOID GraphicalInterfaceManagerUninit()
{
  MemoryUnlock(hGiVideoModeInformation);
  MemoryFree(hGiVideoModeInformation);
}

VOID GiPrimaryDisplayDeviceInitialize()
{
    GiVideoInitialize();
    GiVideoReset();
    GiGetNumberOfVideoModes();
    GiGetAllVideoModeInformation();

    GiGetDeviceFrameBufferAddress();
    GiGetDeviceCapabilities();

    GiDefaultFontInitialize();
    GiCursorsInitialize();

    UiDrawText("GiDisplayInitialize() done.");
}

VOID GiVideoInitialize()
{
    KSTATUS result = DeviceIoControl(hGiPrimaryVideoAdapter, IOCTL_VIDEO_INITIALIZE, NULL, 0, NULL, 0);

    if (result != STATUS_SUCCESS)
    {
      UiDrawText("Failed to initialize video device.");
      HALT;
    }
}

VOID GiVideoReset()
{
   KSTATUS result = DeviceIoControl(hGiPrimaryVideoAdapter, IOCTL_VIDEO_RESET_DEVICE, NULL, 0, NULL, 0);

    if (result != STATUS_SUCCESS)
    {
      UiDrawText("Failed to reset video device.");
      HALT;
    }
}

VOID GiGetNumberOfVideoModes()
{
    KSTATUS result = DeviceIoControl(hGiPrimaryVideoAdapter, IOCTL_VIDEO_GET_NUMBER_OF_MODES, NULL, 0, &GiSupportedVideoModesCount, sizeof(UINT32));

    if (result != STATUS_SUCCESS)
    {
      UiDrawText("Failed to get number of video modes.");
      HALT;
    }

    GiShowVariableNameAndValue("SupportedVideoModesCount", GiSupportedVideoModesCount, 10);
}

VOID GiGetAllVideoModeInformation()
{
    hGiVideoModeInformation = MemoryAllocate(sizeof(VIDEO_MODE_INFORMATION) * GiSupportedVideoModesCount, MEMORY_FLAG_ZERO_INIT, (PCHAR) &"VideoModeInfo");
    pGiVideoModeInformation = MemoryLock(hGiVideoModeInformation);

    PVIDEO_MODE_INFORMATION pVideoModeInformationNext = pGiVideoModeInformation;

    for(UINT32 modeNumber = 0; modeNumber < GiSupportedVideoModesCount; modeNumber++)
    {
      KSTATUS result = DeviceIoControl(hGiPrimaryVideoAdapter, IOCTL_VIDEO_GET_MODE_INFORMATION, &modeNumber, sizeof(UINT32), pVideoModeInformationNext, sizeof(VIDEO_MODE_INFORMATION));

      if (result != STATUS_SUCCESS)
      {
        UiDrawText("Failed to get video mode.");
        HALT;
      }
      else
      {
        GiShowVariableNameAndValue("VideoModeNumber", pVideoModeInformationNext->ModeNumber, 10);
        GiShowVariableNameAndValue("VideoModeWidth", pVideoModeInformationNext->Width, 10);
        GiShowVariableNameAndValue("VideoModeWHeight", pVideoModeInformationNext->Height, 10);
        GiShowVariableNameAndValue("VideoModeNumberOfColors", pVideoModeInformationNext->NumberOfColors, 10);
      }

      pVideoModeInformationNext++;
    }
}

VOID GiGetDeviceFrameBufferAddress()
{
    KSTATUS result = DeviceIoControl(hGiPrimaryVideoAdapter, IOCTL_VIDEO_GET_FRAMEBUFFER_ADDRESS, NULL, 0, &pGiDeviceFrameBuffer, sizeof(UINT32));

    if (result != STATUS_SUCCESS)
    {
      UiDrawText("Failed to get device frame buffer address.");
      //HALT;
    }

    GiShowVariableNameAndValue("FrameBufferAddress", (UINT32)pGiDeviceFrameBuffer, 16);
}

VOID GiGetDeviceCapabilities()
{
    hGiDeviceCapabilities = MemoryAllocate(sizeof(DEVICE_CAPABILITIES), MEMORY_FLAG_ZERO_INIT, (PCHAR) &"DeviceCapabilites");
    pGiDeviceCapabilities = MemoryLock(hGiDeviceCapabilities);

    pGiDeviceCapabilities->DeviceTechnology = DEVICE_TECHNOLOGY_RASTER_DISPLAY;
    pGiDeviceCapabilities->ResolutionX = pGiVideoModeInformation[GiCurrentVideoModeIndex].Width;
    pGiDeviceCapabilities->ResolutionY = pGiVideoModeInformation[GiCurrentVideoModeIndex].Height;
    pGiDeviceCapabilities->NumberOfColors = pGiVideoModeInformation[GiCurrentVideoModeIndex].NumberOfColors;
    pGiDeviceCapabilities->NumberOfVideoModes = GiSupportedVideoModesCount;
    pGiDeviceCapabilities->CurrentVideoModeIndex = GiCurrentVideoModeIndex;
    pGiDeviceCapabilities->RasterCapabilities = RASTER_CAPABILITIES_NONE;
}

VOID GiSetVideoMode(UINT32 requestedVideoMode)
{
    UiDrawText("GiSetVideoMode()");

    KSTATUS result = DeviceIoControl(hGiPrimaryVideoAdapter, IOCTL_VIDEO_SET_CURRENT_MODE, &requestedVideoMode, sizeof(UINT32), NULL, 0);

    if (result != STATUS_SUCCESS)
    {
      UiDrawText("Failed to set video mode.");
      HALT;
    }

    // Available configurations
    //GiPalette2ColorInit();
    //GiPalette4ColorInit();
    //GiPalette8ColorInit();
    //GiPalette16ColorInit();
    //GiPalette16ColorVGAInit();
    //GiPalette16ColorGrayInit();
    //GiPalette555Init();
    //GiPalette666Init();
    //GiPalette676Init();
    //GiPalette884Init();
    //GiPaletteGrayscaleInit();

    //GiClearPalette();

    if (requestedVideoMode == 0)
    {
        //GiPalette555Init();
        //GiPalette666Init();
        GiPalette676Init();
        //GiPalette884Init();
        //GiPalette488Init();
        //GiPalette4416Init();
        //GiPalette3328Init();
    }

    if (requestedVideoMode == 1)
    {
        GiPalette2ColorInit();
    }

    if (requestedVideoMode == 2)
    {
        //GiPalette16ColorInit();
        //GiPalette16ColorVGAInit();
        //GiPalette16ColorGrayInit();
        GiPalette16ColorUniqueInit();
        //GiPalette4ColorInit();
        //GiPalette2ColorInit();
    }

    if (requestedVideoMode == 3)
    {
        //GiPalette666Init();
        GiPalette4416Init();
    }

    GiCurrentVideoModeIndex = requestedVideoMode;
    GiScreenWidth = pGiVideoModeInformation[GiCurrentVideoModeIndex].Width;
    GiScreenHeight = pGiVideoModeInformation[GiCurrentVideoModeIndex].Height;
    GiScreenNumberOfBytesPerPixel = sizeof(UINT32);
    GiScreenWidthInBytes = GiScreenWidth * GiScreenNumberOfBytesPerPixel;
    GiDeviceWidthInBytes = pGiVideoModeInformation[GiCurrentVideoModeIndex].WidthInBytes;

    GiClearDeviceFrameBuffer();

    GiGetDeviceFunctions();
    GiSetDeviceDrawMode(DRAW_MODE_NORMAL);
    GiSetDeviceMouseCursor(hArrowCursor);
}

VOID GiGetDeviceFunctions()
{
    KSTATUS result = DeviceIoControl(hGiPrimaryVideoAdapter, IOCTL_VIDEO_GET_DEVICE_FUNCTIONS, NULL, 0, &VideoDeviceFunctionsVtbl, sizeof(VIDEODEVICEFUNCTIONS_VTBL));

    if (result != STATUS_SUCCESS)
    {
      UiDrawText("Failed to get device functions.");
      HALT;
    }
}

VOID GiClearDeviceFrameBuffer()
{
    memset((char*) pGiDeviceFrameBuffer, 0, GiDeviceWidthInBytes * GiScreenHeight);
}

VOID GiShowVariableNameAndValue(PCHAR pVariableName, UINT32 variableValue, UINT32 base)
{
    CHAR textBuffer[40] = { 0 };
    size_t textLength;

    textLength = strlen((const char*)pVariableName);
    strcpy(&textBuffer[0], (const char*)pVariableName);
    textBuffer[textLength] = '=';
    itoa(variableValue, &textBuffer[textLength + 1], base);

    UiDrawText(&textBuffer[0]);
}

VOID GiClearPalette()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;

    GiCreatePalette(numberOfColors, OUT &pPalette);

    for (UINT32 i = 0; i < 256; i++)
    {
        GiSetPaletteEntry(pPalette,  i, 0x00, 0x00, 0x00);
    }

    GiSetDevicePalette(pPalette);
}

// Sets a 256 color grayscale palette on the primary display device
VOID GiPaletteGrayscaleInit()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

	for(UINT32 grayIndex = 0; grayIndex < 256; grayIndex++)
	{
        GiSetPaletteEntry(pPalette, grayIndex, grayIndex, grayIndex, grayIndex);
	}

    GiSetDevicePalette(pPalette);
}

// Sets the 6R-6G-6B palette on the primary display device
VOID GiPalette666Init()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;
    UINT8  redCount   = 6;
    UINT8  greenCount = 6;
    UINT8  blueCount  = 6;

    // UINT8  redValues[]	 = { 0x00, 0x33, 0x66, 0x99, 0xCC, 0xFF };
    // UINT8  greenValues[]  = { 0x00, 0x33, 0x66, 0x99, 0xCC, 0xFF };
    // UINT8  blueValues[]	 = { 0x00, 0x33, 0x66, 0x99, 0xCC, 0xFF };

    UINT8  redValues[]	 = { 0x00, 0x33, 0x66, 0x80, 0xC0, 0xFF };
    UINT8  greenValues[] = { 0x00, 0x33, 0x66, 0x80, 0xC0, 0xFF };
    UINT8  blueValues[]	 = { 0x00, 0x33, 0x66, 0x80, 0xC0, 0xFF };


    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

	UINT32 i = 0;
	for(UINT8 redIndex = 0; redIndex < redCount; redIndex++)
	{
	  	for(UINT8 greenIndex = 0; greenIndex < greenCount; greenIndex++)
	  	{
	  		for(UINT8 blueIndex = 0; blueIndex < blueCount; blueIndex++)
	  		{
                GiSetPaletteEntry(pPalette, i, redValues[redIndex], greenValues[greenIndex], blueValues[blueIndex]);

	  			i++;
	  		}
	  	}
	}

    GiSetDevicePalette(pPalette);
}

// Sets the 5R-5G-5B palette on the primary display device
VOID GiPalette555Init()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 125;
    UINT8  redCount   = 5;
    UINT8  greenCount = 5;
    UINT8  blueCount  = 5;

    // UINT8  redValues[]	 = { 0x00, 0x33, 0x66, 0x99, 0xCC, 0xFF };
    // UINT8  greenValues[]  = { 0x00, 0x33, 0x66, 0x99, 0xCC, 0xFF };
    // UINT8  blueValues[]	 = { 0x00, 0x33, 0x66, 0x99, 0xCC, 0xFF };

    UINT8  redValues[]	 = { 0x00, 0x3F, 0x7E, 0xBD, 0xFC };
    UINT8  greenValues[] = { 0x00, 0x3F, 0x7E, 0xBD, 0xFC };
    UINT8  blueValues[]	 = { 0x00, 0x3F, 0x7E, 0xBD, 0xFC };


    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

	UINT32 i = 0;
	for(UINT8 redIndex = 0; redIndex < redCount; redIndex++)
	{
	  	for(UINT8 greenIndex = 0; greenIndex < greenCount; greenIndex++)
	  	{
	  		for(UINT8 blueIndex = 0; blueIndex < blueCount; blueIndex++)
	  		{
                GiSetPaletteEntry(pPalette, i, redValues[redIndex], greenValues[greenIndex], blueValues[blueIndex]);

	  			i++;
	  		}
	  	}
	}

    GiSetDevicePalette(pPalette);
}

// Sets the 16 color palette on the primary display device
VOID GiPalette16ColorInit()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

    // Palette set on device
    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  1, 0x00, 0x00, 0x80);
    GiSetPaletteEntry(pPalette,  2, 0x00, 0x80, 0x00);
    GiSetPaletteEntry(pPalette,  3, 0x00, 0x80, 0x80);
    GiSetPaletteEntry(pPalette,  4, 0x80, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  5, 0x80, 0x00, 0x80);
    GiSetPaletteEntry(pPalette, 20, 0x80, 0x80, 0x00);
    GiSetPaletteEntry(pPalette,  7, 0xC0, 0xC0, 0xC0);
    GiSetPaletteEntry(pPalette, 56, 0x80, 0x80, 0x80);
    GiSetPaletteEntry(pPalette, 57, 0x00, 0x00, 0xFF);
    GiSetPaletteEntry(pPalette, 58, 0x00, 0xFF, 0x00);
    GiSetPaletteEntry(pPalette, 59, 0x00, 0xFF, 0xFF);
    GiSetPaletteEntry(pPalette, 60, 0xFF, 0x00, 0x00);
    GiSetPaletteEntry(pPalette, 61, 0xFF, 0x00, 0xFF);
    GiSetPaletteEntry(pPalette, 62, 0xFF, 0xFF, 0x00);
    GiSetPaletteEntry(pPalette, 63, 0xFF, 0xFF, 0xFF);

    GiSetDevicePalette(pPalette);

    // Logical palette used for color lookup
    pPalette->NumberOfEntries = 16;
    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  1, 0x00, 0x00, 0x80);
    GiSetPaletteEntry(pPalette,  2, 0x00, 0x80, 0x00);
    GiSetPaletteEntry(pPalette,  3, 0x00, 0x80, 0x80);
    GiSetPaletteEntry(pPalette,  4, 0x80, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  5, 0x80, 0x00, 0x80);
    GiSetPaletteEntry(pPalette,  6, 0x80, 0x80, 0x00);
    GiSetPaletteEntry(pPalette,  7, 0xC0, 0xC0, 0xC0);
    GiSetPaletteEntry(pPalette,  8, 0x80, 0x80, 0x80);
    GiSetPaletteEntry(pPalette,  9, 0x00, 0x00, 0xFF);
    GiSetPaletteEntry(pPalette, 10, 0x00, 0xFF, 0x00);
    GiSetPaletteEntry(pPalette, 11, 0x00, 0xFF, 0xFF);
    GiSetPaletteEntry(pPalette, 12, 0xFF, 0x00, 0x00);
    GiSetPaletteEntry(pPalette, 13, 0xFF, 0x00, 0xFF);
    GiSetPaletteEntry(pPalette, 14, 0xFF, 0xFF, 0x00);
    GiSetPaletteEntry(pPalette, 15, 0xFF, 0xFF, 0xFF);
}

// Sets the 16 color palette on the primary display device
VOID GiPalette16ColorVGAInit()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

    // Palette set on device
    GiSetPaletteEntry(pPalette,  0,   0,   0,   0);
    GiSetPaletteEntry(pPalette,  1,   0,   0, 170);
    GiSetPaletteEntry(pPalette,  2,   0, 170,  85);
    GiSetPaletteEntry(pPalette,  3,  85, 170, 170);
    GiSetPaletteEntry(pPalette,  4, 170,   0,  85);
    GiSetPaletteEntry(pPalette,  5, 170,  85, 170);
    GiSetPaletteEntry(pPalette, 20, 170, 170,  85);
    GiSetPaletteEntry(pPalette,  7, 195, 199, 203);
    GiSetPaletteEntry(pPalette, 56, 134, 138, 142);
    GiSetPaletteEntry(pPalette, 57,   0,   0, 255);
    GiSetPaletteEntry(pPalette, 58,   0, 255,   0);
    GiSetPaletteEntry(pPalette, 59,   0, 255, 255);
    GiSetPaletteEntry(pPalette, 60, 255,   0,   0);
    GiSetPaletteEntry(pPalette, 61, 255,   0, 255);
    GiSetPaletteEntry(pPalette, 62, 255, 255,   0);
    GiSetPaletteEntry(pPalette, 63, 255, 255, 255);

    GiSetDevicePalette(pPalette);

    // Logical palette used for color lookup
    pPalette->NumberOfEntries = 16;
    GiSetPaletteEntry(pPalette,  0,   0,   0,   0);
    GiSetPaletteEntry(pPalette,  1,   0,   0, 170);
    GiSetPaletteEntry(pPalette,  2,   0, 170,  85);
    GiSetPaletteEntry(pPalette,  3,  85, 170, 170);
    GiSetPaletteEntry(pPalette,  4, 170,   0,  85);
    GiSetPaletteEntry(pPalette,  5, 170,  85, 170);
    GiSetPaletteEntry(pPalette,  6, 170, 170,  85);
    GiSetPaletteEntry(pPalette,  7, 195, 199, 203);
    GiSetPaletteEntry(pPalette,  8, 134, 138, 142);
    GiSetPaletteEntry(pPalette,  9,   0,   0, 255);
    GiSetPaletteEntry(pPalette, 10,   0, 255,   0);
    GiSetPaletteEntry(pPalette, 11,   0, 255, 255);
    GiSetPaletteEntry(pPalette, 12, 255,   0,   0);
    GiSetPaletteEntry(pPalette, 13, 255,   0, 255);
    GiSetPaletteEntry(pPalette, 14, 255, 255,   0);
    GiSetPaletteEntry(pPalette, 15, 255, 255, 255);
}


VOID GiPalette16ColorGrayInit()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

    // Palette set on device
    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  1, 0x11, 0x11, 0x11);
    GiSetPaletteEntry(pPalette,  2, 0x22, 0x22, 0x22);
    GiSetPaletteEntry(pPalette,  3, 0x33, 0x33, 0x33);
    GiSetPaletteEntry(pPalette,  4, 0x44, 0x44, 0x44);
    GiSetPaletteEntry(pPalette,  5, 0x55, 0x55, 0x55);
    GiSetPaletteEntry(pPalette, 20, 0x66, 0x66, 0x66);
    GiSetPaletteEntry(pPalette,  7, 0x77, 0x77, 0x77);
    GiSetPaletteEntry(pPalette, 56, 0x88, 0x88, 0x88);
    GiSetPaletteEntry(pPalette, 57, 0x99, 0x99, 0x99);
    GiSetPaletteEntry(pPalette, 58, 0xAA, 0xAA, 0xAA);
    GiSetPaletteEntry(pPalette, 59, 0xBB, 0xBB, 0xBB);
    GiSetPaletteEntry(pPalette, 60, 0xCC, 0xCC, 0xCC);
    GiSetPaletteEntry(pPalette, 61, 0xDD, 0xDD, 0xDD);
    GiSetPaletteEntry(pPalette, 62, 0xEE, 0xEE, 0xEE);
    GiSetPaletteEntry(pPalette, 63, 0xFF, 0xFF, 0xFF);

    GiSetDevicePalette(pPalette);

    // Logical palette used for color lookup
    pPalette->NumberOfEntries = 16;
    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  1, 0x11, 0x11, 0x11);
    GiSetPaletteEntry(pPalette,  2, 0x22, 0x22, 0x22);
    GiSetPaletteEntry(pPalette,  3, 0x33, 0x33, 0x33);
    GiSetPaletteEntry(pPalette,  4, 0x44, 0x44, 0x44);
    GiSetPaletteEntry(pPalette,  5, 0x55, 0x55, 0x55);
    GiSetPaletteEntry(pPalette,  6, 0x66, 0x66, 0x66);
    GiSetPaletteEntry(pPalette,  7, 0x77, 0x77, 0x77);
    GiSetPaletteEntry(pPalette,  8, 0x88, 0x88, 0x88);
    GiSetPaletteEntry(pPalette,  9, 0x99, 0x99, 0x99);
    GiSetPaletteEntry(pPalette, 10, 0xAA, 0xAA, 0xAA);
    GiSetPaletteEntry(pPalette, 11, 0xBB, 0xBB, 0xBB);
    GiSetPaletteEntry(pPalette, 12, 0xCC, 0xCC, 0xCC);
    GiSetPaletteEntry(pPalette, 13, 0xDD, 0xDD, 0xDD);
    GiSetPaletteEntry(pPalette, 14, 0xEE, 0xEE, 0xEE);
    GiSetPaletteEntry(pPalette, 15, 0xFF, 0xFF, 0xFF);
}

VOID GiPalette16ColorUniqueInit()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

    // Palette set on device
    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  1, 0x00, 0x00, 0x22);
    GiSetPaletteEntry(pPalette,  2, 0x00, 0x00, 0x44);
    GiSetPaletteEntry(pPalette,  3, 0x00, 0x00, 0x66);
    GiSetPaletteEntry(pPalette,  4, 0x00, 0x00, 0x88);
    GiSetPaletteEntry(pPalette,  5, 0x00, 0x00, 0xAA);
    GiSetPaletteEntry(pPalette, 20, 0x00, 0x00, 0xCC);
    GiSetPaletteEntry(pPalette,  7, 0x00, 0x00, 0xFF);
    GiSetPaletteEntry(pPalette, 56, 0x40, 0x40, 0x40);
    GiSetPaletteEntry(pPalette, 57, 0x60, 0x60, 0x60);
    GiSetPaletteEntry(pPalette, 58, 0x80, 0x80, 0x80);
    GiSetPaletteEntry(pPalette, 59, 0xA0, 0xA0, 0xA0);
    GiSetPaletteEntry(pPalette, 60, 0xB0, 0xB0, 0xB0);
    GiSetPaletteEntry(pPalette, 61, 0xC0, 0xC0, 0xC0);
    GiSetPaletteEntry(pPalette, 62, 0xE0, 0xE0, 0xE0);
    GiSetPaletteEntry(pPalette, 63, 0xFF, 0xFF, 0xFF);

    GiSetDevicePalette(pPalette);

    // Logical palette used for color lookup
    pPalette->NumberOfEntries = 16;
    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  1, 0x00, 0x00, 0x22);
    GiSetPaletteEntry(pPalette,  2, 0x00, 0x00, 0x44);
    GiSetPaletteEntry(pPalette,  3, 0x00, 0x00, 0x66);
    GiSetPaletteEntry(pPalette,  4, 0x00, 0x00, 0x88);
    GiSetPaletteEntry(pPalette,  5, 0x00, 0x00, 0xAA);
    GiSetPaletteEntry(pPalette,  6, 0x00, 0x00, 0xCC);
    GiSetPaletteEntry(pPalette,  7, 0x00, 0x00, 0xFF);
    GiSetPaletteEntry(pPalette,  8, 0x30, 0x30, 0x30);
    GiSetPaletteEntry(pPalette,  9, 0x60, 0x60, 0x60);
    GiSetPaletteEntry(pPalette, 10, 0x80, 0x80, 0x80);
    GiSetPaletteEntry(pPalette, 11, 0xA0, 0xA0, 0xA0);
    GiSetPaletteEntry(pPalette, 12, 0xB0, 0xB0, 0xB0);
    GiSetPaletteEntry(pPalette, 13, 0xC0, 0xC0, 0xC0);
    GiSetPaletteEntry(pPalette, 14, 0xE0, 0xE0, 0xE0);
    GiSetPaletteEntry(pPalette, 15, 0xFF, 0xFF, 0xFF);
}

VOID GiPalette8ColorInit()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

    // Grays
    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  1, 0x22, 0x22, 0x22);
    GiSetPaletteEntry(pPalette,  2, 0x44, 0x44, 0x44);
    GiSetPaletteEntry(pPalette,  3, 0x66, 0x66, 0x66);
    GiSetPaletteEntry(pPalette,  4, 0x88, 0x88, 0x88);
    GiSetPaletteEntry(pPalette,  5, 0xAA, 0xAA, 0xAA);
    GiSetPaletteEntry(pPalette,  6, 0xCC, 0xCC, 0xCC);
    GiSetPaletteEntry(pPalette,  7, 0xFF, 0xFF, 0xFF);

    // EGA - Black, Blue, Magenta, White
    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  1, 0x00, 0x80, 0x80);
    GiSetPaletteEntry(pPalette,  2, 0x80, 0x00, 0x80);
    GiSetPaletteEntry(pPalette,  3, 0x80, 0x80, 0x80);
    GiSetPaletteEntry(pPalette,  4, 0x00, 0xFF, 0xFF);
    GiSetPaletteEntry(pPalette,  5, 0xFF, 0x00, 0xFF);
    GiSetPaletteEntry(pPalette, 20 , 0xCC, 0xCC, 0xCC);
    GiSetPaletteEntry(pPalette,  7, 0xFF, 0xFF, 0xFF);

    GiSetDevicePalette(pPalette);

    // Logical palette used for color lookup
    pPalette->NumberOfEntries = 8;
    GiSetPaletteEntry(pPalette,  6, 0xCC, 0xCC, 0xCC);
}


VOID GiPalette4ColorInit()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 4;

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

    // CGA - Black, Blue, Magenta, White
    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  1, 0x00, 0xFF, 0xFF);
    GiSetPaletteEntry(pPalette,  2, 0xFF, 0x00, 0xFF);
    GiSetPaletteEntry(pPalette,  3, 0xFF, 0xFF, 0xFF);

    // CGA - Black, Red, Green, Yellow
    // GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    // GiSetPaletteEntry(pPalette,  1, 0xFF, 0x00, 0x00);
    // GiSetPaletteEntry(pPalette,  2, 0x00, 0xFF, 0x00);
    // GiSetPaletteEntry(pPalette,  3, 0xFF, 0xFF, 0x00);

    // Grays
    // GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    // GiSetPaletteEntry(pPalette,  1, 0x66, 0x66, 0x66);
    // GiSetPaletteEntry(pPalette,  2, 0xCC, 0xCC, 0xCC);
    // GiSetPaletteEntry(pPalette,  3, 0xFF, 0xFF, 0xFF);

    GiSetDevicePalette(pPalette);
}

VOID GiPalette2ColorInit()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  63, 0x00, 0xA0, 0xA0);

    GiSetDevicePalette(pPalette);

    numberOfColors = 2;
    GiSetPaletteEntry(pPalette,  0, 0x00, 0x00, 0x00);
    GiSetPaletteEntry(pPalette,  1, 0x00, 0xA0, 0xA0);

}

// Sets the 6R-7G-6B palette on the primary display device
VOID GiPalette676Init()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;
    UINT8  redCount   = 6;
    UINT8  greenCount = 7;
    UINT8  blueCount  = 6;
    UINT8  redValues[]	 = { 0x00, 0x33, 0x66, 0x99, 0xCC, 0xFF };
    UINT8  greenValues[] = { 0x00, 0x2A, 0x55, 0x80, 0xAA, 0xD4, 0xFF };
    UINT8  blueValues[]	 = { 0x00, 0x33, 0x66, 0x99, 0xCC, 0xFF };

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

	UINT32 i = 0;
	for(UINT8 redIndex = 0; redIndex < redCount; redIndex++)
	{
	    for(UINT8 greenIndex = 0; greenIndex < greenCount; greenIndex++)
	  	{
	  		for(UINT8 blueIndex = 0; blueIndex < blueCount; blueIndex++)
	  		{
                GiSetPaletteEntry(pPalette, i, redValues[redIndex], greenValues[greenIndex], blueValues[blueIndex]);

	  			i++;
	  		}
	  	}
	}

	// Fill remaining 4 colors with grays
	UINT32 grayCount = 4;
	UINT32 grayStep = 256 / grayCount;
	for (UINT32 j = i + 1; j < i + grayCount; j++)
	{
        GiSetPaletteEntry(pPalette, j, j * grayStep, j * grayStep, j * grayStep);
	}

    GiSetDevicePalette(pPalette);
}

// Sets the 8R-8G-4B palette on the primary display device
VOID GiPalette884Init()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;
    UINT8  redCount   = 8;
    UINT8  greenCount = 8;
    UINT8  blueCount  = 4;
    UINT8  redValues[]	 = { 0x00, 0x24, 0x49, 0x6D, 0x92, 0xB6, 0xDB, 0xFF };
    UINT8  greenValues[] = { 0x00, 0x24, 0x49, 0x6D, 0x92, 0xB6, 0xDB, 0xFF };
    UINT8  blueValues[]	 = { 0x00, 0x55, 0xAA, 0xFF };

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

	UINT32 i = 0;
	for(UINT8 redIndex = 0; redIndex < redCount; redIndex++)
	{
	  	for(UINT8 greenIndex = 0; greenIndex < greenCount; greenIndex++)
	  	{
	  		for(UINT8 blueIndex = 0; blueIndex < blueCount; blueIndex++)
	  		{
                GiSetPaletteEntry(pPalette, i, redValues[redIndex], greenValues[greenIndex], blueValues[blueIndex]);

	  			i++;
	  		}
	  	}
	}

    GiSetDevicePalette(pPalette);
}

// Sets the 4R-8G-8B palette on the primary display device
VOID GiPalette488Init()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;
    UINT8  redCount   = 4;
    UINT8  greenCount = 8;
    UINT8  blueCount  = 8;
    UINT8  redValues[]	 = { 0x00, 0x55, 0xAA, 0xFF };
    UINT8  greenValues[] = { 0x00, 0x24, 0x49, 0x6D, 0x92, 0xB6, 0xDB, 0xFF };
    UINT8  blueValues[]	 = { 0x00, 0x24, 0x49, 0x6D, 0x92, 0xB6, 0xDB, 0xFF };

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

	UINT32 i = 0;
	for(UINT8 redIndex = 0; redIndex < redCount; redIndex++)
	{
	  	for(UINT8 greenIndex = 0; greenIndex < greenCount; greenIndex++)
	  	{
	  		for(UINT8 blueIndex = 0; blueIndex < blueCount; blueIndex++)
	  		{
                GiSetPaletteEntry(pPalette, i, redValues[redIndex], greenValues[greenIndex], blueValues[blueIndex]);

	  			i++;
	  		}
	  	}
	}

    GiSetDevicePalette(pPalette);
}

// Sets the 4R-48G-16B palette on the primary display device
VOID GiPalette4416Init()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;
    UINT8  redCount   = 4;
    UINT8  greenCount = 4;
    UINT8  blueCount  = 16;
    UINT8  redValues[]	 = { 0x00, 0x7F, 0xBF, 0xFF };
    UINT8  greenValues[] = { 0x00, 0x7F, 0xBF, 0xFF };
    UINT8  blueValues[]	 = { 0x00, 0x1F, 0x2F, 0x3F, 0x4F, 0x5F, 0x6F, 0x7F, 0x8F, 0x9F, 0xAF, 0xBF, 0xCF, 0xDF, 0xEF, 0xFF };

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

	UINT32 i = 0;
	for(UINT8 redIndex = 0; redIndex < redCount; redIndex++)
	{
	  	for(UINT8 greenIndex = 0; greenIndex < greenCount; greenIndex++)
	  	{
	  		for(UINT8 blueIndex = 0; blueIndex < blueCount; blueIndex++)
	  		{
                GiSetPaletteEntry(pPalette, i, redValues[redIndex], greenValues[greenIndex], blueValues[blueIndex]);

	  			i++;
	  		}
	  	}
	}

    GiSetDevicePalette(pPalette);
}

// Sets the 4R-48G-16B palette on the primary display device
VOID GiPalette3328Init()
{
    PPALETTE pPalette = NULL;
    UINT32 numberOfColors = 256;
    UINT8  redCount   = 3;
    UINT8  greenCount = 3;
    UINT8  blueCount  = 28;
    UINT8  redValues[]	 = { 0x00, 0xA0, 0xFF };
    UINT8  greenValues[] = { 0x00, 0xA0, 0xFF };
    UINT8  blueValues[]	 = { 0x00, 0x09, 0x12, 0x1B, 0x24, 0x2D, 0x36, 0x3F, 0x48, 0x51, 0x5A, 0x63, 0x6C, 0x75, 0x7E, 0x87, 0x90, 0x99, 0xA0, 0xAB, 0xB4, 0xBD, 0xC6, 0xCF, 0xD8, 0xE1, 0xEA, 0xFF   };

    GiCreatePalette(numberOfColors, OUT &pPalette);

    if (pPalette == NULL)
    {
        UiDrawText("Unable to create palette!");
        HALT;
    }

	UINT32 i = 0;
	for(UINT8 redIndex = 0; redIndex < redCount; redIndex++)
	{
	  	for(UINT8 greenIndex = 0; greenIndex < greenCount; greenIndex++)
	  	{
	  		for(UINT8 blueIndex = 0; blueIndex < blueCount; blueIndex++)
	  		{
                GiSetPaletteEntry(pPalette, i, redValues[redIndex], greenValues[greenIndex], blueValues[blueIndex]);

	  			i++;
	  		}
	  	}
	}

    GiSetDevicePalette(pPalette);
}

VOID GiCreatePalette(UINT32 numberOfEntries, OUT PPALETTE* pPalette)
{
    // Allocate space for PaletteEntries
    HANDLE hPaletteEntries = MemoryAllocate(numberOfEntries * sizeof(PALETTE_ENTRY), MEMORY_FLAG_ZERO_INIT, (PCHAR) &"PaletteEntries");

    if (hPaletteEntries == (HANDLE)NULL)
    {
        UiDrawText("GiCreatePalette() - Not enough memory for PaletteEntries!");
        HALT;
    }

    PPALETTE_ENTRY pPaletteEntries = (PPALETTE_ENTRY) MemoryLock(hPaletteEntries);

    if (pPaletteEntries == NULL)
    {
        UiDrawText("GiCreatePalette() - Can't lock memory of PaletteEntries!");
        HALT;
    }

    // Allocate space for PaletteStruct
    HANDLE hPalette = MemoryAllocate(sizeof(PALETTE), MEMORY_FLAG_ZERO_INIT, (PCHAR) &"PaletteStruct");

    if (hPalette == (HANDLE)NULL)
    {
        UiDrawText("GiCreatePalette() - Not enough memory for PaletteStruct!");
        HALT;
    }

    *pPalette = (PPALETTE) MemoryLock(hPalette);

    if (pPalette == NULL)
    {
        UiDrawText("GiCreatePalette() - Can't lock memory of PaletteStruct!");
        HALT;
    }

    // Fill in PaletteStruct
    (*pPalette)->NumberOfEntries = numberOfEntries;
    (*pPalette)->pPaletteEntries = pPaletteEntries;
}

VOID GiSetPaletteEntry(PPALETTE pPalette, UINT32 entryIndex, UINT8 red, UINT8 green, UINT8 blue)
{
    PPALETTE_ENTRY pPaletteEntry = pPalette->pPaletteEntries;

    pPaletteEntry += entryIndex;

    pPaletteEntry->Red = red;
    pPaletteEntry->Green = green;
    pPaletteEntry->Blue = blue;
}

COLOR GiGetPaletteEntryColor(PPALETTE pPalette, UINT32 entryIndex)
{
    if (entryIndex > pPalette->NumberOfEntries - 1)
    {
        return 0;
    }

    PPALETTE_ENTRY pPaletteEntry = pPalette->pPaletteEntries;

    pPaletteEntry += entryIndex;

    return GetColorFromComponents(pPaletteEntry->Red, pPaletteEntry->Green, pPaletteEntry->Blue);
}

UINT32 GiGetPaletteIndex(PPALETTE pPalette, COLOR color)
{
    UINT32 paletteEntryCount = pGiDevicePalette->NumberOfEntries;
    PPALETTE_ENTRY pPaletteEntry = pGiDevicePalette->pPaletteEntries;

    INT32 red;
    INT32 green;
    INT32 blue;
    INT32 redDiff;
	INT32 greenDiff;
	INT32 blueDiff;
    INT32 diffMin;
    INT32 diff;
    INT32 index;

    red =  (color >> 16 & 0xFF);
    green = (color >> 8 & 0xFF);
    blue =       (color & 0xFF);
    diffMin = 3 * 255;

    for(UINT32 i = 0; i < paletteEntryCount; i++)
    {
        redDiff    = pPaletteEntry[i].Red   - red;
        greenDiff  = pPaletteEntry[i].Green - green;
        blueDiff   = pPaletteEntry[i].Blue  - blue;

        if (redDiff == 0 && greenDiff == 0 && blueDiff == 0)
        {
            return i;
        }

        if (redDiff < 0)
        {
            //redDiff *= -1;
            redDiff = 0 - redDiff;
        }

        if (greenDiff < 0)
        {
            //greenDiff *= -1;
            greenDiff = 0 - greenDiff;
        }

        if (blueDiff < 0)
        {
            //blueDiff *= -1;
            blueDiff = 0 - blueDiff;
        }

        diff = redDiff + greenDiff + blueDiff;

        if (diff < diffMin)
        {
            index = i;
            diffMin = diff;
        }
    }

    return index;
}

VOID GiSetDevicePalette(PPALETTE pPalette)
{
    KSTATUS result = DeviceIoControl(hGiPrimaryVideoAdapter, IOCTL_VIDEO_SET_PALETTE, &pPalette, sizeof(PVOID), NULL, 0);

    if (result != STATUS_SUCCESS)
    {
      UiDrawText("Failed to set palette on video device.");
      HALT;
    }

    pGiDevicePalette = pPalette;
}

VOID DrawPalette(HDEVICE_CONTEXT hDeviceContext, INT32 posX, INT32 posY)
{
    UINT8 index = 0;
    for(UINT32 y = 0; y < 16; y++)
    {
        for(UINT32 x = 0; x < 16; x++)
        {
            COLOR color = GiGetPaletteEntryColor(pGiDevicePalette, index);

            SetBackgroundColor(hDeviceContext, color);
            DrawFilledRectangleA(hDeviceContext, posX + x * 8, posY + y * 8,  posX + x * 8 + 8, posY + y * 8 + 8);

            index++;
        }
    }
}

VOID GiDefaultFontInitialize()
{
    // Allocate buffer
    HFONT hFont = MemoryAllocate(DEFAULT_FONT_BUFFER_SIZE, MEMORY_FLAG_ZERO_INIT, (PCHAR) &"DefaultFont");

    if (hFont == (HFONT) NULL)
    {
        UiDrawText("Failed to initialize Default Font.");
        HALT;
    }

    PFONT pFont = MemoryLock(hFont);

    if (pFont == NULL)
    {
        UiDrawText("Failed to initialize Default Font.");
        HALT;
    }

    // Copy default font to new location
    strncpy((PCHAR) pFont, (PCHAR) pDefaultFont, DEFAULT_FONT_BUFFER_SIZE);

    hGiDefaultFont = hFont;

    MemoryUnlock(hFont);
}

VOID CreateCompatibleIcon(HICON hIcon)
{
    PICON pIcon = MemoryLock(hIcon);

    if (pIcon == (PICON) NULL)
    {
        return;
    }

    pIcon->hBitmapAndMask = CreateCompatibleBitmap(pIcon->hBitmapAndMask);
    pIcon->hBitmapXorMask = CreateCompatibleBitmap(pIcon->hBitmapXorMask);

    MemoryUnlock(hIcon);
}

VOID GiCursorsInitialize()
{
    //hArrowCursor = LoadIconFromMemory(pProgramsIcon);
    hArrowCursor = LoadIconFromMemory(pArrowCursor);

    CreateCompatibleIcon(hArrowCursor);
}



// --- INITIALIZATION - END ---

// PUBLIC

UINT32 GetNumberOfDevices()
{
    return NUMBER_OF_VIDEO_DEVICES;
}

UINT32 EnumerateDevices(OUT PDISPLAY_DEVICE pDisplayDevice, UINT32 bufferSizeInBytes)
{
    if (bufferSizeInBytes < sizeof(DISPLAY_DEVICE))
    {
        return 0;
    }

    strcpy((char*) pDisplayDevice->DeviceName, (const char*) &"\\VideoAdapter0");
    strcpy((char*) pDisplayDevice->DeviceDescription, (const char*) &"Primary Video Adapter");
    pDisplayDevice->hDevice = hGiPrimaryVideoAdapter;

    return NUMBER_OF_VIDEO_DEVICES;
}

HDEVICE GetPrimaryDisplayDevice()
{
    return hGiPrimaryVideoAdapter;
}

KSTATUS GetDeviceCapabilities(HDEVICE hDevice, OUT PDEVICE_CAPABILITIES pDeviceCapabilities)
{
    if (hDevice != hGiPrimaryVideoAdapter)
    {
        return IO_STATUS_ERROR_INVALID_REQUEST;
    }

    pDeviceCapabilities[0] = pGiDeviceCapabilities[0];

    return STATUS_SUCCESS;
}

KSTATUS GetVideoModeInformation(HDEVICE hDevice, UINT32 videoModeNumber, OUT PVIDEO_MODE_INFORMATION pVideoModeInformation)
{
    if (hDevice != hGiPrimaryVideoAdapter)
    {
        return IO_STATUS_ERROR_INVALID_REQUEST;
    }

    if (videoModeNumber == VIDEO_MODE_NUMBER_CURRENT)
    {
        videoModeNumber = GiCurrentVideoModeIndex;
    }

    if (videoModeNumber > GiSupportedVideoModesCount)
    {
        return IO_STATUS_ERROR_INVALID_REQUEST;
    }

    pVideoModeInformation[0] = pGiVideoModeInformation[videoModeNumber];

    return STATUS_SUCCESS;
}

KSTATUS GetCurrentVideoModeInformation(HDEVICE hDevice, OUT PVIDEO_MODE_INFORMATION pVideoModeInformation)
{
    return GetVideoModeInformation(hDevice, GiCurrentVideoModeIndex, pVideoModeInformation);
}

KSTATUS SetVideoMode(HDEVICE hDevice, UINT32 videoModeNumber)
{
    if (hDevice != hGiPrimaryVideoAdapter)
    {
        return IO_STATUS_ERROR_INVALID_REQUEST;
    }

    //MachineDisableInterrupts();
    //PicDisableIRQs();
    //PicRestoreBiosIRQs();
    //PicMapBiosIRQs();

    GiSetVideoMode(videoModeNumber);

    //MachineDisableInterrupts();
    //PicDisableIRQs();


    //PicMapSystemIRQs();
    //PicDisableIRQs();
    //PicEnableSpecificIRQs();
    //MachineEnableInterrupts();

    return STATUS_SUCCESS;
}

HDEVICE_CONTEXT CreateDeviceContext(HDEVICE hDevice)
{
    UiDrawText("CreateDeviceContext()");

    if (hDevice != hGiPrimaryVideoAdapter)
    {
        return (HDEVICE_CONTEXT) NULL;
    }

    // Create Device Context
    HDEVICE_CONTEXT hDeviceContext = (HDEVICE_CONTEXT) MemoryAllocate(sizeof(DEVICE_CONTEXT), MEMORY_FLAG_ZERO_INIT,(PCHAR) &"DeviceContext");

    if (hDeviceContext == (HDEVICE_CONTEXT) NULL)
    {
        BugCheck(NULL, (PCHAR) &"Device handle not valid!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Device handle:", hDeviceContext);
        //return (HDEVICE_CONTEXT) NULL;
    }

    PDEVICE_CONTEXT pDeviceContext = (PDEVICE_CONTEXT) MemoryLock(hDeviceContext);

    if (pDeviceContext == (PDEVICE_CONTEXT) NULL)
    {
        BugCheck(NULL, (PCHAR) &"Device handle not valid!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Device handle:", hDeviceContext);
        //return (HDEVICE_CONTEXT) NULL;
    }

    // Create Window Rectangle
    HANDLE hWindowRectangle = MemoryAllocate(sizeof(RECTANGLE), MEMORY_FLAG_ZERO_INIT, (PCHAR) &"Rectangle");

    if (hWindowRectangle == (HANDLE) NULL)
    {
        BugCheck(NULL, (PCHAR) &"Rectangle handle not valid!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Rectangle handle:", hWindowRectangle);
        //return (HDEVICE_CONTEXT) NULL;
    }

    PRECTANGLE pWindowRectangle = (PRECTANGLE) MemoryLock(hWindowRectangle);

    if (pWindowRectangle == (PRECTANGLE) NULL)
    {
        BugCheck(NULL, (PCHAR) &"Rectangle handle not valid!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Rectangle handle:", hWindowRectangle);
        //return (HDEVICE_CONTEXT) NULL;
    }

    // Create Clipping Region
    HANDLE hClippingRegion = MemoryAllocate(sizeof(REGION), MEMORY_FLAG_ZERO_INIT, (PCHAR) &"ClippingRegion");

    if (hClippingRegion == (HANDLE) NULL)
    {
        BugCheck(NULL, (PCHAR) &"Region handle not valid!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Region handle:", hClippingRegion);
        //return (HDEVICE_CONTEXT) NULL;
    }

    PREGION pClippingRegion = (PREGION) MemoryLock(hClippingRegion);

    if (pClippingRegion == (PREGION) NULL)
    {
        BugCheck(NULL, (PCHAR) &"Region handle not valid!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Region handle:", hClippingRegion);
        //return (HDEVICE_CONTEXT) NULL;
    }

    // Set up Device Context

    pWindowRectangle->Left = 0; // Should be replaced with a CreateRectangle later
    pWindowRectangle->Top = 0;
    pWindowRectangle->Right = pGiVideoModeInformation[GiCurrentVideoModeIndex].Width;
    pWindowRectangle->Bottom = pGiVideoModeInformation[GiCurrentVideoModeIndex].Height;

    pClippingRegion->NumberOfRectangles = 0;    // Should be replaced with a CreateRegion later
    pClippingRegion->pClippingRectangleList = (PRECTANGLE*) NULL;

    pDeviceContext->hDevice                 = hGiPrimaryVideoAdapter;
    pDeviceContext->hDeviceContext          = hDeviceContext;
    pDeviceContext->pDeviceCapabilities     = pGiDeviceCapabilities;
    pDeviceContext->pVideoModeInformation   = &pGiVideoModeInformation[GiCurrentVideoModeIndex];
    pDeviceContext->pDeviceFrameBuffer      = pGiDeviceFrameBuffer;
    pDeviceContext->hWindow                 = (HWINDOW) NULL;
    pDeviceContext->pWindowRectangle        = pWindowRectangle;
    pDeviceContext->pClippingRegion         = pClippingRegion;
    pDeviceContext->hPen                    = (HPEN) NULL;
    pDeviceContext->hBrush                  = (HBRUSH) NULL;
    pDeviceContext->hFont                   = (HFONT) hGiDefaultFont;
    pDeviceContext->TransparentColor        = 0xFFFF00FF;
    pDeviceContext->ForegroundColor         = 0xFF000000;
    pDeviceContext->BackgroundColor         = 0xFFFFFFFF;
    pDeviceContext->TextColor               = 0xFF000000;
    pDeviceContext->DrawMode                = DRAW_MODE_NORMAL;
    pDeviceContext->ReferenceCount          = 0;
    pDeviceContext->LastErrorCode           = 0;
    pDeviceContext->pNext                   = NULL;

    // Add to list

    PDEVICE_CONTEXT pDeviceContextEntry = pGiDeviceContextListHead;

    while(pDeviceContextEntry->pNext != NULL)
    {
        pDeviceContextEntry = pDeviceContextEntry->pNext;
    }

    pDeviceContextEntry->pNext = pDeviceContext;

    return hDeviceContext;
}

BOOL IsValidDeviceContext(HDEVICE_CONTEXT hDeviceContext)
{
    if (hDeviceContext == (HDEVICE_CONTEXT) NULL)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CheckIfDeviceContextValid(HDEVICE_CONTEXT hDeviceContext)
{
    if (hDeviceContext == (HDEVICE_CONTEXT) NULL)
    {
        return FALSE;
    }

    PDEVICE_CONTEXT pDeviceContext = (PDEVICE_CONTEXT) MemoryLock(hDeviceContext);
    HANDLE hDevice = pDeviceContext->hDevice;
    MemoryUnlock(hDeviceContext);

    if (hDevice != hGiPrimaryVideoAdapter)
    {
        return FALSE;
    }

    return TRUE;
}

KSTATUS DeleteDeviceContext(HDEVICE_CONTEXT hDeviceContext)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);
    MemoryUnlock(hDeviceContext);

    if (pDeviceContext->ReferenceCount != 0)
    {
        return STATUS_ERROR_INVALID_REQUEST;
    }

    // Remove from list

    PDEVICE_CONTEXT pDeviceContextEntry = pGiDeviceContextListHead;

    while(pDeviceContextEntry->pNext != pDeviceContext)
    {
        pDeviceContextEntry = pDeviceContextEntry->pNext;
    }

    pDeviceContextEntry->pNext = pDeviceContext->pNext;

    MemoryUnlock(hDeviceContext);
    MemoryFree(hDeviceContext);

    return STATUS_SUCCESS;
}

HDEVICE_CONTEXT GetDeviceContext(HWINDOW hWindow)
{
    UiDrawText("GetDeviceContext()");

    if (!IsValid(hWindow))
    {
        return (HDEVICE_CONTEXT) NULL;
    }

    PDEVICE_CONTEXT pDeviceContextEntry = pGiDeviceContextListHead;

    while(pDeviceContextEntry != NULL)
    {
        if (pDeviceContextEntry->hWindow == hWindow)
        {
            pDeviceContextEntry->ReferenceCount++;

            RECTANGLE windowRectangle;

            // THIS SHOULD BE REPLACED BY A GETCLIENTRECTANGLE + RECTANGLETOSCREEN CALL
            GetScreenRectangle(hWindow, &windowRectangle);
            RectangleReduceToClientArea(hWindow, &windowRectangle);

            pDeviceContextEntry->pWindowRectangle->Left = windowRectangle.Left;
            pDeviceContextEntry->pWindowRectangle->Top = windowRectangle.Top;
            pDeviceContextEntry->pWindowRectangle->Right = windowRectangle.Right;
            pDeviceContextEntry->pWindowRectangle->Bottom = windowRectangle.Bottom;

            return pDeviceContextEntry->hDeviceContext;
        }

        pDeviceContextEntry = pDeviceContextEntry->pNext;
    }

    // For this Window no DeviceContext exist yet

    HDEVICE_CONTEXT hNewDeviceContext = CreateDeviceContext(hGiPrimaryVideoAdapter);
    PDEVICE_CONTEXT pNewDeviceContext = MemoryLock(hNewDeviceContext);

    RECTANGLE windowRectangle;
    // THIS SHOULD BE REPLACED BY A GETCLIENTRECTANGLE + RECTANGLETOSCREEN CALL
    GetScreenRectangle(hWindow, &windowRectangle);
    RectangleReduceToClientArea(hWindow, &windowRectangle);

    pNewDeviceContext->hWindow = hWindow;
    pNewDeviceContext->pWindowRectangle->Left = windowRectangle.Left;
    pNewDeviceContext->pWindowRectangle->Top = windowRectangle.Top;
    pNewDeviceContext->pWindowRectangle->Right = windowRectangle.Right;
    pNewDeviceContext->pWindowRectangle->Bottom = windowRectangle.Bottom;
    pNewDeviceContext->pNext = NULL;

    if (pGiDeviceContextListHead = NULL)
    {
        pGiDeviceContextListHead = pNewDeviceContext;
    }
    else
    {
        pNewDeviceContext->pNext = pGiDeviceContextListHead;
        pGiDeviceContextListHead = pNewDeviceContext;
    }

    //MemoryUnlock(hNewDeviceContext);

    return hNewDeviceContext;
}

HDEVICE_CONTEXT GetDeviceContextNonClient(HWINDOW hWindow)
{
    UiDrawText("GetDeviceContext()");

    if (!IsValid(hWindow))
    {
        return (HDEVICE_CONTEXT) NULL;
    }

    PDEVICE_CONTEXT pDeviceContextEntry = pGiDeviceContextListHead;

    while(pDeviceContextEntry != NULL)
    {
        if (pDeviceContextEntry->hWindow == hWindow)
        {
            pDeviceContextEntry->ReferenceCount++;

            RECTANGLE windowRectangle;

            GetScreenRectangle(hWindow, &windowRectangle);

            pDeviceContextEntry->pWindowRectangle->Left = windowRectangle.Left;
            pDeviceContextEntry->pWindowRectangle->Top = windowRectangle.Top;
            pDeviceContextEntry->pWindowRectangle->Right = windowRectangle.Right;
            pDeviceContextEntry->pWindowRectangle->Bottom = windowRectangle.Bottom;

            return pDeviceContextEntry->hDeviceContext;
        }

        pDeviceContextEntry = pDeviceContextEntry->pNext;
    }

    // For this Window no DeviceContext exist yet

    HDEVICE_CONTEXT hNewDeviceContext = CreateDeviceContext(hGiPrimaryVideoAdapter);
    PDEVICE_CONTEXT pNewDeviceContext = MemoryLock(hNewDeviceContext);

    RECTANGLE windowRectangle;
    GetScreenRectangle(hWindow, &windowRectangle);

    pNewDeviceContext->hWindow = hWindow;
    pNewDeviceContext->pWindowRectangle->Left = windowRectangle.Left;
    pNewDeviceContext->pWindowRectangle->Top = windowRectangle.Top;
    pNewDeviceContext->pWindowRectangle->Right = windowRectangle.Right;
    pNewDeviceContext->pWindowRectangle->Bottom = windowRectangle.Bottom;
    pNewDeviceContext->pNext = NULL;

    if (pGiDeviceContextListHead = NULL)
    {
        pGiDeviceContextListHead = pNewDeviceContext;
    }
    else
    {
        pNewDeviceContext->pNext = pGiDeviceContextListHead;
        pGiDeviceContextListHead = pNewDeviceContext;
    }

    //MemoryUnlock(hNewDeviceContext);

    return hNewDeviceContext;
}

DRAWMODE GetDrawMode(HDEVICE_CONTEXT hDeviceContext)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);

    if (pDeviceContext == NULL)
    {
        return 0;
    }

    MemoryUnlock(hDeviceContext);

    return pDeviceContext->DrawMode;
}

VOID GiSetDeviceDrawMode(DRAWMODE drawMode)
{
    if (drawMode & DRAW_MODE_NORMAL)
    {
        DrawPixelVtbl.DeviceDrawPixel = VideoDeviceFunctionsVtbl.DeviceDrawPixelOverwrite;

        return;
    }

    if (drawMode & DRAW_MODE_XOR)
    {
        DrawPixelVtbl.DeviceDrawPixel = VideoDeviceFunctionsVtbl.DeviceDrawPixelXor;

        return;
    }
}

VOID SetDrawMode(HDEVICE_CONTEXT hDeviceContext, DRAWMODE drawMode)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);

    if (pDeviceContext == NULL)
    {
        return;
    }

    pDeviceContext->DrawMode = drawMode;
    GiRasterOperationMode = drawMode;

    GiSetDeviceDrawMode(drawMode);

    MemoryUnlock(hDeviceContext);
}

VOID GiSetDeviceMouseCursor(HCURSOR hCursor)
{
    DeviceSetMouseCursor(hCursor);
}

VOID ReleaseDeviceContext(HDEVICE_CONTEXT hDeviceContext)
{
    UiDrawText("ReleaseDeviceContext()");
}

VOID GiGetDeviceRectangle(HDEVICE_CONTEXT hDeviceContext, PRECTANGLE pRectangle)
{
    if (!CheckIfDeviceContextValid(hDeviceContext))
    {
        return;
    }

    pRectangle->Left   = 0;
    pRectangle->Top    = 0;
    pRectangle->Right  = GiScreenWidth;
    pRectangle->Bottom = GiScreenHeight;
}

VOID SetRectangleA(PRECTANGLE pRectangle, INT32 left, INT32 top, INT32 right, INT32 bottom)
{
    pRectangle->Left   = left;
    pRectangle->Top    = top;
    pRectangle->Right  = right;
    pRectangle->Bottom = bottom;
}

VOID SetRectangleB(PRECTANGLE pRectangle, INT32 left, INT32 top, INT32 width, INT32 height)
{
    pRectangle->Left   = left;
    pRectangle->Top    = top;
    pRectangle->Right  = left + width;
    pRectangle->Bottom = top + height;
}

VOID SetRectangleEmpty(PRECTANGLE pRectangle)
{
    pRectangle->Left   = 0;
    pRectangle->Top    = 0;
    pRectangle->Right  = 0;
    pRectangle->Bottom = 0;
}

BOOL CheckIfEqualRectangles(PRECTANGLE pRectangle1, PRECTANGLE pRectangle2)
{
    if (pRectangle1->Left   == pRectangle2->Left &&
	    pRectangle1->Top    == pRectangle2->Top &&
	    pRectangle1->Right  == pRectangle2->Right &&
	    pRectangle1->Bottom == pRectangle2->Bottom)
    {
        return TRUE;
    }

    return FALSE;
}

// A rectangle is considered empty when the computed area is 0
BOOL CheckIfRectangleEmpty(PRECTANGLE pRectangle)
{
    if (pRectangle->Left >= pRectangle->Right ||
	    pRectangle->Top  >= pRectangle->Bottom)
    {
        return TRUE;
    }

    return FALSE;
}

// A rectangle is considered empty when the left and right sides or the top and bottom sides are mixed up
BOOL CheckIfRectangleValid(PRECTANGLE pRectangle)
{
    if (pRectangle->Left < pRectangle->Right &&
	    pRectangle->Top  < pRectangle->Bottom)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL CheckIfPointInRectangle(PRECTANGLE pRectangle, PPOINT pPoint)
{
    if (pRectangle->Left   <= pPoint->X &&
	    pRectangle->Top    <= pPoint->Y &&
	    pRectangle->Right  >  pPoint->X &&
	    pRectangle->Bottom >  pPoint->Y)
    {
        return TRUE;
    }

    return FALSE;
}

VOID OffsetRectangle(PRECTANGLE pRectangle, PPOINT pPoint)
{
    pRectangle->Left   += pPoint->X;
    pRectangle->Right  += pPoint->X;
    pRectangle->Top    += pPoint->Y;
    pRectangle->Bottom += pPoint->Y;
}

VOID IntersectRectangle(PRECTANGLE pSourceRectangle1, PRECTANGLE pSourceRectangle2, PRECTANGLE pDestinationRectangle)
{
    pDestinationRectangle->Left   = Max(pSourceRectangle1->Left, pSourceRectangle2->Left);
    pDestinationRectangle->Right  = Min(pSourceRectangle1->Right, pSourceRectangle2->Right);
    pDestinationRectangle->Top    = Max(pSourceRectangle1->Top, pSourceRectangle2->Top);
    pDestinationRectangle->Bottom = Min(pSourceRectangle1->Bottom, pSourceRectangle2->Bottom);

    if (!CheckIfRectangleValid(pDestinationRectangle))
    {
        pDestinationRectangle->Right  = pDestinationRectangle->Left;
        pDestinationRectangle->Bottom = pDestinationRectangle->Top;
    }
}

VOID UnionRectangle(PRECTANGLE pSourceRectangle1, PRECTANGLE pSourceRectangle2, PRECTANGLE pDestinationRectangle)
{
    pDestinationRectangle->Left   = Min(pSourceRectangle1->Left, pSourceRectangle2->Left);
    pDestinationRectangle->Right  = Max(pSourceRectangle1->Right, pSourceRectangle2->Right);
    pDestinationRectangle->Top    = Min(pSourceRectangle1->Top, pSourceRectangle2->Top);
    pDestinationRectangle->Bottom = Max(pSourceRectangle1->Bottom, pSourceRectangle2->Bottom);
}

INT32 Min(INT32 number1, INT32 number2)
{
    if (number1 < number2)
    {
        return number1;
    }

    return number2;
}

INT32 Max(INT32 number1, INT32 number2)
{
    if (number1 > number2)
    {
        return number1;
    }

    return number2;
}


VOID SubtractRectangle(PRECTANGLE pSourceRectangle1, PRECTANGLE pSourceRectangle2, PRECTANGLE pDestinationRectangle)
{
  // ???
}

// This is only used to set the updateable region within the device context
// Otherwise drawing will take place for the whole client rectangle
VOID DrawBegin(HDEVICE_CONTEXT hDeviceContext)
{
    if (!hDeviceContext)
    {
        return;
    }

    HideMouse();
}

// Validates the whole region of the device context
VOID DrawEnd(HDEVICE_CONTEXT hDeviceContext)
{
    if (!hDeviceContext)
    {
        return;
    }

    ShowMouse();
}

VOID GiDrawLineCore(PRECTANGLE pClippingRect, INT32 xStart, INT32 yStart, INT32 xEnd, INT32 yEnd, COLOR color)
{
    INT32 x;
    INT32 y;
    INT32 deltaX;
    INT32 deltaY;
    INT32 pixelCount;
    INT32 d;
    INT32 dInc1;
    INT32 dInc2;
    INT32 xInc1;
    INT32 xInc2;
    INT32 yInc1;
    INT32 yInc2;

    // Drawing
    if (xEnd > xStart)
    {
        deltaX = xEnd - xStart;
    }
    else
    {
        deltaX = xStart - xEnd;
    }

    if (yEnd > yStart)
    {
        deltaY = yEnd - yStart;
    }
    else
    {
        deltaY = yStart - yEnd;
    }

    // Set initial values based on whether X or Y is the independent variable
    if (deltaX > deltaY)
    {
        // X is the independent variable
        pixelCount = deltaX + 1;
        d = (2 * deltaY) - deltaX;
        dInc1 = deltaY << 1;
        dInc2 = (deltaY - deltaX) << 1;
        xInc1 = 1;
        xInc2 = 1;
        yInc1 = 0;
        yInc2 = 1;
    }
    else
    {
        // Y is the independent variable
        pixelCount = deltaY + 1;
        d = (2 * deltaX) - deltaY;
        dInc1 = deltaX << 1;
        dInc2 = (deltaX - deltaY) << 1;
        xInc1 = 0;
        xInc2 = 1;
        yInc1 = 1;
        yInc2 = 1;
    }

    // Make sure x and y move in the right directions
    if (xStart > xEnd)
    {
        xInc1 = -xInc1;
        xInc2 = -xInc2;
    }
    if (yStart > yEnd)
    {
        yInc1 = -yInc1;
        yInc2 = -yInc2;
    }

    // Drawing
    x = xStart;
    y = yStart;

    for(INT32 i = 0; i < pixelCount - 1; i++)
    {
        // TODO: Check clipping region
        // TODO: Check drawing is outside of device Rect?

        // Set pixel

        POINT point;
        point.X = x;
        point.Y = y;

        //if (x >= pClippingRect->Left && x < pClippingRect->Right && y >= pClippingRect->Top && y < pClippingRect->Bottom)
        if (CheckIfPointInRectangle(pClippingRect, &point))
        {
            GiDrawPixelCore(x, y, color);
        }

        if (d < 0)
        {
            d += dInc1;
            x += xInc1;
            y += yInc1;
        }
        else
        {
            d += dInc2;
            x += xInc2;
            y += yInc2;
        }
    }
}

VOID GiDrawPixelCore(INT32 x, INT32 y, COLOR color)
{
    // !!! Should not validate whether pixels are in the device !!!
    // Pixels should have been validated already !!!

    //if (x % 5 ==  y % 5)
    if ((GiRasterOperationMode & DRAW_MODE_PEN_DOT) && x % 2 !=  y % 2)
    {
        return;
    }

    if (x < 0 || y < 0 || x > GiScreenWidth || y > GiScreenHeight)
    {
        return;
    }

    DeviceDrawPixel(x, y, color);
}

VOID DrawFilledRectangleA(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 right, INT32 bottom)
{
    RECTANGLE rectangle;
    rectangle.Left = left;
    rectangle.Right = right;
    rectangle.Top = top;
    rectangle.Bottom = bottom;

    DrawFilledRectangleC(hDeviceContext, &rectangle);
}

VOID DrawFilledRectangleB(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 width, INT32 height)
{
    RECTANGLE rectangle;
    rectangle.Left = left;
    rectangle.Right = left + width;
    rectangle.Top = top;
    rectangle.Bottom = top + height;

    DrawFilledRectangleC(hDeviceContext, &rectangle);
}


VOID DrawFilledRectangleC(HDEVICE_CONTEXT hDeviceContext, PRECTANGLE pRectangle)
{
    PDEVICE_CONTEXT pDeviceContext;
    PRECTANGLE pWindowRect;
    RECTANGLE screenDeviceRect;
    PREGION pClippingRegion;
    RECTANGLE screenWindowRect;
    RECTANGLE screenRect;
    HWINDOW hWindow;
    COLOR color;

    // Validation
    if (!CheckIfDeviceContextValid(hDeviceContext))                     // -> checks checksum and that if hDevice is a valid display adapter (primary display adapter for now)
    {
        return;
    }

    if (CheckIfRectangleEmpty(pRectangle))                              //  -> checks if area = 0, or if left > right or top > bottom
    {
        return;
    }

    // Get info from device context (IsValidDevContext has already checked the checksum and whether we can lock the DC)
    pDeviceContext  = MemoryLock(hDeviceContext);
    hWindow         = pDeviceContext->hWindow;
    pWindowRect     = pDeviceContext->pWindowRectangle;  // Should always exist, even if what is stored is (0, 0, deviceW, deviceH)
    pClippingRegion = pDeviceContext->pClippingRegion;   // It should always exist, even if the stored rects are 0, if no clipping the clipping region is the whole device area
                                                         // Probably it should be stored in the window struct???

    color = GiGetPaletteIndex(pGiDevicePalette, pDeviceContext->BackgroundColor);

    // Set up drawable rectangle
    screenRect = pRectangle[0];

    // Set up window rectangle (depending on DC it's either Client or NonClient area)
    screenRect.Left   += pWindowRect->Left;
    screenRect.Top    += pWindowRect->Top;
    screenRect.Right  += pWindowRect->Left;
    screenRect.Bottom += pWindowRect->Top;

    GetScreenRectangle(hWindow, &screenWindowRect);                     // <-- pWindowRect is not needed in the device context! Actually it would be annoying always updating it!
    GiGetDeviceRectangle(hDeviceContext, &screenDeviceRect);                   // ???

    // for(UINT32 i = 0; i < pClippingRegion->NumberOfRectangles; i++)
    // {
    //     PRECTANGLE pClippingRect = pClippingRegion->pClippingRectangleList[i];  // The clipping rect is in client coordinates. But why? The user should never deal with them directly!

    //     // Transform to screen coordinates
    //     RECTANGLE screenClippingRect = pClippingRect[0];
    //     RectangleToScreen(hWindow , &screenClippingRect);

	//     IntersectRectangle(&screenRect, &screenClippingRect, &screenRect);
	//     IntersectRectangle(&screenRect, &screenWindowRect  , &screenRect);
	//     IntersectRectangle(&screenRect, &screenDeviceRect  , &screenRect);

	//     if (CheckIfRectangleEmpty(&screenRect))
    //     {
    //         continue;
    //     }

    //     GiDrawFilledRectangleCore(&screenRect, color);
    // }

	//IntersectRectangle(&screenRect, &screenClippingRect, &screenRect);
	//IntersectRectangle(&screenRect, &screenWindowRect  , &screenRect);
	IntersectRectangle(&screenRect, &screenDeviceRect  , &screenRect);

	if (CheckIfRectangleEmpty(&screenRect))
    {
        return;
    }

    GiDrawFilledRectangleCore(&screenRect, color);

    MemoryUnlock(hDeviceContext);
}

VOID GiDrawFilledRectangleCore(PRECTANGLE pScreenRect, COLOR color)
{
    INT32 left   = pScreenRect->Left;
    INT32 top    = pScreenRect->Top;
    INT32 right  = pScreenRect->Right;
    INT32 bottom = pScreenRect->Bottom;
    INT32 width  = right  - left;
    INT32 height = bottom - top;

    if (width <= 0 || height <= 0) return;     // ??? is this needed really ???

    DeviceDrawFilledRectangle(left, top, right, bottom, color);
}

VOID DrawLineA(HDEVICE_CONTEXT hDeviceContext, INT32 xStart, INT32 yStart, INT32 xEnd, INT32 yEnd)
{
    POINT start;
    POINT end;

    start.X = xStart;
    start.Y = yStart;
    end.X   = xEnd;
    end.Y   = yEnd;

    DrawLineB(hDeviceContext, &start, &end);
}

VOID DrawLineB(HDEVICE_CONTEXT hDeviceContext, PPOINT pStart, PPOINT pEnd)
{
    PDEVICE_CONTEXT pDeviceContext;
    PRECTANGLE pWindowRect;
    PREGION pClippingRegion;
    RECTANGLE screenWindowRect;
    RECTANGLE screenDeviceRect;
    POINT screenStart;
    POINT screenEnd;
    HWINDOW hWindow;
    COLOR color;

    // Validation
    if (!CheckIfDeviceContextValid(hDeviceContext))                     // -> checks checksum and that if hDevice is a valid display adapter (primary display adapter for now)
    {
        return;
    }

    // Get info from device context (IsValidDevContext has already checked the checksum and whether we can lock the DC)
    pDeviceContext   = MemoryLock(hDeviceContext);
    hWindow          = pDeviceContext->hWindow;
    pWindowRect      = pDeviceContext->pWindowRectangle;  // Should always exist, even if what is stored is (0, 0, deviceW, deviceH)
    pClippingRegion  = pDeviceContext->pClippingRegion;   // It should always exist, even if the stored rects are 0, if no clipping the clipping region is the whole device area

    color = GiGetPaletteIndex(pGiDevicePalette, pDeviceContext->ForegroundColor);

    // Set up screen rectangle
    screenWindowRect = pWindowRect[0];
    //GetWindowRect(hWindow, &screenWindowRect);                        // <-- pWindowRect is not needed in the device context! Actually it would be annoying always updating it!
    GiGetDeviceRectangle(hDeviceContext, &screenDeviceRect);

    // Set up points
    screenStart = pStart[0];
    screenEnd   = pEnd[0];

    // THIS SHOULD BE 2 CALLS BASED ON WHETHER OR NOT THE DC IS CLIENTDC OR WINDOWDC
    screenStart.X += pWindowRect->Left;
    screenStart.Y += pWindowRect->Top;
    screenEnd.X   += pWindowRect->Left;
    screenEnd.Y   += pWindowRect->Top;

    //PointNonClientToScreen(hWindow, &screenStart);
    //PointNonClientToScreen(hWindow, &screenEnd);

    // for(UINT32 i = 0; i < pClippingRegion->NumberOfRectangles; i++)
    // {
    //     PRECTANGLE pClippingRect = pClippingRegion->pClippingRectangleList[i];  // The clipping rect is in client coordinates. But why? The user should never deal with them directly!

    //     // Transform to screen coordinates
    //     RECTANGLE screenClippingRect = pClippingRect[0];
    //     ToScreenRectangle(hWindow , &screenClippingRect);

	// IntersectRectangle(&screenClippingRect, &screenWindowRect);
	// IntersectRectangle(&screenClippingRect, &screenDeviceRect);

    //     GiDrawLineCore(&screenClippingRect, &screenStart, &screenEnd, color);
    // }

	IntersectRectangle(&screenWindowRect, &screenDeviceRect, &screenWindowRect);
    GiDrawLineCore(&screenWindowRect, screenStart.X, screenStart.Y, screenEnd.X, screenEnd.Y, color);

    MemoryUnlock(hDeviceContext);
}

VOID DrawRectangleA(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 right, INT32 bottom)
{
    RECTANGLE rectangle;
    rectangle.Left = left;
    rectangle.Right = right;
    rectangle.Top = top;
    rectangle.Bottom = bottom;

    DrawRectangleC(hDeviceContext, &rectangle);
}

VOID DrawRectangleB(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 width, INT32 height)
{
    RECTANGLE rectangle;
    rectangle.Left = left;
    rectangle.Right = left + width;
    rectangle.Top = top;
    rectangle.Bottom = top + height;

    DrawRectangleC(hDeviceContext, &rectangle);
}

VOID DrawRectangleC(HDEVICE_CONTEXT hDeviceContext, PRECTANGLE pRectangle)
{
    DrawLineA(hDeviceContext, pRectangle->Left     , pRectangle->Top       , pRectangle->Right - 1, pRectangle->Top);
    DrawLineA(hDeviceContext, pRectangle->Left  + 1, pRectangle->Bottom - 1, pRectangle->Right    , pRectangle->Bottom - 1);
    DrawLineA(hDeviceContext, pRectangle->Left     , pRectangle->Top    + 1, pRectangle->Left     , pRectangle->Bottom);
    DrawLineA(hDeviceContext, pRectangle->Right - 1, pRectangle->Top       , pRectangle->Right - 1, pRectangle->Bottom - 1);
}

VOID DrawControlState(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 right, INT32 bottom, DRAWMODE controlFlags)
{
    RECTANGLE controlRectangle;
    controlRectangle.Left = left;
    controlRectangle.Top = top;
    controlRectangle.Right = right;
    controlRectangle.Bottom = bottom;

    if (!IsValidDeviceContext(hDeviceContext))
    {
        return;
    }

    if (controlFlags == DRAW_CONTROL_STATE_WINDOW_RESIZE_FRAME_THIN)
    {
        DRAWMODE prevDrawMode = GetDrawMode(hDeviceContext);
        SetDrawMode(hDeviceContext, DRAW_MODE_XOR | DRAW_MODE_PEN_DOT);

        DrawRectangleA(hDeviceContext, left, top, right, bottom);

        SetDrawMode(hDeviceContext, prevDrawMode);
    }

    if (controlFlags == DRAW_CONTROL_STATE_WINDOW_RESIZE_FRAME_THICK)
    {
        DRAWMODE prevDrawMode = GetDrawMode(hDeviceContext);
        SetDrawMode(hDeviceContext, DRAW_MODE_XOR | DRAW_MODE_PEN_DOT);

        for (UINT32 i = 0; i < GetSystemMetric(METRIC_BORDER_SIZE); i++)
        {
            DrawRectangleA(hDeviceContext, left + i, top + i, right - i, bottom - i);
        }

        SetDrawMode(hDeviceContext, prevDrawMode);
    }

    if (controlFlags == DRAW_CONTROL_STATE_EDGE_RAISED)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLighter);
        DrawLineA(hDeviceContext, left, top, right - 1, top);
        DrawLineA(hDeviceContext, left, top, left, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLight);
        DrawLineA(hDeviceContext, left + 1, top + 1, right - 2, top + 1);
        DrawLineA(hDeviceContext, left + 1, top + 1, left + 1, bottom - 2);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDark);
        DrawLineA(hDeviceContext, left + 1, bottom - 2, right - 2, bottom - 2);
        DrawLineA(hDeviceContext, right - 2, top + 1, right - 2, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDarker);
        DrawLineA(hDeviceContext, left, bottom - 1, right - 1, bottom - 1);
        DrawLineA(hDeviceContext, right - 1, top, right - 1, bottom);

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_EDGE_SUNKEN)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        //DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDarker);
        DrawLineA(hDeviceContext, left, top, right - 1, top);
        DrawLineA(hDeviceContext, left, top, left, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDark);
        DrawLineA(hDeviceContext, left + 1, top + 1, right - 2, top + 1);
        DrawLineA(hDeviceContext, left + 1, top + 1, left + 1, bottom - 2);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLight);
        DrawLineA(hDeviceContext, left + 1, bottom - 2, right - 2, bottom - 2);
        DrawLineA(hDeviceContext, right - 2, top + 1, right - 2, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLighter);
        DrawLineA(hDeviceContext, left, bottom - 1, right - 1, bottom - 1);
        DrawLineA(hDeviceContext, right - 1, top, right - 1, bottom);

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_EDGE_THIN_RAISED)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLighter);
        DrawLineA(hDeviceContext, left, top, right - 1, top);
        DrawLineA(hDeviceContext, left, top, left, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDark);
        DrawLineA(hDeviceContext, left, bottom - 1, right - 1, bottom - 1);
        DrawLineA(hDeviceContext, right - 1, top, right - 1, bottom);

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_EDGE_THIN_SUNKEN)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDark);
        DrawLineA(hDeviceContext, left, top, right - 1, top);
        DrawLineA(hDeviceContext, left, top, left, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLighter);
        DrawLineA(hDeviceContext, left, bottom - 1, right - 1, bottom - 1);
        DrawLineA(hDeviceContext, right - 1, top, right - 1, bottom);

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_EDGE_WINDOW)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        //SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        //DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLight);
        DrawLineA(hDeviceContext, left, top, right - 1, top);
        DrawLineA(hDeviceContext, left, top, left, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLighter);
        DrawLineA(hDeviceContext, left + 1, top + 1, right - 2, top + 1);
        DrawLineA(hDeviceContext, left + 1, top + 1, left + 1, bottom - 2);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDark);
        DrawLineA(hDeviceContext, left + 1, bottom - 2, right - 2, bottom - 2);
        DrawLineA(hDeviceContext, right - 2, top + 1, right - 2, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDarker);
        DrawLineA(hDeviceContext, left, bottom - 1, right - 1, bottom - 1);
        DrawLineA(hDeviceContext, right - 1, top, right - 1, bottom);

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_ACTIVE_CAPTION_GRADIENT)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);

        COLOR color  = GetSystemColor(COLOR_WINDOW_ACTIVE_TITLE_BAR);
        COLOR color2 = GetSystemColor(COLOR_WINDOW_ACTIVE_TITLE_BAR_GRADIENT);

        INT32 red   = color >> 16 & 0xFF;
        INT32 green = color >>  8 & 0xFF;
        INT32 blue  = color & 0xFF;

        INT32 red2   = color2 >> 16 & 0xFF;
        INT32 green2 = color2 >>  8 & 0xFF;
        INT32 blue2  = color2 & 0xFF;

        INT32 dRed   = red2   - red;
        INT32 dGreen = green2 - green;
        INT32 dBlue  = blue2  - blue;

        INT32 width = right - left;

        for(UINT32 x = 0; x < width; x++)
        {
            INT32 nextRed    = red   + (dRed   * x / width);
            INT32 nextGreen  = green + (dGreen * x / width);
            INT32 nextBlue   = blue  + (dBlue  * x / width);

            if (nextRed   < 0) nextRed   = 0;
            if (nextGreen < 0) nextGreen = 0;
            if (nextBlue  < 0) nextBlue  = 0;

            if (nextRed   > 255) nextRed   = 255;
            if (nextGreen > 255) nextGreen = 255;
            if (nextBlue  > 255) nextBlue  = 255;

            COLOR nextColor  = GetColorFromComponents(nextRed, nextGreen, nextBlue);

            SetBackgroundColor(hDeviceContext, nextColor);
            DrawFilledRectangleA(hDeviceContext, left + x, top, right, bottom);
        }

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_INACTIVE_CAPTION_GRADIENT)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);

        COLOR color  = GetSystemColor(COLOR_WINDOW_INACTIVE_TITLE_BAR);
        COLOR color2 = GetSystemColor(COLOR_WINDOW_INACTIVE_TITLE_BAR_GRADIENT);

        INT32 red   = color >> 16 & 0xFF;
        INT32 green = color >>  8 & 0xFF;
        INT32 blue  = color & 0xFF;

        INT32 red2   = color2 >> 16 & 0xFF;
        INT32 green2 = color2 >>  8 & 0xFF;
        INT32 blue2  = color2 & 0xFF;

        INT32 dRed   = red2   - red;
        INT32 dGreen = green2 - green;
        INT32 dBlue  = blue2  - blue;

        INT32 width = right - left;
        COLOR lastUsedColor = 0xFFFFFF;

        for(UINT32 x = 0; x < width; x++)
        {
            INT32 nextRed    = red   + (dRed   * x / width);
            INT32 nextGreen  = green + (dGreen * x / width);
            INT32 nextBlue   = blue  + (dBlue  * x / width);

            if (nextRed   < 0) nextRed   = 0;
            if (nextGreen < 0) nextGreen = 0;
            if (nextBlue  < 0) nextBlue  = 0;

            if (nextRed   > 255) nextRed   = 255;
            if (nextGreen > 255) nextGreen = 255;
            if (nextBlue  > 255) nextBlue  = 255;

            COLOR nextColor  = GetColorFromComponents(nextRed, nextGreen, nextBlue);

            if (nextColor != lastUsedColor)
            {
                SetBackgroundColor(hDeviceContext, nextColor);
                DrawFilledRectangleA(hDeviceContext, left + x, top, right, bottom);

                lastUsedColor = nextColor;
            }
        }

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_NORMAL)
    {
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetForegroundColor(hDeviceContext, GetSystemColor(COLOR_CONTROL_TEXT_NORMAL));
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_EDGE_WINDOW);
        //DrawText(hDeviceContext, "+", &controlRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        UINT32 scrollbarSize = GetSystemMetric(METRIC_SCROLLBAR_SIZE);
        UINT32 arrowStart = scrollbarSize / 2 - 2;

        for(UINT32 y = arrowStart, x = 1; y <= arrowStart + 3; y++, x++)
        {
            DrawLineA(hDeviceContext, left + scrollbarSize / 2 - x, top + y, left + scrollbarSize / 2 + x - 1, top + y);
        }
    }

    if (controlFlags == DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_ONHOVER)
    {
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetForegroundColor(hDeviceContext, GetSystemColor(COLOR_CONTROL_TEXT_ONHOVER));
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_EDGE_WINDOW);
        //DrawText(hDeviceContext, "+", &controlRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        UINT32 scrollbarSize = GetSystemMetric(METRIC_SCROLLBAR_SIZE);
        UINT32 arrowStart = scrollbarSize / 2 - 2;

        for(UINT32 y = arrowStart, x = 1; y <= arrowStart + 3; y++, x++)
        {
            DrawLineA(hDeviceContext, left + scrollbarSize / 2 - x, top + y, left + scrollbarSize / 2 + x - 1, top + y);
        }
    }

    if (controlFlags == DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_PRESSED)
    {
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetForegroundColor(hDeviceContext, GetSystemColor(COLOR_CONTROL_TEXT_ONHOVER));
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_EDGE_SUNKEN);
        //DrawText(hDeviceContext, "+", &controlRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        UINT32 scrollbarSize = GetSystemMetric(METRIC_SCROLLBAR_SIZE);
        UINT32 arrowStart = scrollbarSize / 2 - 2;

        for(UINT32 y = arrowStart + 1, x = 1; y <= arrowStart + 4; y++, x++)
        {
            DrawLineA(hDeviceContext, left + scrollbarSize / 2 - x + 1, top + y, left + scrollbarSize / 2 + x, top + y);
        }
    }

    if (controlFlags == DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_NORMAL)
    {
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetForegroundColor(hDeviceContext, GetSystemColor(COLOR_CONTROL_TEXT_NORMAL));
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_EDGE_WINDOW);
        //DrawText(hDeviceContext, "+", &controlRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        UINT32 scrollbarSize = GetSystemMetric(METRIC_SCROLLBAR_SIZE);
        UINT32 arrowStart = scrollbarSize / 2 - 2;

        for(UINT32 y = arrowStart + 3, x = 1; y >= arrowStart; y--, x++)
        {
            DrawLineA(hDeviceContext, left + scrollbarSize / 2 - x, top + y, left + scrollbarSize / 2 + x - 1, top + y);
        }
    }

    if (controlFlags == DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_ONHOVER)
    {
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetForegroundColor(hDeviceContext, GetSystemColor(COLOR_CONTROL_TEXT_ONHOVER));
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_EDGE_WINDOW);
        //DrawText(hDeviceContext, "+", &controlRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        UINT32 scrollbarSize = GetSystemMetric(METRIC_SCROLLBAR_SIZE);
        UINT32 arrowStart = scrollbarSize / 2 - 2;

        for(UINT32 y = arrowStart + 3, x = 1; y >= arrowStart; y--, x++)
        {
            DrawLineA(hDeviceContext, left + scrollbarSize / 2 - x, top + y, left + scrollbarSize / 2 + x - 1, top + y);
        }
    }

    if (controlFlags == DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_PRESSED)
    {
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetForegroundColor(hDeviceContext, GetSystemColor(COLOR_CONTROL_TEXT_ONHOVER));
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_EDGE_SUNKEN);
        //DrawText(hDeviceContext, "+", &controlRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        UINT32 scrollbarSize = GetSystemMetric(METRIC_SCROLLBAR_SIZE);
        UINT32 arrowStart = scrollbarSize / 2 - 2;

        for(UINT32 y = arrowStart + 4, x = 1; y >= arrowStart + 1; y--, x++)
        {
            DrawLineA(hDeviceContext, left + scrollbarSize / 2 - x + 1, top + y, left + scrollbarSize / 2 + x, top + y);
        }
    }

    if (controlFlags == DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_THUMB_NORMAL)
    {
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetTextColor(hDeviceContext, GetSystemColor(COLOR_CONTROL_TEXT_NORMAL));
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_EDGE_WINDOW);
        DrawText(hDeviceContext, "=", &controlRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
    }

    if (controlFlags == DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_THUMB_ONHOVER)
    {
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetTextColor(hDeviceContext, GetSystemColor(COLOR_CONTROL_TEXT_ONHOVER));
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_EDGE_WINDOW);
        DrawText(hDeviceContext, "=", &controlRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        UINT32 scrollbarSize = GetSystemMetric(METRIC_SCROLLBAR_SIZE);
    }

    if (controlFlags == DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_THUMB_PRESSED)
    {
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetTextColor(hDeviceContext, GetSystemColor(COLOR_CONTROL_TEXT_ONHOVER));
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_EDGE_SUNKEN);
        // controlRectangle.Left += 1;
        // controlRectangle.Top  += 1;
        // controlRectangle.Right += 1;
        // controlRectangle.Bottom  += 1;
        DrawText(hDeviceContext, "=", &controlRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
    }

    if (controlFlags == DRAW_CONTROL_STATE_FIELD)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        SetBackgroundColor(hDeviceContext, ColorControlContentArea);
        DrawFilledRectangleA(hDeviceContext, left + 2, top + 2, right - 2, bottom - 2);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDark);
        DrawLineA(hDeviceContext, left, top, right - 1, top);
        DrawLineA(hDeviceContext, left, top, left, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDarker);
        DrawLineA(hDeviceContext, left + 1, top + 1, right - 2, top + 1);
        DrawLineA(hDeviceContext, left + 1, top + 1, left + 1, bottom - 2);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLight);
        DrawLineA(hDeviceContext, left + 1, bottom - 2, right - 2, bottom - 2);
        DrawLineA(hDeviceContext, right - 2, top + 1, right - 2, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLighter);
        DrawLineA(hDeviceContext, left, bottom - 1, right - 1, bottom - 1);
        DrawLineA(hDeviceContext, right - 1, top, right - 1, bottom);

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_FIELD_DISABLED)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        DrawFilledRectangleA(hDeviceContext, left + 2, top + 2, right - 2, bottom - 2);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDark);
        DrawLineA(hDeviceContext, left, top, right - 1, top);
        DrawLineA(hDeviceContext, left, top, left, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDarker);
        DrawLineA(hDeviceContext, left + 1, top + 1, right - 2, top + 1);
        DrawLineA(hDeviceContext, left + 1, top + 1, left + 1, bottom - 2);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLight);
        DrawLineA(hDeviceContext, left + 1, bottom - 2, right - 2, bottom - 2);
        DrawLineA(hDeviceContext, right - 2, top + 1, right - 2, bottom - 1);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLighter);
        DrawLineA(hDeviceContext, left, bottom - 1, right - 1, bottom - 1);
        DrawLineA(hDeviceContext, right - 1, top, right - 1, bottom);

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_CHECKBOX_TICK || controlFlags == DRAW_CONTROL_STATE_CHECKBOX_TICK_DISABLED)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        UINT32 midX = (left + right) / 2;
        UINT32 midY = (top + bottom) / 2;

        SetForegroundColor(hDeviceContext, ColorControlTextNormal);

        if (controlFlags == DRAW_CONTROL_STATE_CHECKBOX_TICK_DISABLED)
        {
            SetForegroundColor(hDeviceContext, ColorControlTextDisabled);
        }

        DrawLineA(hDeviceContext, midX + 3, midY - 3, midX - 3, midY + 3);
        DrawLineA(hDeviceContext, midX + 3, midY - 2, midX - 2, midY + 3);
        DrawLineA(hDeviceContext, midX + 3, midY - 1, midX - 2, midY + 4);

        DrawLineA(hDeviceContext, midX - 3, midY - 1, midX - 0, midY + 2);
        DrawLineA(hDeviceContext, midX - 3, midY - 0, midX - 1, midY + 2);
        DrawLineA(hDeviceContext, midX - 3, midY + 1, midX - 1, midY + 3);

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }    

    if (controlFlags == DRAW_CONTROL_STATE_RADIOBUTTON)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        UINT32 midX = (left + right) / 2;
        UINT32 midY = (top + bottom) / 2;

        SetForegroundColor(hDeviceContext, ColorControlContentArea);

        for(UINT32 i = 0; i < 5; i++)
        {
            DrawLineA(hDeviceContext, midX - 4 + i, midY + i, midX + 1 + i, midY - 5 + i);
        }        

        for(UINT32 i = 0; i < 4; i++)
        {
            DrawLineA(hDeviceContext, midX - 3 + i, midY + i, midX + 1 + i, midY - 4 + i);
        }        

        SetForegroundColor(hDeviceContext, ColorControlEdgeDark);
        DrawLineA(hDeviceContext, midX - 6, midY, midX - 0, midY - 6);
        DrawLineA(hDeviceContext, midX - 6, midY, midX - 0, midY + 6);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDarker);
        DrawLineA(hDeviceContext, midX - 5, midY, midX - 0, midY - 5);
        DrawLineA(hDeviceContext, midX - 5, midY, midX - 0, midY + 5);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLight);
        DrawLineA(hDeviceContext, midX + 5, midY, midX - 1, midY - 6);
        DrawLineA(hDeviceContext, midX + 5, midY, midX - 1, midY + 6);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLighter);
        DrawLineA(hDeviceContext, midX + 6, midY, midX - 1, midY - 7);
        DrawLineA(hDeviceContext, midX + 6, midY, midX - 1, midY + 7);

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_RADIOBUTTON_DISABLED)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        UINT32 midX = (left + right) / 2;
        UINT32 midY = (top + bottom) / 2;

        SetForegroundColor(hDeviceContext, ColorControlSurfaceNormal);

        for(UINT32 i = 0; i < 5; i++)
        {
            DrawLineA(hDeviceContext, midX - 4 + i, midY + i, midX + 1 + i, midY - 5 + i);
        }        

        for(UINT32 i = 0; i < 4; i++)
        {
            DrawLineA(hDeviceContext, midX - 3 + i, midY + i, midX + 1 + i, midY - 4 + i);
        }        

        SetForegroundColor(hDeviceContext, ColorControlEdgeDark);
        DrawLineA(hDeviceContext, midX - 6, midY, midX - 0, midY - 6);
        DrawLineA(hDeviceContext, midX - 6, midY, midX - 0, midY + 6);

        SetForegroundColor(hDeviceContext, ColorControlEdgeDarker);
        DrawLineA(hDeviceContext, midX - 5, midY, midX - 0, midY - 5);
        DrawLineA(hDeviceContext, midX - 5, midY, midX - 0, midY + 5);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLight);
        DrawLineA(hDeviceContext, midX + 5, midY, midX - 1, midY - 6);
        DrawLineA(hDeviceContext, midX + 5, midY, midX - 1, midY + 6);

        SetForegroundColor(hDeviceContext, ColorControlEdgeLighter);
        DrawLineA(hDeviceContext, midX + 6, midY, midX - 1, midY - 7);
        DrawLineA(hDeviceContext, midX + 6, midY, midX - 1, midY + 7);

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }

    if (controlFlags == DRAW_CONTROL_STATE_RADIOBUTTON_SELECTION || controlFlags == DRAW_CONTROL_STATE_RADIOBUTTON_SELECTION_DISABLED)
    {
        COLOR prevBackgroundColor = GetBackgroundColor(hDeviceContext);
        COLOR prevForegroundColor = GetForegroundColor(hDeviceContext);

        UINT32 midX = (left + right) / 2;
        UINT32 midY = (top + bottom) / 2;

        SetForegroundColor(hDeviceContext, ColorControlTextNormal);

        if (controlFlags == DRAW_CONTROL_STATE_RADIOBUTTON_SELECTION_DISABLED)
        {
            SetForegroundColor(hDeviceContext, ColorControlTextDisabled);
        }

        for(UINT32 i = 0; i < 3; i++)
        {
            DrawLineA(hDeviceContext, midX - 2 + i, midY + i, midX + 1 + i, midY - 3 + i);
        }        

        for(UINT32 i = 0; i < 2; i++)
        {
            DrawLineA(hDeviceContext, midX - 1 + i, midY + i, midX + 1 + i, midY - 2 + i);
        }        

        SetBackgroundColor(hDeviceContext, prevBackgroundColor);
        SetForegroundColor(hDeviceContext, prevForegroundColor);
    }

}

COLOR GetColorFromComponents(UINT8 red, UINT8 green, UINT8 blue)
{
    return   0xFF000000 + ((UINT32) red << 16) + ((UINT32) green << 8) + ((UINT32) blue);
}

UINT32 GetTextWidth(HDEVICE_CONTEXT hDeviceContext, PCHAR text, DRAW_TEXT_FLAGS flags)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);
    HFONT hFont = pDeviceContext->hFont;
    PFONT pFont = (PFONT) MemoryLock(hFont);

    // GiShowVariableNameAndValue("hFont", hFont, 16);
    // GiShowVariableNameAndValue("pFont", (UINT32) pFont, 16);
    // GiShowVariableNameAndValue("pFont->dfVersion", pFont->dfVersion, 16);
    // GiShowVariableNameAndValue("pFont->dfSize", pFont->dfSize, 16);
    // GiShowVariableNameAndValue("pFont->dfFirstChar", pFont->dfFirstChar, 10);
    // GiShowVariableNameAndValue("pFont->dfLastChar", pFont->dfLastChar, 10);
    // GiShowVariableNameAndValue("pFont->dfDefaultChar", pFont->dfDefaultChar, 10);

    UINT32 textLength = strlen(text);
    UINT32 textWidth = 0;

    for(UINT32 i = 0; i < textLength; i++)
    {
        CHAR letter = text[i];

        if (letter < pFont->dfFirstChar || letter > pFont->dfLastChar)
        {
            letter = pFont->dfDefaultChar;
        }

        letter -= pFont->dfFirstChar;

        UINT32 letterOffset = ((UINT32) letter) * 4; // 4 bytes for Win2.0 fonts / 6 bytes for Win3.0 fonts
        letterOffset += LETTER_DESCRIPTIONS_OFFSET;  // LETTER_DESCRIPTIONS_OFFSET_EXTRA needs to be added for Win3.0 fonts

        PUINT8 pLetterDescriptor = ((PUINT8) pFont) + letterOffset;

        textWidth += ((PUINT16)pLetterDescriptor)[0];

        if (flags & DRAW_TEXT_FLAG_BOLD)
        {
            // Add extra kerning (space between letters)
            textWidth++;
        }
    }

    MemoryUnlock(hFont);
    MemoryUnlock(hDeviceContext);

    return textWidth;
}

UINT32 GetTextHeight(HDEVICE_CONTEXT hDeviceContext, PCHAR text)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);
    HFONT hFont = pDeviceContext->hFont;
    PFONT pFont = (PFONT) MemoryLock(hFont);

    UINT32 fontHeight = pFont->dfPixHeight;

    MemoryUnlock(hFont);
    MemoryUnlock(hDeviceContext);

    return fontHeight;
}

VOID DrawText(HDEVICE_CONTEXT hDeviceContext, PCHAR text, PRECTANGLE pRectangle, DRAW_TEXT_FLAGS flags)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);
    HFONT hFont = pDeviceContext->hFont;
    PFONT pFont = (PFONT) MemoryLock(hFont);
    COLOR color;

    if (pDeviceContext->hDevice == hGiPrimaryVideoAdapter)
    {
        UINT32 textLength = strlen(text);
        UINT32 textWidth = 0;

        INT32 textDrawableWidth = GetTextWidth(hDeviceContext, text, flags);
        INT32 textDrawableHeight = GetTextHeight(hDeviceContext, text);

        PRECTANGLE pWindowRect = pDeviceContext->pWindowRectangle;
        PREGION pClippingRegion = pDeviceContext->pClippingRegion;

        color = GiGetPaletteIndex(pGiDevicePalette, pDeviceContext->TextColor);

        INT32 textLeft = pRectangle->Left;
        INT32 textTop = pRectangle->Top;

        // Horizontal Alignment
        if (flags & DRAW_TEXT_FLAG_ALIGN_LEFT)
        {
            textLeft = pRectangle->Left;
        }

        if (flags & DRAW_TEXT_FLAG_ALIGN_RIGHT)
        {
            textLeft = pRectangle->Right - textDrawableWidth;
        }

        if (flags & DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER)
        {
            textLeft = (pRectangle->Left + pRectangle->Right - 1 - textDrawableWidth) / 2;
        }

        // Vertical Alignment
        if (flags & DRAW_TEXT_FLAG_ALIGN_TOP)
        {
            textTop = pRectangle->Top;
        }

        if (flags & DRAW_TEXT_FLAG_ALIGN_BOTTOM)
        {
            textTop = pRectangle->Bottom - textDrawableHeight;
        }

        if (flags & DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER)
        {
            textTop = (pRectangle->Top + pRectangle->Bottom - 1 - textDrawableHeight) / 2;
        }

        // Add window rectangle
        textLeft += pWindowRect->Left;
        textTop += pWindowRect->Top;

        UINT32 dirtyLeft = textLeft;
        UINT32 dirtyRight = dirtyLeft + textDrawableWidth;
        UINT32 dirtyTop = textTop;
        UINT32 dirtyBottom = dirtyTop + textDrawableHeight;

        for(UINT32 i = 0; i < textLength; i++)
        {
            CHAR letter = text[i];

            if (letter < pFont->dfFirstChar || letter > pFont->dfLastChar)
            {
                letter = pFont->dfDefaultChar;
            }

            letter -= pFont->dfFirstChar;

            UINT32 letterOffset = ((UINT32) letter) * 4; // 4 bytes for Win2.0 fonts / 6 bytes for Win3.0 fonts
            letterOffset += LETTER_DESCRIPTIONS_OFFSET;  // LETTER_DESCRIPTIONS_OFFSET_EXTRA needs to be added for Win3.0 fonts

            PUINT8 pLetterDescriptor = ((PUINT8) pFont) + letterOffset;
            UINT32 letterWidth = ((PUINT16)pLetterDescriptor)[0];
            UINT32 offsetToGlyph = ((PUINT16)pLetterDescriptor)[1];

            PUINT8 pGlyph = ((PUINT8) pFont) + offsetToGlyph;

            for(UINT32 x = 0; x <= ((letterWidth - 1) / 8); x++)
            {
                for(UINT32 y = 0; y < pFont->dfPixHeight; y++)
                {
                    UINT8 glyphByte = pGlyph[0];

                    for(UINT32 d = 0; d < 8 ; d++)
                    {
                      if (glyphByte >> (7 - d) & 1 > 0)
                      {
                        INT32 pixelX = textLeft + textWidth + (x * 8) + d;
                        INT32 pixelY = textTop + y;

                        if (pixelX < pWindowRect->Right && pixelY < pWindowRect->Bottom)
                        {
                            GiDrawPixelCore(pixelX, pixelY, color);

                            if (flags & DRAW_TEXT_FLAG_BOLD)
                            {
                                GiDrawPixelCore(pixelX + 1, pixelY, color);
                            }
                        }
                      }
                    }

                    pGlyph++;
                }
            }

            textWidth += letterWidth;

            if (flags & DRAW_TEXT_FLAG_BOLD)
            {
                // Add extra kerning (space between letters)
                textWidth++;
            }
        }

        // Mark dirty sections
        //GiMarkDirty(dirtyLeft, dirtyTop, dirtyRight, dirtyBottom);
    }

    MemoryUnlock(hFont);
    MemoryUnlock(hDeviceContext);
}

COLOR GetBackgroundColor(HDEVICE_CONTEXT hDeviceContext)
{
    COLOR color;

    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);

    color = pDeviceContext->BackgroundColor;

    MemoryUnlock(hDeviceContext);

    return color;
}


COLOR GetForegroundColor(HDEVICE_CONTEXT hDeviceContext)
{
    COLOR color;

    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);

    color = pDeviceContext->ForegroundColor;

    MemoryUnlock(hDeviceContext);

    return color;
}

VOID SetBackgroundColor(HDEVICE_CONTEXT hDeviceContext, COLOR color)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);

    pDeviceContext->BackgroundColor = color;

    MemoryUnlock(hDeviceContext);
}

VOID SetForegroundColor(HDEVICE_CONTEXT hDeviceContext, COLOR color)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);

    if (pDeviceContext == NULL)
    {
        return;
    }

    pDeviceContext->ForegroundColor = color;

    MemoryUnlock(hDeviceContext);
}

VOID SetTransparentColor(HDEVICE_CONTEXT hDeviceContext, COLOR color)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);

    if (pDeviceContext == NULL)
    {
        return;
    }

    pDeviceContext->TransparentColor = color;

    MemoryUnlock(hDeviceContext);
}

VOID SetTextColor(HDEVICE_CONTEXT hDeviceContext, COLOR color)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);

    if (pDeviceContext == NULL)
    {
        return;
    }

    pDeviceContext->TextColor = color;

    MemoryUnlock(hDeviceContext);
}

HBITMAP LoadBitmapFromMemory(PVOID pBitmapData)
{
    // Validate file header
    PBITMAPFILEHEADER pFileHeader = (PBITMAPFILEHEADER) pBitmapData;

    if (pFileHeader->Signature != BITMAP_SIGNATURE)
    {
        // SetLastError(STATUS_ERROR_UNRECOGNIZED_FORMAT);
        return (HBITMAP) NULL;
    }

    if (pFileHeader->Reserved != 0)
    {
        return (HBITMAP) NULL;
    }

    if (pFileHeader->Size == 0)
    {
        return (HBITMAP) NULL;
    }

    if (pFileHeader->BitsOffset == 0)
    {
        return (HBITMAP) NULL;
    }

    // Validate info header
    PBITMAPINFOHEADER pInfoHeader = (PBITMAPINFOHEADER) (pBitmapData + BITMAPFILEHEADER_SIZE);

    if (pInfoHeader->HeaderSize != BITMAPINFOHEADER_SIZE)
    {
        return (HBITMAP) NULL;
    }

    if (pInfoHeader->NumberOfBitsPerPixel != 24)
    {
        return (HBITMAP) NULL;
    }

    // Get needed buffer size
    UINT32 bufferSize = pFileHeader->Size - BITMAPFILEHEADER_SIZE;

    // Allocate buffer
    HANDLE hBitmap = MemoryAllocate(bufferSize, MEMORY_FLAG_ZERO_INIT, (PCHAR) &"Bitmap");

    if (hBitmap == (HANDLE) NULL)
    {
        return (HBITMAP) NULL;
    }

    // Lock buffer
    PBITMAP pBitmap = (PBITMAP) MemoryLock(hBitmap);

    if (pBitmap == (PBITMAP) NULL)
    {
        MemoryFree(hBitmap);
        return (HBITMAP) NULL;
    }

    // Copy bitmap data to new location
    strncpy((PCHAR) pBitmap, (PCHAR) pInfoHeader, bufferSize);

    // Unlock memory
    MemoryUnlock(hBitmap);

    return (HBITMAP) hBitmap;
}

HBITMAP CreateCompatibleBitmap(HBITMAP hBitmap)
{
    // Validation
    PBITMAP pBitmap = (PBITMAP) MemoryLock(hBitmap);

    if (pBitmap == (PBITMAP) NULL)
    {
        // SetLastError(STATUS_ERROR_FAILED_TO_LOCK_OBJECT)

        return (HBITMAP) NULL;
    }

    // Check format of source bitmap
    PBITMAPINFOHEADER pInfoHeader = (PBITMAPINFOHEADER) pBitmap;
    HBITMAP hCompatibleBitmap = (HBITMAP) NULL;

    // Test
    // HICON hIcon = LoadIconFromMemory(pFloppyIcon);
    // PICON pIcon = (PICON) MemoryLock(hIcon);
    // HBITMAP hNewBitmap = CreateCompatibleBitmap(pIcon->hBitmapXorMask);

    if (pInfoHeader->NumberOfBitsPerPixel == 1)
    {
        hCompatibleBitmap = GiCreateCompatibleBitmap1bit(pInfoHeader);
        //hCompatibleBitmap = hNewBitmap;
    }
    else if (pInfoHeader->NumberOfBitsPerPixel == 4)
    {
        hCompatibleBitmap = GiCreateCompatibleBitmap4bit(pInfoHeader);
    }
    else if (pInfoHeader->NumberOfBitsPerPixel == 8)
    {
        hCompatibleBitmap = GiCreateCompatibleBitmap8bit(pInfoHeader);
    }
    else if (pInfoHeader->NumberOfBitsPerPixel == 24)
    {
        hCompatibleBitmap = GiCreateCompatibleBitmap24bit(pInfoHeader);
    }
    else if (pInfoHeader->NumberOfBitsPerPixel == 32)
    {
        hCompatibleBitmap = hBitmap;
    }

    MemoryUnlock(hBitmap);

    return hCompatibleBitmap;
}

HBITMAP GiCreateCompatibleBitmap1bit(PBITMAPINFOHEADER pInfoHeader)
{
    // Allocate buffer for new bitmap
    INT32 compatibleBitmapWidth = pInfoHeader->Width;
    INT32 compatibleBitmapHeight = pInfoHeader->Height;
    INT32 compatibleBitmapWidthInBytes = compatibleBitmapWidth * 4;
    UINT32 compatibleBitmapImageSize = compatibleBitmapWidthInBytes * compatibleBitmapHeight;
    UINT32 compatibleBitmapBufferSize = compatibleBitmapImageSize + BITMAPINFOHEADER_SIZE;

    HBITMAP hCompatibleBitmap = (HBITMAP) MemoryAllocate(compatibleBitmapBufferSize, MEMORY_FLAG_ZERO_INIT, (PCHAR) &"Bitmap");

    if (hCompatibleBitmap == (HBITMAP) NULL)
    {
        // SetLastError(STATUS_ERROR_FAILED_TO_ALLOCATE_BUFFER)

        return (HBITMAP) NULL;
    }

    PBITMAP pCompatibleBitmap = (PBITMAP) MemoryLock(hCompatibleBitmap);

    if (pCompatibleBitmap == (PBITMAP) NULL)
    {
        MemoryFree(hCompatibleBitmap);
        // SetLastError(STATUS_ERROR_FAILED_TO_LOCK_OBJECT)

        return (HBITMAP) NULL;
    }

    // Fill header info
    PBITMAPINFOHEADER pCompatibleInfoHeader = (PBITMAPINFOHEADER) pCompatibleBitmap;

    pCompatibleInfoHeader[0] = pInfoHeader[0];
    pCompatibleInfoHeader->NumberOfPlanes = 0;
    pCompatibleInfoHeader->NumberOfBitsPerPixel = 32;
    pCompatibleInfoHeader->ImageSize = compatibleBitmapImageSize;
    pCompatibleInfoHeader->NumberOfUsedColors = 0;
    pCompatibleInfoHeader->NumberOfImportantColors = 0;

    // Copy and convert pixel data

    PRGBQUAD pPalette = (PRGBQUAD) ((PBYTE) pInfoHeader + pInfoHeader->HeaderSize);
    PBYTE pBitmapBytes = (PBYTE) pInfoHeader + pInfoHeader->HeaderSize + pInfoHeader->NumberOfUsedColors * sizeof(RGBQUAD);
    INT32 width = pInfoHeader->Width;
    INT32 widthInBytes = width / 8;
    INT32 padding = (((widthInBytes + 3) / 4) * 4) - widthInBytes;
    INT32 height = pInfoHeader->Height;
    PUINT32 pCompatibleBitmapBytes = (PUINT32) ((PBYTE) pCompatibleInfoHeader + pCompatibleInfoHeader->HeaderSize);

    for(INT32 y = height; y > 0; y--)
    {
        for(INT32 x = 0; x < width; x++)
        {
            COLOR color;
            RGBQUAD rgbQuad;
            UINT32 colorIndex = 0;
            INT32 d = x % 8;
            colorIndex = (UINT32) (pBitmapBytes[x / 8] >> (7 - d) & 1);

            rgbQuad = pPalette[colorIndex];
            color = 0xFF000000;
            color += (((UINT32) rgbQuad.Red) << 16);
            color += (((UINT32) rgbQuad.Green) << 8);
            color += ((UINT32) rgbQuad.Blue);

            // if (colorIndex == 0)
            // {
            //     color = 0xFFFFFFFF;
            //     //color = 0xFF000000;
            // }
            // else
            // {
            //     color = 0xFF000000;
            //     //color = 0xFFFFFFFF;
            // }

            pCompatibleBitmapBytes[x] = color;
        }

        pBitmapBytes += widthInBytes + padding;
        pCompatibleBitmapBytes += width;
    }

    MemoryUnlock(hCompatibleBitmap);

    return hCompatibleBitmap;
}

HBITMAP GiCreateCompatibleBitmap4bit(PBITMAPINFOHEADER pInfoHeader)
{
    // Allocate buffer for new bitmap
    INT32 compatibleBitmapWidth = pInfoHeader->Width;
    INT32 compatibleBitmapHeight = pInfoHeader->Height;
    INT32 compatibleBitmapWidthInBytes = compatibleBitmapWidth * 4;
    UINT32 compatibleBitmapImageSize = compatibleBitmapWidthInBytes * compatibleBitmapHeight;
    UINT32 compatibleBitmapBufferSize = compatibleBitmapImageSize + BITMAPINFOHEADER_SIZE;

    HBITMAP hCompatibleBitmap = (HBITMAP) MemoryAllocate(compatibleBitmapBufferSize, MEMORY_FLAG_ZERO_INIT, (PCHAR) &"Bitmap");

    if (hCompatibleBitmap == (HBITMAP) NULL)
    {
        // SetLastError(STATUS_ERROR_FAILED_TO_ALLOCATE_BUFFER)

        return (HBITMAP) NULL;
    }

    PBITMAP pCompatibleBitmap = (PBITMAP) MemoryLock(hCompatibleBitmap);

    if (pCompatibleBitmap == (PBITMAP) NULL)
    {
        MemoryFree(hCompatibleBitmap);
        // SetLastError(STATUS_ERROR_FAILED_TO_LOCK_OBJECT)

        return (HBITMAP) NULL;
    }

    // Fill header info
    PBITMAPINFOHEADER pCompatibleInfoHeader = (PBITMAPINFOHEADER) pCompatibleBitmap;

    pCompatibleInfoHeader[0] = pInfoHeader[0];
    pCompatibleInfoHeader->NumberOfPlanes = 0;
    pCompatibleInfoHeader->NumberOfBitsPerPixel = 32;
    pCompatibleInfoHeader->ImageSize = compatibleBitmapImageSize;
    pCompatibleInfoHeader->NumberOfUsedColors = 0;
    pCompatibleInfoHeader->NumberOfImportantColors = 0;

    // Copy and convert pixel data

    PRGBQUAD pPalette = (PRGBQUAD) ((PBYTE) pInfoHeader + pInfoHeader->HeaderSize);
    PBYTE pBitmapBytes = (PBYTE) pInfoHeader + pInfoHeader->HeaderSize + pInfoHeader->NumberOfUsedColors * sizeof(RGBQUAD);
    INT32 width = pInfoHeader->Width;
    INT32 widthInBytes = width / 2;
    INT32 padding = (((widthInBytes + 3) / 4) * 4) - widthInBytes;
    INT32 height = pInfoHeader->Height;
    PUINT32 pCompatibleBitmapBytes = (PUINT32) ((PBYTE) pCompatibleInfoHeader + pCompatibleInfoHeader->HeaderSize);

    for(INT32 y = height; y > 0; y--)
    {
        for(INT32 x = 0; x < width; x++)
        {
            COLOR color;
            RGBQUAD rgbQuad;
            UINT32 colorIndex = 0;

            if (x % 2 == 1)
            {
                colorIndex = (UINT32) pBitmapBytes[x / 2] & 0xF;
            }
            else
            {
                colorIndex = (UINT32) pBitmapBytes[x / 2] >> 4;
            }

            rgbQuad = pPalette[colorIndex];

            color = 0xFF000000;
            color += ((UINT32) rgbQuad.Red << 16);
            color += ((UINT32) rgbQuad.Green << 8);
            color += ((UINT32) rgbQuad.Blue);

            pCompatibleBitmapBytes[x] = color;
        }

        pBitmapBytes += widthInBytes + padding;
        pCompatibleBitmapBytes += width;
    }

    MemoryUnlock(hCompatibleBitmap);

    return hCompatibleBitmap;
}

HBITMAP GiCreateCompatibleBitmap8bit(PBITMAPINFOHEADER pInfoHeader)
{

}

HBITMAP GiCreateCompatibleBitmap24bit(PBITMAPINFOHEADER pInfoHeader)
{
    // Allocate buffer for new bitmap
    INT32 compatibleBitmapWidth = pInfoHeader->Width;
    INT32 compatibleBitmapHeight = pInfoHeader->Height;
    INT32 compatibleBitmapWidthInBytes = compatibleBitmapWidth * 4;
    UINT32 compatibleBitmapImageSize = compatibleBitmapWidthInBytes * compatibleBitmapHeight;
    UINT32 compatibleBitmapBufferSize = compatibleBitmapImageSize + BITMAPINFOHEADER_SIZE;

    HBITMAP hCompatibleBitmap = (HBITMAP) MemoryAllocate(compatibleBitmapBufferSize, MEMORY_FLAG_ZERO_INIT, (PCHAR) &"Bitmap");

    if (hCompatibleBitmap == (HBITMAP) NULL)
    {
        // SetLastError(STATUS_ERROR_FAILED_TO_ALLOCATE_BUFFER)

        return (HBITMAP) NULL;
    }

    PBITMAP pCompatibleBitmap = (PBITMAP) MemoryLock(hCompatibleBitmap);

    if (pCompatibleBitmap == (PBITMAP) NULL)
    {
        MemoryFree(hCompatibleBitmap);
        // SetLastError(STATUS_ERROR_FAILED_TO_LOCK_OBJECT)

        return (HBITMAP) NULL;
    }

    // Fill header info
    PBITMAPINFOHEADER pCompatibleInfoHeader = (PBITMAPINFOHEADER) pCompatibleBitmap;

    pCompatibleInfoHeader[0] = pInfoHeader[0];
    pCompatibleInfoHeader->NumberOfBitsPerPixel = 32;
    pCompatibleInfoHeader->ImageSize = compatibleBitmapImageSize;

    // Copy and convert pixel data

    PBYTE pBitmapBytes = (PBYTE) pInfoHeader + pInfoHeader->HeaderSize;
    PUINT32 pCompatibleBitmapBytes = (PUINT32) ((PBYTE) pCompatibleInfoHeader + pInfoHeader->HeaderSize);
    INT32 width = pInfoHeader->Width;
    INT32 widthInBytes = width * 3;
    INT32 padding = (((widthInBytes + 3) / 4) * 4) - widthInBytes;
    INT32 height = pInfoHeader->Height;

    for(INT32 y = height; y > 0; y--)
    {
        for(INT32 x = 0; x < width; x++)
        {
            COLOR color = 0xFF000000;
            // color += pBitmapBytes[x*3] << 16;
            // color += pBitmapBytes[x*3 + 1] << 8;
            // color += pBitmapBytes[x*3 + 2];

            color += pBitmapBytes[x*3];
            color += pBitmapBytes[x*3 + 1] << 8;
            color += pBitmapBytes[x*3 + 2] << 16;


            pCompatibleBitmapBytes[x] = color;
        }

        pBitmapBytes += width * 3 + padding;
        pCompatibleBitmapBytes += width;
    }

    MemoryUnlock(hCompatibleBitmap);

    return hCompatibleBitmap;
}

BOOL DrawBitmap(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y, HBITMAP hBitmap)
{
    // Convert bitmap if needs be
    hBitmap = CreateCompatibleBitmap(hBitmap);

    // Test
    // HICON hIcon = LoadIconFromMemory(pFloppyIcon);
    // PICON pIcon = (PICON) MemoryLock(hIcon);
    // hBitmap = CreateCompatibleBitmap(pIcon->hBitmapXorMask);

    // Lock device context

    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);
    PRECTANGLE pWindowRect = pDeviceContext->pWindowRectangle;
    PREGION pClippingRegion = pDeviceContext->pClippingRegion;
    BOOL isCompatibleBitmap = TRUE;

    if (pDeviceContext == NULL)
    {
        // SetLastError(STATUS_ERROR_FAILED_TO_LOCK_OBJECT)

        return FALSE;
    }

    // Lock bitmap in memory
    PBITMAP pBitmap = (PBITMAP) MemoryLock(hBitmap);

    if (pBitmap == (PBITMAP) NULL)
    {
        MemoryUnlock(hDeviceContext);
        // SetLastError(STATUS_ERROR_FAILED_TO_LOCK_OBJECT)

        return FALSE;
    }

    // Add window rectangle
    INT32 bitmapX = x + pWindowRect->Left;
    INT32 bitmapY = y + pWindowRect->Top;

    // Draw 32bit compatible bitmap
    PBITMAPINFOHEADER pInfoHeader = (PBITMAPINFOHEADER) pBitmap;
    PUINT32 pBitmapBytes = (PUINT32) ((PBYTE) pInfoHeader + pInfoHeader->HeaderSize);
    INT32 width = pInfoHeader->Width;
    INT32 height = pInfoHeader->Height;

    for(INT32 j = height - 1; j >= 0; j--)
    {
        for(INT32 i = 0; i < width; i++)
        {
            COLOR color = pBitmapBytes[i];

            GiDrawPixelCore(bitmapX + i, bitmapY + j, GiGetPaletteIndex(pGiDevicePalette, color));
            //GiDrawPixelCore(bitmapX + i, bitmapY + j, 0);
        }

        pBitmapBytes += width;
    }

    // Release resources
    MemoryUnlock(hBitmap);

    if (!isCompatibleBitmap)
    {
        MemoryUnlock(hBitmap);
        MemoryFree(hBitmap);
    }

    MemoryUnlock(hDeviceContext);

    return TRUE;
}

VOID GetBitmapInfo(HBITMAP hBitmap, OUT PBITMAPINFOHEADER pBitmapInfoHeader)
{
    // Lock bitmap in memory
    PBITMAP pBitmap = (PBITMAP) MemoryLock(hBitmap);

    if (pBitmap == (PBITMAP) NULL)
    {
        // SetLastError(STATUS_ERROR_FAILED_TO_LOCK_OBJECT)

        return;
    }

    // Copy info header to target
    PBITMAPINFOHEADER pInfoHeader = (PBITMAPINFOHEADER) pBitmap;

    pBitmapInfoHeader[0] = pInfoHeader[0];
}

HBITMAP CreateBitmap(UINT32 width, UINT32 height, UINT32 numberOfPlanes, UINT32 numberOfBitsPerPixel, PBYTE pColorPalette, PBYTE pBitmapBits)
{
    // Calculate the needed buffer
    UINT32 colorPaletteSize = 0;

    if (numberOfBitsPerPixel == 1)
    {
        colorPaletteSize = 2;
    }
    else if (numberOfBitsPerPixel == 4)
    {
        colorPaletteSize = 16;
    }
    else if (numberOfBitsPerPixel == 8)
    {
        colorPaletteSize = 256;
    }

    UINT32 widthInBytes = (width * numberOfBitsPerPixel) / 8;
    UINT32 padding = (((widthInBytes + 3) / 4) * 4) - widthInBytes;
    UINT32 bitmapBytesSize = (widthInBytes + padding) * height;

    UINT32 bufferSize = BITMAPINFOHEADER_SIZE + (colorPaletteSize * sizeof(RGBQUAD)) + bitmapBytesSize;

    // Allocate buffer
    HBITMAP hNewBitmap = (HBITMAP) MemoryAllocate(bufferSize, MEMORY_FLAG_ZERO_INIT, (PCHAR) &"Bitmap");

    if (hNewBitmap == (HBITMAP) NULL)
    {
        return (HBITMAP) NULL;
    }

    PBITMAP pNewBitmap = (PBITMAP) MemoryLock(hNewBitmap);

    if (pNewBitmap == (PBITMAP) NULL)
    {
        MemoryFree(hNewBitmap);
        return (HBITMAP) NULL;
    }

    // Fill in the Info Header structure
    PBITMAPINFOHEADER pNewInfoHeader = (PBITMAPINFOHEADER) pNewBitmap;

    pNewInfoHeader->HeaderSize = BITMAPINFOHEADER_SIZE;
    pNewInfoHeader->Width = width;
    pNewInfoHeader->Height = height;
    pNewInfoHeader->NumberOfPlanes = numberOfPlanes;
    pNewInfoHeader->NumberOfBitsPerPixel = numberOfBitsPerPixel;
    pNewInfoHeader->Compression = 0;
    pNewInfoHeader->ImageSize = bitmapBytesSize;
    pNewInfoHeader->XPixelsPerMeter = 0;
    pNewInfoHeader->YPixelsPerMeter = 0;
    pNewInfoHeader->NumberOfUsedColors = colorPaletteSize;
    pNewInfoHeader->NumberOfImportantColors = 0;

    // Copy color palatte
    PBYTE pNewColorPalette = (PBYTE) pNewBitmap + BITMAPINFOHEADER_SIZE;

    if (colorPaletteSize != 0)
    {
        if (pColorPalette != (PBYTE) NULL)
        {
            strncpy((char*) pNewColorPalette, (char*) pColorPalette, (colorPaletteSize * sizeof(RGBQUAD)));
        }
        else
        {
            memset((char*) pNewColorPalette, 0, colorPaletteSize * sizeof(RGBQUAD));

            PRGBQUAD pColorEntry = (PRGBQUAD) pNewColorPalette;
            pColorEntry[0].Red = 0;
            pColorEntry[0].Green = 0;
            pColorEntry[0].Blue = 0;
            pColorEntry[0].Reserved = 0;

            pColorEntry[1].Red = 0xFF;
            pColorEntry[1].Green = 0xFF;
            pColorEntry[1].Blue = 0xFF;
            pColorEntry[1].Reserved = 0xFF;
        }
    }

    // Copy bitmap bits
    PBYTE pNewBitmapBits = (PBYTE) pNewColorPalette + (colorPaletteSize * sizeof(RGBQUAD));
    strncpy((char*) pNewBitmapBits, (char*) pBitmapBits, bitmapBytesSize);

    // Unlock memory
    MemoryUnlock(hNewBitmap);

    return hNewBitmap;
}

HICON LoadIconFromMemory(PVOID pIconData)
{
    // Validate icon directory header
    PICONDIRECTORY pIconDirectory = (PICONDIRECTORY) pIconData;

    if (pIconDirectory->Reserved != 0)
    {
        // SetLastError(STATUS_ERROR_UNRECOGNIZED_FORMAT);
        return (HICON) 1;
    }

    // if (pIconDirectory->ImageType != 1)
    // {
    //     // SetLastError(STATUS_ERROR_UNRECOGNIZED_FORMAT);
    //     return (HICON) 2;
    // }

    if (pIconDirectory->ImageCount != 1)
    {
        // SetLastError(STATUS_ERROR_UNRECOGNIZED_FORMAT);
        return (HICON) 3;
    }

    // Validate icon entry header
    PICONDIRECTORYENTRY pIconEntry = (PICONDIRECTORYENTRY) ((PBYTE) pIconDirectory + ICONDIRECTORY_SIZE);

    if (pIconEntry->Length == 0)
    {
        return (HICON) 4;
    }

    if (pIconEntry->Offset == 0)
    {
        return (HICON) 5;
    }

    // Allocate buffer for Icon structure
    HICON hIcon = (HICON) MemoryAllocate(sizeof(ICON), MEMORY_FLAG_ZERO_INIT, (PCHAR) &"Icon");

    if (hIcon == (HICON) NULL)
    {
        return (HICON) 7;
    }

    PBITMAPINFOHEADER pXorMaskInfoHeader = (PBITMAPINFOHEADER) ((PBYTE) pIconDirectory + pIconEntry->Offset);

    // Lock buffer
    PICON pIcon = (PICON) MemoryLock(hIcon);

    if (pIcon == (PICON) NULL)
    {
        MemoryFree(hIcon);
        return (HICON) NULL;
    }

    // Fix common mistakes in IconEntry and BitmapInfoHeader
    if (pIconEntry->NumberOfBitsPerPixel == 0)
    {
        pIconEntry->NumberOfBitsPerPixel = pXorMaskInfoHeader->NumberOfBitsPerPixel;
    }

    if (pXorMaskInfoHeader->NumberOfUsedColors == 0)
    {
        pXorMaskInfoHeader->NumberOfUsedColors = 16;
    }

    // Fill in the Icon structure
    pIcon->Width = pIconEntry->Width;
    pIcon->Height = pIconEntry->Height;
    pIcon->NumberOfColorPlanes = pIconEntry->NumberOfColorPlanes;
    pIcon->NumberOfBitsPerPixel = pIconEntry->NumberOfBitsPerPixel; // UNRELIABLE! Often is set to 0.
    pIcon->hBitmapAndMask = (HBITMAP) NULL;
    pIcon->hBitmapXorMask = (HBITMAP) NULL;

    // if (pIcon->NumberOfColorPlanes == 0)
    // {
    //     pIcon->NumberOfColorPlanes = 1;
    // }

    // Create bitmap out of the XOR mask
    PBYTE pXorMaskColorPalette = (PBYTE) pXorMaskInfoHeader + pXorMaskInfoHeader->HeaderSize;
    PBYTE pXorMaskBitmapBits = pXorMaskColorPalette + (pXorMaskInfoHeader->NumberOfUsedColors * sizeof(RGBQUAD));
    UINT32 xorMaskWidthInBytes = (pIcon->Width * pIcon->NumberOfBitsPerPixel) / 8;
    UINT32 xorMaskPadding = (((xorMaskWidthInBytes + 3) / 4) * 4) - xorMaskWidthInBytes;

    pIcon->hBitmapXorMask = CreateBitmap(pIcon->Width, pIcon->Height, pIcon->NumberOfColorPlanes, pIcon->NumberOfBitsPerPixel, pXorMaskColorPalette, pXorMaskBitmapBits);

    // Create bitmap out of the AND mask
    PBYTE pAndMaskBitmapBits = (PBYTE) pXorMaskBitmapBits + ((xorMaskWidthInBytes + xorMaskPadding) * pIcon->Height);

    pIcon->hBitmapAndMask = CreateBitmap(pIcon->Width, pIcon->Height, 1, 1, NULL, pAndMaskBitmapBits);

    // Unlock memory
    MemoryUnlock(hIcon);

    return (HICON) hIcon;
}

BOOL DrawIcon(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y, HICON hIcon)
{
    // Lock device context
    RECTANGLE windowRect;
    RECTANGLE deviceRect;
    PDEVICE_CONTEXT pDeviceContext;
    PRECTANGLE pWindowRect = &windowRect;
    PRECTANGLE pDeviceRect = &deviceRect;
    PREGION pClippingRegion;

    pDeviceContext = MemoryLock(hDeviceContext);
    pWindowRect = pDeviceContext->pWindowRectangle;

    GetScreenRectangle(hDesktopWindow, pDeviceRect);

    // Lock bitmap in memory
    PICON pIcon = (PICON) MemoryLock(hIcon);

    if (pIcon == (PICON) NULL)
    {
        MemoryUnlock(hDeviceContext);
        // SetLastError(STATUS_ERROR_FAILED_TO_LOCK_OBJECT)

        return FALSE;
    }

    //HBITMAP hCompatibleAndMask = CreateCompatibleBitmap(pIcon->hBitmapAndMask);
    //HBITMAP hCompatibleXorMask = CreateCompatibleBitmap(pIcon->hBitmapXorMask);

    HBITMAP hCompatibleAndMask = pIcon->hBitmapAndMask;
    HBITMAP hCompatibleXorMask = pIcon->hBitmapXorMask;

    // Lock bitmaps
    PBITMAP pCompatibleAndMask = (PBITMAP) MemoryLock(hCompatibleAndMask);

    if (pCompatibleAndMask == (PBITMAP) NULL)
    {
        MemoryUnlock(hIcon);
        MemoryUnlock(hDeviceContext);

        return FALSE;
    }

    PBITMAP pCompatibleXorMask = (PBITMAP) MemoryLock(hCompatibleXorMask);

    if (pCompatibleXorMask == (PBITMAP) NULL)
    {
        MemoryUnlock(hIcon);
        MemoryUnlock(hCompatibleAndMask);
        MemoryUnlock(hDeviceContext);

        return FALSE;
    }

    // Add window rectangle
    INT32 iconX = x + pWindowRect->Left;
    INT32 iconY = y + pWindowRect->Top;

    if (iconX < pDeviceRect->Left - 32 || iconX > pDeviceRect->Right)
    {
        return FALSE;
    }
    if (iconY < pDeviceRect->Top -32 || iconY > pDeviceRect->Bottom)
    {
        return FALSE;
    }

    // Draw 32bit compatible bitmap
    INT32 width = pIcon->Width;
    INT32 height = pIcon->Height;

    PBITMAPINFOHEADER pCompatibleAndInfoHeader = (PBITMAPINFOHEADER) pCompatibleAndMask;
    PBITMAPINFOHEADER pCompatibleXorInfoHeader = (PBITMAPINFOHEADER) pCompatibleXorMask;

    PUINT32 pIconAndBytes = (PUINT32) ((PBYTE) pCompatibleAndInfoHeader + pCompatibleAndInfoHeader->HeaderSize);
    PUINT32 pIconXorBytes = (PUINT32) ((PBYTE) pCompatibleXorInfoHeader + pCompatibleXorInfoHeader->HeaderSize);

    UINT32 widthInBytes = width * 4;
    UINT32 padding = (((widthInBytes + 3) / 4) * 4) - widthInBytes;

    //SetDrawMode(hDeviceContext, DRAW_MODE_PEN_DOT);

    for(INT32 j = height - 1; j >= 0; j--)
    {
        for(INT32 i = 0; i < width; i++)
        {
            if (pIconAndBytes[i] == (COLOR) 0xFF000000)
            {
                //GiDrawPixelCore(iconX + i, iconY + j, 0xFF000000);
                GiDrawPixelCore(iconX + i, iconY + j, GiGetPaletteIndex(pGiDevicePalette, pIconXorBytes[i]));
            }
        }

        pIconAndBytes += width;
        pIconXorBytes += width;
    }

    //SetDrawMode(hDeviceContext, DRAW_MODE_NORMAL);

    // Release resources
    MemoryUnlock(hCompatibleAndMask);
    MemoryUnlock(hCompatibleXorMask);

    if (hCompatibleAndMask != pIcon->hBitmapAndMask)
    {
        MemoryFree(hCompatibleAndMask);
    }

    if (hCompatibleXorMask != pIcon->hBitmapXorMask)
    {
        MemoryFree(hCompatibleXorMask);
    }

    MemoryUnlock(hIcon);

    if (hDeviceContext != (HDEVICE_CONTEXT) NULL)
    {
        MemoryUnlock(hDeviceContext);
    }


    return TRUE;
}

// MOUSE
VOID GiDrawMouseCursor(INT32 x, INT32 y, HICON hIcon)
{
    // Lock bitmap in memory
    PICON pIcon = (PICON) MemoryLock(hIcon);

    if (pIcon == (PICON) NULL)
    {
        return;
    }

    HBITMAP hCompatibleAndMask = pIcon->hBitmapAndMask;
    HBITMAP hCompatibleXorMask = pIcon->hBitmapXorMask;

    // Lock bitmaps
    PBITMAP pCompatibleAndMask = (PBITMAP) MemoryLock(hCompatibleAndMask);

    if (pCompatibleAndMask == (PBITMAP) NULL)
    {
        return;
    }

    PBITMAP pCompatibleXorMask = (PBITMAP) MemoryLock(hCompatibleXorMask);

    if (pCompatibleXorMask == (PBITMAP) NULL)
    {
        MemoryUnlock(hCompatibleAndMask);
        MemoryUnlock(hIcon);

        return;
    }

    // Add window rectangle
    INT32 iconX = x;
    INT32 iconY = y;

    // Draw 32bit compatible bitmap
    INT32 width = pIcon->Width;
    INT32 height = pIcon->Height;

    PBITMAPINFOHEADER pCompatibleAndInfoHeader = (PBITMAPINFOHEADER) pCompatibleAndMask;
    PBITMAPINFOHEADER pCompatibleXorInfoHeader = (PBITMAPINFOHEADER) pCompatibleXorMask;

    PUINT32 pIconAndBytes = (PUINT32) ((PBYTE) pCompatibleAndInfoHeader + pCompatibleAndInfoHeader->HeaderSize);
    PUINT32 pIconXorBytes = (PUINT32) ((PBYTE) pCompatibleXorInfoHeader + pCompatibleXorInfoHeader->HeaderSize);

    UINT32 widthInBytes = width * 4;
    UINT32 padding = (((widthInBytes + 3) / 4) * 4) - widthInBytes;

    UINT8 index = GiGetPaletteIndex(pGiDevicePalette, 0xFFFFFF);

    for(INT32 j = height - 1; j >= 0; j--)
    {
        for(INT32 i = 0; i < width; i++)
        {
            if (pIconAndBytes[i] == (COLOR) 0xFF000000)
            {
                GiDrawPixelCore(iconX + i, iconY + j, 0xFF000000);
            }
            if (pIconXorBytes[i] != (COLOR) 0xFF000000)
            {
                GiDrawPixelCore(iconX + i, iconY + j, index);
            }

        }

        pIconAndBytes += width;
        pIconXorBytes += width;
    }

    // Release resources
    MemoryUnlock(hCompatibleAndMask);
    MemoryUnlock(hCompatibleXorMask);
    MemoryUnlock(hIcon);
}

VOID UpdateMouse(MOUSE_STATE mouseState)
{
    GiMouseIsLeftButtonDown = mouseState.IsLeftButtonDown;
    GiMouseIsMiddleButtonDown = mouseState.IsMiddleButtonDown;
    GiMouseIsRightButtonDown = mouseState.IsRightButtonDown;
    GiMouseX += mouseState.RelativeX;
    GiMouseY -= mouseState.RelativeY;

    if (GiMouseX < 0) GiMouseX = 0;
    if (GiMouseY < 0) GiMouseY = 0;
    if (GiMouseX > GiScreenWidth - 1) GiMouseX = GiScreenWidth - 1;
    if (GiMouseY > GiScreenHeight - 1) GiMouseY = GiScreenHeight - 1;


    if (!GiDisableMouseUpdate)
    {
        POINT mousePosition;
        mousePosition.X = GiMouseX;
        mousePosition.Y = GiMouseY;

        GiDisableMouseUpdate = TRUE;
        DeviceMoveMouse(&mousePosition);
        GiDisableMouseUpdate = FALSE;
    }

    GiMouseIsLeftButtonDownOld = GiMouseIsLeftButtonDown;
    GiMouseIsMiddleButtonDownOld = GiMouseIsMiddleButtonDown;
    GiMouseIsRightButtonDownOld = GiMouseIsRightButtonDown;
}

VOID ShowMouse()
{
    GiDisableMouseUpdate = TRUE;
    DeviceShowMouse();
    GiDisableMouseUpdate = FALSE;
}

VOID HideMouse()
{
    GiDisableMouseUpdate = TRUE;
    DeviceHideMouse();
    GiDisableMouseUpdate = FALSE;
}

VOID SetMousePosition(INT32 mouseX, INT32 mouseY)
{
    POINT mousePosition;
    mousePosition.X = mouseX;
    mousePosition.Y = mouseY;

    DeviceSetMousePosition(&mousePosition);
    GiMouseX = mouseX;
    GiMouseY = mouseY;
}

VOID GetMousePosition(OUT PINT32 mouseX, OUT PINT32 mouseY)
{
    *mouseX = GiMouseX;
    *mouseY = GiMouseY;
}

VOID GetMouseLeftButtonState(OUT PBOOL isMouseLeftButtonDown)
{
    *isMouseLeftButtonDown = GiMouseIsLeftButtonDown;
}

// KEYBOARD
VOID CreateCaret(HWINDOW hWindow, UINT32 width, UINT32 height)
{
    PCARET pCaret;

    if (hGiSystemCaret == (HANDLE) NULL)
    {
        hGiSystemCaret = MemoryAllocate(sizeof(CARET), MEMORY_FLAG_ZERO_INIT, (PCHAR) &"SystemCaret");

        pCaret = MemoryLock(hGiSystemCaret);

        if (width < 1) width = 1;
        if (height < 1) height = 1;

        pCaret->hWindow = hWindow;
        pCaret->Width = width;
        pCaret->Height = height;
        pCaret->PositionX = 0;
        pCaret->PositionY = 0;
        pCaret->IsShown = FALSE;

        return;
    }
    
    pCaret = MemoryLock(hGiSystemCaret);

    if (pCaret->hWindow != (HWINDOW) NULL)
    {
        HideCaret(hWindow);
    }

    if (width < 1) width = 1;
    if (height < 1) height = 1;

    pCaret->hWindow = hWindow;
    pCaret->Width = width;
    pCaret->Height = height;
    pCaret->PositionX = 0;
    pCaret->PositionY = 0;
    pCaret->IsShown = FALSE;
}

VOID SetCaretPosition(INT32 x, INT32 y)
{
    if (hGiSystemCaret == (HANDLE) NULL)
    {
        return;
    }

    PCARET pCaret = MemoryLock(hGiSystemCaret);

    pCaret->PositionX = x;
    pCaret->PositionY = y;
}

VOID ShowCaret(HWINDOW hWindow)
{
    if (hGiSystemCaret == (HANDLE) NULL)
    {
        return;
    }

    PCARET pCaret = MemoryLock(hGiSystemCaret);

    if (pCaret->hWindow != hWindow)
    {
        return;
    }

    HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);

    DrawCaret(hDeviceContext, pCaret->PositionX, pCaret->PositionY, pCaret->Width, pCaret->Height);
}

VOID HideCaret(HWINDOW hWindow)
{
    if (hGiSystemCaret == (HANDLE) NULL)
    {
        return;
    }

    PCARET pCaret = MemoryLock(hGiSystemCaret);

    if (pCaret->hWindow != hWindow)
    {
        return;
    }

    HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);

    DrawCaret(hDeviceContext, pCaret->PositionX, pCaret->PositionY, pCaret->Width, pCaret->Height);
}

VOID SetCaretBlinkTime(UINT32 milliSeconds)
{

}

VOID DrawCaret(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y, UINT32 width, UINT32 height)
{
    DrawBegin(hDeviceContext);

    SetForegroundColor(hDeviceContext, 0xFFFFFF);

    DRAWMODE prevDrawMode = GetDrawMode(hDeviceContext);
    SetDrawMode(hDeviceContext, DRAW_MODE_XOR);

    for(UINT32 i = 0; i < width; i++)
    {
        DrawLineA(hDeviceContext, x + i, y, x + i, y + height);
    }    

    SetDrawMode(hDeviceContext, prevDrawMode);

    DrawEnd(hDeviceContext);
}

// --------------------------

VOID ShowVariableNameAndValue(HDEVICE_CONTEXT hDeviceContext, PCHAR pVariableName, UINT32 variableValue, UINT32 base, PRECTANGLE pWindowRectangle)
{
    SetTextColor(hDeviceContext, 0xFFFF0000);

    CHAR textBuffer[40] = { 0 };
    size_t textLength;

    textLength = strlen((const char*)pVariableName);
    strcpy(&textBuffer[0], (const char*)pVariableName);
    textBuffer[textLength] = '=';
    itoa(variableValue, &textBuffer[textLength + 1], base);

    DrawText(hDeviceContext, &textBuffer[0], pWindowRectangle, 0);
}

