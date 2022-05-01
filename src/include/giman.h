/*
  - Date: 2021-08-30
  - Author: Joel
  - Purpose: Graphical Interface Manager
*/

#ifndef __GIMAN_H
#define __GIMAN_H

typedef struct _VIDEODEVICEFUNCTIONS_VTBL
{	
    // Instead of these a SetDrawMode would be better (a bunch of VGA stuff does not need to be set for every pixel draw call)
    VOID    (*DeviceDrawPixelOverwrite)(UINT32 deviceX, UINT32 deviceY, COLOR color);
    VOID    (*DeviceDrawPixelAnd)(UINT32 deviceX, UINT32 deviceY, COLOR color);
    VOID    (*DeviceDrawPixelOr)(UINT32 deviceX, UINT32 deviceY, COLOR color);
    VOID    (*DeviceDrawPixelXor)(UINT32 deviceX, UINT32 deviceY, COLOR color);    

    VOID    (*DeviceDrawFilledRectangle)(UINT32 deviceLeft, UINT32 deviceTop, UINT32 deviceRight, UINT32 deviceBottom, COLOR);

    VOID    (*DeviceHideMouse)();
    VOID    (*DeviceShowMouse)();
    VOID    (*DeviceMoveMouse)(PPOINT pMousePosition);
    VOID    (*DeviceSetMousePosition)(PPOINT pMousePosition);
    VOID    (*DeviceGetMousePosition)(OUT PPOINT pMousePosition);
    VOID    (*DeviceSetMouseCursor)(HCURSOR hCursor);
    HCURSOR (*DeviceGetMouseCursor)();

} __attribute__((packed)) VIDEODEVICEFUNCTIONS_VTBL, *PVIDEODEVICEFUNCTIONS_VTBL;

// Init
VOID GraphicalInterfaceManagerInit();
VOID GraphicalInterfaceManagerUninit();

// Device related
UINT32 GetNumberOfDevices();
HDEVICE GetPrimaryDisplayDevice();
UINT32 EnumerateDevices(OUT PDISPLAY_DEVICE pDisplayDevice, UINT32 bufferSizeInBytes);
KSTATUS GetDeviceCapabilities(HDEVICE hDevice, OUT PDEVICE_CAPABILITIES pDeviceCapabilities);
KSTATUS GetVideoModeInformation(HDEVICE hDevice, UINT32 videoModeNumber, OUT PVIDEO_MODE_INFORMATION pVideoModeInformation);
KSTATUS GetCurrentVideoModeInformation(HDEVICE hDevice, OUT PVIDEO_MODE_INFORMATION pVideoModeInformation);
KSTATUS SetVideoMode(HDEVICE hDevice, UINT32 videoModeNumber);

// Device context related
HDEVICE_CONTEXT CreateDeviceContext(HDEVICE hDevice);
BOOL IsValidDeviceContext(HDEVICE_CONTEXT hDeviceContext);
KSTATUS DeleteDeviceContext(HDEVICE_CONTEXT hDeviceContext);
HDEVICE_CONTEXT GetDeviceContext(HWINDOW hWindow);
HDEVICE_CONTEXT GetDeviceContextNonClient(HWINDOW hWindow);
VOID ReleaseDeviceContext(HDEVICE_CONTEXT hDeviceContext);
BOOL CheckIfDeviceContextValid(HDEVICE_CONTEXT hDeviceContext);

// Graphical objects related

// Bitmap
HBITMAP CreateBitmap(UINT32 width, UINT32 height, UINT32 numberOfPlanes, UINT32 numberOfBitsPerPixel, PBYTE pColorPalette, PBYTE pBitmapBits);
HBITMAP CreateCompatibleBitmap(HBITMAP hBitmap);
VOID CreateCompatibleIcon(HICON hIcon);
BOOL DestroyBitmap(HBITMAP hBitmap);

HBITMAP LoadBitmapFromMemory(PVOID pBitmapData);
HBITMAP LoadBitmapFromFile(PCHAR pFileNamePath);
BOOL ReleaseBitmap(HBITMAP hBitmap);

