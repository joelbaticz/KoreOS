/*
  - Date: 2021-09-14
  - Author: Joel
  - Purpose: Graphical Interface object definitions
*/

#ifndef __GIOBJECTS_H
#define __GIOBJECTS_H

typedef HANDLE HDEVICE;
typedef UINT32 DEVICE_TECHNOLOGY;
typedef UINT32 RASTER_CAPABILITIES;
typedef HANDLE HDEVICE_CONTEXT;
typedef HANDLE HWINDOW;
typedef HANDLE HPEN;
typedef HANDLE HBRUSH;
typedef HANDLE HFONT;
typedef HANDLE HBITMAP;
typedef HANDLE HICON;
typedef HANDLE HCURSOR;
typedef UINT32 COLOR;
typedef UINT32 DRAWMODE;
typedef UINT32 DRAW_TEXT_FLAGS;
typedef UINT32 CONTROL_STATE_FLAG;

#define DEVICE_TECHNOLOGY_RASTER_DISPLAY            1
#define DEVICE_TECHNOLOGY_RASTER_PRINTER            2
#define DEVICE_TECHNOLOGY_RASTER_VECTOR_PLOTTER     3

#define RASTER_CAPABILITIES_NONE                    0
#define RASTER_CAPABILITIES_DRAW_LINES              1
#define RASTER_CAPABILITIES_DRAW_RECTANGLES         2
#define RASTER_CAPABILITIES_DRAW_TEXT               4

#define VIDEO_MODE_ATTRIBUTE_COLOR                  0x0001
#define VIDEO_MODE_ATTRIBUTE_GRAPHICS               0x0002
#define VIDEO_MODE_ATTRIBUTE_PALETTE_DRIVEN         0x0003
#define VIDEO_MODE_ATTRIBUTE_MANAGED_PALETTE        0x0004
#define VIDEO_MODE_ATTRIBUTE_BANKED                 0x0005

#define VIDEO_MODE_NUMBER_CURRENT                   0xFFFF

#define DRAW_MODE_NORMAL                            0x01
#define DRAW_MODE_OVERWRITE                         0x01
#define DRAW_MODE_AND                               0x02
#define DRAW_MODE_OR                                0x03
#define DRAW_MODE_XOR                               0x04
#define DRAW_MODE_PEN_SOLID                         0x00
#define DRAW_MODE_PEN_DOT                           0x10

#define DRAW_TEXT_FLAG_ALIGN_LEFT                   0x0001
#define DRAW_TEXT_FLAG_ALIGN_RIGHT                  0x0002
#define DRAW_TEXT_FLAG_ALIGN_TOP                    0x0004
#define DRAW_TEXT_FLAG_ALIGN_BOTTOM                 0x0008
#define DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER      0x0010
#define DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER        0x0020
#define DRAW_TEXT_FLAG_BOLD                         0x0100
#define DRAW_TEXT_FLAG_ITALIC                       0x0200

#define DRAW_CONTROL_STATE_WINDOW_RESIZE_FRAME_THIN   0x100
#define DRAW_CONTROL_STATE_WINDOW_RESIZE_FRAME_THICK  0x110
#define DRAW_CONTROL_STATE_EDGE_RAISED                0x200
#define DRAW_CONTROL_STATE_EDGE_SUNKEN                0x210
#define DRAW_CONTROL_STATE_EDGE_THIN_RAISED           0x220
#define DRAW_CONTROL_STATE_EDGE_THIN_SUNKEN           0x230
#define DRAW_CONTROL_STATE_EDGE_ETCHED                0x240
#define DRAW_CONTROL_STATE_EDGE_WINDOW                0x250
#define DRAW_CONTROL_STATE_ACTIVE_CAPTION_GRADIENT    0x300
#define DRAW_CONTROL_STATE_INACTIVE_CAPTION_GRADIENT  0x310

#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_NORMAL       0x410
#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_ONHOVER      0x420
#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_PRESSED      0x430

#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_NORMAL     0x510
#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_ONHOVER    0x520
#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_PRESSED    0x530

#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_LEFT_NORMAL     0x610
#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_LEFT_ONHOVER    0x620
#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_LEFT_PRESSED    0x630

#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_RIGHT_NORMAL    0x710
#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_RIGHT_ONHOVER   0x720
#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_RIGHT_PRESSED   0x730

#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_THUMB_NORMAL    0x810
#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_THUMB_ONHOVER   0x820
#define DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_THUMB_PRESSED   0x830

#define DRAW_CONTROL_STATE_FIELD                            0x900
#define DRAW_CONTROL_STATE_FIELD_DISABLED                   0x910

#define DRAW_CONTROL_STATE_CHECKBOX_TICK                    0xA00
#define DRAW_CONTROL_STATE_CHECKBOX_TICK_DISABLED           0xA10

#define DRAW_CONTROL_STATE_RADIOBUTTON                      0xB00
#define DRAW_CONTROL_STATE_RADIOBUTTON_DISABLED             0xB10
#define DRAW_CONTROL_STATE_RADIOBUTTON_SELECTION            0xB20
#define DRAW_CONTROL_STATE_RADIOBUTTON_SELECTION_DISABLED   0xB30

typedef struct _PALETTE_ENTRY
{
    UINT8 Red;
    UINT8 Green;
    UINT8 Blue;
} PALETTE_ENTRY, *PPALETTE_ENTRY;

typedef struct _PALETTE
{
    UINT32 NumberOfEntries;
    PPALETTE_ENTRY pPaletteEntries;
} PALETTE, *PPALETTE;

typedef struct _DISPLAY_DEVICE
{
    CHAR DeviceName[32];
    CHAR DeviceDescription[128];  // ???
    HDEVICE hDevice;
} DISPLAY_DEVICE, *PDISPLAY_DEVICE;

typedef struct _DEVICE_CAPABILITIES
{
    DEVICE_TECHNOLOGY DeviceTechnology;
    UINT32 ResolutionX;                             // If it is a display device, then this value reflects the current video mode's resolution
    UINT32 ResolutionY;                             // If it is a display device, then this value reflects the current video mode's resolution
    UINT32 NumberOfColors;                          // If it is a display device, then this value reflects the current video mode's number of displayable colors
    UINT32 NumberOfVideoModes;
    UINT32 CurrentVideoModeIndex;
    RASTER_CAPABILITIES RasterCapabilities;
} DEVICE_CAPABILITIES, *PDEVICE_CAPABILITIES;

typedef struct _VIDEO_MODE_INFORMATION
{
  UINT32 ModeNumber;    // Unique mode number, starts from 0 
  UINT32 Width;
  UINT32 Height;
  UINT32 WidthInBytes;  // pitch / stride
  UINT32 NumberOfBitsPerPixel;
  UINT32 NumberOfColors;
  UINT32 NumberOfPlanes;
  UINT32 NumberOfRedBits;
  UINT32 NumberOfGreenBits;
  UINT32 NumberOfBlueBits;
  UINT32 RedMask;
  UINT32 GreenMask;
  UINT32 BlueMask;
  UINT32 BitsPerPlane;
  UINT32 Attributes;
  UINT32 Frequency;
} VIDEO_MODE_INFORMATION, *PVIDEO_MODE_INFORMATION;

typedef struct _POINT
{
  INT32 X;
  INT32 Y;
} POINT, *PPOINT;

typedef struct _RECTANGLE
{
  INT32 Left;
  INT32 Top;
  INT32 Right;
  INT32 Bottom;
} RECTANGLE, *PRECTANGLE;

typedef struct _REGION
{
  UINT32 NumberOfRectangles;
  PRECTANGLE* pClippingRectangleList;
} REGION, *PREGION;

typedef struct _DEVICE_CONTEXT
{
  HDEVICE                 hDevice;
  HDEVICE_CONTEXT         hDeviceContext;         // This DeviceContext's handle
  PDEVICE_CAPABILITIES    pDeviceCapabilities;
  PVIDEO_MODE_INFORMATION pVideoModeInformation;
  PVOID                   pDeviceFrameBuffer;
  HWINDOW                 hWindow;
  PRECTANGLE              pWindowRectangle;
  PREGION                 pClippingRegion;

  HPEN      hPen;
  HBRUSH    hBrush;
  HFONT     hFont;

  COLOR     TransparentColor;
  COLOR     ForegroundColor;
  COLOR     BackgroundColor;
  COLOR     TextColor;

  DRAWMODE  DrawMode;

  UINT32    ReferenceCount;   // Reference count is affected by Get, GetClient, Release and any drawing operation. The device context only can be deleted if it is not held hostage by any operation
  UINT32    LastErrorCode;
  PVOID     pNext;
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

typedef struct _MOUSE_STATE
{
    INT32 RelativeX;
    INT32 RelativeY;
    BOOL  IsLeftButtonDown;
    BOOL  IsMiddleButtonDown;
    BOOL  IsRightButtonDown;
} MOUSE_STATE, *PMOUSE_STATE;

typedef struct _CARET
{
    HWINDOW hWindow;
    INT32 Width;
    INT32 Height;
    INT32 PositionX;
    INT32 PositionY;
    BOOL IsShown;
} CARET, *PCARET;

#endif /* __GIOBJECTS_H */