VOID GetBitmapInfo(HBITMAP hBitmap, OUT PBITMAPINFOHEADER pBitmapInfoHeader);

// Icon
HICON CreateIcon(UINT32 width, UINT32 height, UINT8 numberOfPlanes, UINT8 numberOfBitsPerPixel, PVOID pIconAndMaskBits, PVOID pIconXorMaskBits);
BOOL DestroyIcon(HICON hIcon);

HICON LoadIconFromMemory(PVOID pIconData);
HICON LoadIconFromFile(PCHAR pFileNamePath);
HICON LoadSystemIcon(UINT32 iconId);
BOOL ReleaseIcon(HICON hIcon);

// Cursor
HCURSOR CreateCursor(UINT32 width, UINT32 height, UINT32 hotSpotX, UINT32 hotSpotY, PVOID pCursorAndMaskBits, PVOID pCursorXorMaskBits);
BOOL DestroyCursor(HCURSOR);

HCURSOR LoadCursorFromMemory(PVOID pCursorData);
HCURSOR LoadCursorFromFile(PCHAR pFileNamePath);
HCURSOR LoadSystemCursor(INT32 cursorId);
BOOL ReleaseCursor(HCURSOR hIcon);

// Drawing Related
VOID DrawBegin(HDEVICE_CONTEXT hDeviceContext);
VOID DrawEnd(HDEVICE_CONTEXT hDeviceContext);

VOID DrawRectangleA(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 right, INT32 bottom);
VOID DrawRectangleB(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 width, INT32 height);
VOID DrawRectangleC(HDEVICE_CONTEXT hDeviceContext, PRECTANGLE pRectangle);

VOID DrawFilledRectangleA(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 right, INT32 bottom);
VOID DrawFilledRectangleB(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 width, INT32 height);
VOID DrawFilledRectangleC(HDEVICE_CONTEXT hDeviceContext, PRECTANGLE pRectangle);

VOID DrawControlState(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 right, INT32 bottom, CONTROL_STATE_FLAG controlFlags);

//VOID DrawLine(HDEVICE_CONTEXT hDeviceContext, INT32 xStart, INT32 yStart, INT32 xEnd, INT32 yEnd);

VOID DrawLineA(HDEVICE_CONTEXT hDeviceContext, INT32 xStart, INT32 yStart, INT32 xEnd, INT32 yEnd);
VOID DrawLineB(HDEVICE_CONTEXT hDeviceContext, PPOINT pStart, PPOINT pEnd);

VOID DrawText(HDEVICE_CONTEXT hDeviceContext, PCHAR text, PRECTANGLE pRectangle, DRAW_TEXT_FLAGS flags);
BOOL DrawBitmap(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y, HBITMAP hBitmap);
BOOL DrawIcon(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y, HICON hIcon);
BOOL DrawIconB(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y, HICON hIcon, UINT32 width, UINT32 height, UINT32 animationStepIndex, UINT32 flags);
BOOL DrawCursor(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y, HCURSOR hCursor);

VOID DrawPalette(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y);

UINT32 GetTextWidth(HDEVICE_CONTEXT hDeviceContext, PCHAR text, DRAW_TEXT_FLAGS flags);
UINT32 GetTextHeight(HDEVICE_CONTEXT hDeviceContext, PCHAR text);
COLOR GetColorFromComponents(UINT8 red, UINT8 green, UINT8 blue);
HWINDOW GetWindowFromDeviceContext(HDEVICE_CONTEXT hDeviceContext);
DRAWMODE GetDrawMode(HDEVICE_CONTEXT hDeviceContext);

COLOR GetBackgroundColor(HDEVICE_CONTEXT hDeviceContext);
COLOR GetForegroundColor(HDEVICE_CONTEXT hDeviceContext);

VOID SetBackgroundColor(HDEVICE_CONTEXT hDeviceContext, COLOR color);
VOID SetForegroundColor(HDEVICE_CONTEXT hDeviceContext, COLOR color);
VOID SetTransparentColor(HDEVICE_CONTEXT hDeviceContext, COLOR color);
VOID SetTextColor(HDEVICE_CONTEXT hDeviceContext, COLOR color);
VOID SetDrawMode(HDEVICE_CONTEXT hDeviceContext, DRAWMODE drawMode);

// Rectangle
VOID SetRectangleA(PRECTANGLE pRectangle, INT32 left, INT32 top, INT32 right, INT32 bottom);
VOID SetRectangleB(PRECTANGLE pRectangle, INT32 left, INT32 top, INT32 width, INT32 height);
VOID SetRectangleEmpty(PRECTANGLE pRectangle);

BOOL CheckIfEqualRectangles(PRECTANGLE pRectangle1, PRECTANGLE pRectangle2);
BOOL CheckIfRectangleEmpty(PRECTANGLE pRectangle);
BOOL CheckIfRectangleValid(PRECTANGLE pRectangle);
BOOL CheckIfPointInRectangle(PRECTANGLE pRectangle, PPOINT pPoint);

VOID CopyRectangle(PRECTANGLE pSourceRectangle, PRECTANGLE pDestinationRectangle);
VOID OffsetRectangle(PRECTANGLE pRectangle, PPOINT pPoint);
VOID IntersectRectangle(PRECTANGLE pSourceRectangle1, PRECTANGLE pSourceRectangle2, PRECTANGLE pDestinationRectangle);
VOID UnionRectangle(PRECTANGLE pSourceRectangle1, PRECTANGLE pSourceRectangle2, PRECTANGLE pDestinationRectangle);
VOID SubtractRectangle(PRECTANGLE pSourceRectangle1, PRECTANGLE pSourceRectangle2, PRECTANGLE pDestinationRectangle);

INT32 Min(INT32 number1, INT32 number2);
INT32 Max(INT32 number1, INT32 number2);

// Mouse
VOID UpdateMouse(MOUSE_STATE mouseState);
VOID ShowMouse();
VOID HideMouse();
VOID SetMousePosition(INT32 mouseX, INT32 mouseY);
VOID GetMousePosition(OUT PINT32 mouseX, OUT PINT32 mouseY);
VOID GetMouseLeftButtonState(OUT PBOOL isMouseLeftButtonDown);

// Keyboard
VOID CreateCaret(HWINDOW hWindow, UINT32 width, UINT32 height);
VOID SetCaretPosition(INT32 x, INT32 y);
VOID ShowCaret(HWINDOW hWindow);
VOID HideCaret(HWINDOW hWindow);
VOID SetCaretBlinkTime(UINT32 milliSeconds);

// Helpers

VOID ShowVariableNameAndValue(HDEVICE_CONTEXT hDeviceContext, PCHAR pVariableName, UINT32 variableValue, UINT32 base, PRECTANGLE pWindowRectangle);

VOID DisableUiUpdate();
VOID EnableUiUpdate();

// Next up:
// X GiAllocateFrameBuffer();
//DrawBegin
//DrawFilledRectangle - needs CreateSolidBrush
//DrawEnd
// SetWindowRectangle - needs CreateRectangle
// Move the DrawPalette to GiTest


/*
HDEVICE_CONTEXT GetDeviceContext(HWINDOW hWindow);
HDEVICE_CONTEXT GetClientDeviceContext(HWINDOW hWindow);
VOID ReleaseDeviceContext(HDEVICE_CONTEXT hDeviceContext);

VOID GetClippingRegion(HDEVICE_CONTEXT hDeviceContext, OUT PRECTANGLE pRectangle);
VOID SetClippingRegion(HDEVICE_CONTEXT hDeviceContext, PRECTANGLE pRectangle);
VOID GetWindowRectangle(HDEVICE_CONTEXT hDeviceContext, OUT PRECTANGLE pRectangle);
VOID SetWindowRectangle(HDEVICE_CONTEXT hDeviceContext, PRECTANGLE pRectangle);

// Drawing related

VOID DrawPoint(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y);
VOID DrawLine(HDEVICE_CONTEXT hDeviceContext, INT32 x1, INT32 y1, INT32 x2, INT32 y2);
VOID DrawRectangle(HDEVICE_CONTEXT hDeviceContext, INT32 x1, INT32 y1, INT32 x2, INT32 y2);
VOID DrawFilledRectangle(HDEVICE_CONTEXT hDeviceContext, INT32 x1, INT32 y1, INT32 x2, INT32 y2);
VOID Draw3dRectangle(); // DrawFrame(COLOR colorHighlight1, COLOR colorHighlight2, COLOR colorShadow1, COLOR colorShadow2);
VOID DrawText(HDEVICE_CONTEXT hDeviceContext, PCHAR text, PRECTANGLE pRectangle, DRAW_TEXT_FLAGS flags);
UINT32 GetTextWidth(HDEVICE_CONTEXT hDeviceContext, PCHAR text);
UINT32 GetTextHeight(HDEVICE_CONTEXT hDeviceContext, PCHAR text);
VOID DrawBlit();
VOID DrawMaskBlit();
VOID DrawStretchBlit();
VOID DrawTransparentBlit();

// Paint object related
HPEN CreatePen();  // Would tools include color as well or just line thickness and style?
HBRUSH CreatePatternBrush();
HBRUSH CreateSolidBrush();
HBRUSH CreateDitherBrush() // ???
HFONT CreateFont();
HBITMAP CreateBitmap();  // Creates a 32bit bitmap out of a resource suitable to be drawn

VOID SelectObject(HDEVICE_CONTEXT hDeviceContext, HOBJECT hObject);  // HOBJECT = HPEN | HBRUSH | HFONT
VOID DeleteObject(HDEVICE_CONTEXT hDeviceContext, HOBJECT hObject);

// Region related
VOID CreateRegion();
VOID CombineRegion();

// Color related
VOID SetTransparentColor(HDEVICE_CONTEXT hDeviceContext, COLOR color);
VOID SetForegroundColor(HDEVICE_CONTEXT hDeviceContext, COLOR color); // Sets Pen color for point, line, rectangle
VOID SetBackgroundColor(HDEVICE_CONTEXT hDeviceContext, COLOR color); // Sets Brush color for filledRectangle, textOut
VOID SetTextColor(HDEVICE_CONTEXT hDeviceContext, COLOR color); // Sets the Text color for textOut

COLOR GetColorFromReference(COLOR_REFERENCE colorReference);
UINT8 GetRedValue(COLOR color);
UINT8 GetGreenValue(COLOR color);
UINT8 GetBlueValue(COLOR color);

// Window related
VOID RegisterWindowClass();
VOID CreateWindow();

// Other
VOID GetSystemMetrics(PSYSTEM_METRICS* pSystemMetrics);

// PRIVATE

VOID GiCreateDefaultFont();
COLOR GetPaletteEntryAtIndex(HPALETTE hPalette, UINT32 index);
VOID GiSetPaletteEntryAtIndex(HPALETTE hPalette, UINT32 index, COLOR color);
VOID GiClearDirtyFlags();
VOID GiSetDirtyFlags(INT32 x1, INT32 y1, INT32 x2, INT32 y2);
VOID GiCopyBlock8bit();
VOID GiRectangleToScreen();
VOID GiRectangleToDevice(); // ???
VOID GiRectangleToWindow();

*/

// STRUCTS

//typedef struct _SYSTEM_METRICS

// QUESTIONS

// - How colors will be used?
//     1.) Are they going to be a part of the drawing objects/tools (pen, brush, font)?
//         (makes sense to create all the pens/brushes at the beginning, but might be annoying, as you can't name them all - they will be like pen1, pen2 and utter confusion)
//     2.) Or colors will need to be set on the device context
//         (pretty flexible, don't need create annoying new pens/brushes just to change color, but Draw3dRectangle won't work - but this one could be an outlier, pass 4 colors and it saves your colors and resets)
//     3.) Or gonna add them for every draw call?
//         (flexible, but probably slow, might become annoying too)
//     Option 2.) sounds the way to go

#endif // __GIMAN_H