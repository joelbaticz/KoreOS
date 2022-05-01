#ifndef __WINDOWING_H
#define __WINDOWING_H

#define MAX_CLASS_NAME_LENGTH                               64
#define MAX_WINDOW_NAME_LENGTH                              255

#define CLASS_SIGNATURE                                     0x1001
#define WINDOW_SIGNATURE                                    0x2001

// Class Flags
#define CLASS_FLAG_OWNER_DRAWN_BACKGROUND                   0x0001  // The Gi is not drawing the background with the backgroundColor, the owner is responsible for that
#define CLASS_FLAG_SAVE_OBSCURED_AREA_FOR_REDRAW            0x0002
#define CLASS_FLAG_DISABLE_CLOSE                            0x0004
#define CLASS_FLAG_REDRAW_ALL_ON_HORIZONTAL_ADJUSTMENT      0x0008
#define CLASS_FLAG_REDRAW_ALL_ON_VERTICAL_ADJUSTMENT        0x0010

// Common Window / Control Flags
#define WINDOW_FLAG_IS_VISIBLE                              0x00000001  // IS_SHOWN ?
#define WINDOW_FLAG_IS_DISABLED                             0x00000002
#define WINDOW_FLAG_IS_CHILD                                0x00000004
#define WINDOW_FLAG_IS_TAB_STOP                             0x00000008
#define WINDOW_FLAG_HAS_HORIZONTAL_SCROLL_BAR               0x00000010
#define WINDOW_FLAG_HAS_VERTICAL_SCROLL_BAR                 0x00000020
#define WINDOW_FLAG_IS_ACTIVE                               0x00000040
//#define WINDOW_FLAG_IS_POPUP   // Popup windows are not constrained to the parent window's area, they are not clipped by the parent window. Like PopupMenus.

// Window Flags
#define WINDOW_FLAG_HAS_BORDER		                        0x00010000  // The window has a thin-line border
#define WINDOW_FLAG_HAS_SIZING_BORDER                       0x00020000  // The window has a sizing border, which is normally thicker than the usual
#define WINDOW_FLAG_HAS_DIALOG_BORDER	                    0x00040000  // The window has a border of a style typically used with dialog boxes.
#define WINDOW_FLAG_HAS_CAPTION                             0x00080000
#define WINDOW_FLAG_HAS_MINIMIZE_BUTTON                     0x00100000
#define WINDOW_FLAG_HAS_MAXIMIZE_BUTTON                     0x00200000
#define WINDOW_FLAG_IS_MAXIMIZED                            0x00400000
#define WINDOW_FLAG_IS_MINIMIZED                            0x00800000
#define WINDOW_FLAG_DO_CLIP_CHILDREN                        0x01000000
#define WINDOW_FLAG_DO_CLIP_SIBLINGS                        0x02000000
#define WINDOW_FLAG_IS_TOP_LEVEL_WINDOW                     WINDOW_FLAG_HAS_BORDER + WINDOW_FLAG_HAS_CAPTION + WINDOW_FLAG_HAS_MINIMIZE_BUTTON + WINDOW_FLAG_HAS_MAXIMIZE_BUTTON + WINDOW_FLAG_IS_VISIBLE
#define WINDOW_FLAG_VERTICAL_SCROLLBAR                      0x10000000
#define WINDOW_FLAG_HORIZONTAL_SCROLLBAR                    0x20000000
//#define WINDOW_FLAG_HAS_CLOSE_BUTTON

// Button Flags
#define BUTTON_FLAG_IS_FLAT_STYLE                           0x00010000     // Not used
#define BUTTON_FLAG_IS_DEFAULT                              0x00100000
#define BUTTON_FLAG_IS_FOCUSED                              0x00200000
#define BUTTON_FLAG_IS_ON_HOVER                             0x00400000
#define BUTTON_FLAG_IS_PRESSED                              0x00800000

// Scrollbar Flags
#define SCROLLBAR_FLAG_BUTTON1_IS_ON_HOVER                  0x00100000
#define SCROLLBAR_FLAG_BUTTON2_IS_ON_HOVER                  0x00200000
#define SCROLLBAR_FLAG_THUMB_IS_ON_HOVER                    0x00400000
#define SCROLLBAR_FLAG_BUTTON1_IS_PRESSED                   0x00800000
#define SCROLLBAR_FLAG_BUTTON2_IS_PRESSED                   0x01000000
#define SCROLLBAR_FLAG_THUMB_IS_PRESSED                     0x02000000

// Checkbox Flags
#define CHECKBOX_FLAG_IS_DEFAULT                            0x00100000
#define CHECKBOX_FLAG_IS_FOCUSED                            0x00200000
#define CHECKBOX_FLAG_IS_ON_HOVER                           0x00400000
#define CHECKBOX_FLAG_IS_PRESSED                            0x00800000
#define CHECKBOX_FLAG_IS_CHECKED                            0x01000000

// Radiobutton Flags
#define RADIOBUTTON_FLAG_IS_DEFAULT                         0x00100000
#define RADIOBUTTON_FLAG_IS_FOCUSED                         0x00200000
#define RADIOBUTTON_FLAG_IS_ON_HOVER                        0x00400000
#define RADIOBUTTON_FLAG_IS_PRESSED                         0x00800000
#define RADIOBUTTON_FLAG_IS_CHECKED                         0x01000000

// TextBox Flags
#define TEXTBOX_FLAG_IS_DEFAULT                             0x00100000
#define TEXTBOX_FLAG_IS_FOCUSED                             0x00200000
#define TEXTBOX_FLAG_IS_ON_HOVER                            0x00400000

// Window Messages
#define WINDOW_MESSAGE_NULL_MESSAGE                         0x0000
#define WINDOW_MESSAGE_ACTIVATE                             0x0001
#define WINDOW_MESSAGE_DEACTIVATE                           0x0002
#define WINDOW_MESSAGE_PAINT_NON_CLIENT                     0x0003
#define WINDOW_MESSAGE_PAINT                                0x0004
#define WINDOW_MESSAGE_MOUSE_ENTER                          0x0005
#define WINDOW_MESSAGE_MOUSE_LEAVE                          0x0006
#define WINDOW_MESSAGE_MOUSE_MOVE                           0x0007
#define WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_DOWN               0x0008
#define WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_UP                 0x0009
#define WINDOW_MESSAGE_MOUSE_MIDDLE_BUTTON_DOWN             0x000A
#define WINDOW_MESSAGE_MOUSE_MIDDLE_BUTTON_UP               0x000B
#define WINDOW_MESSAGE_MOUSE_RIGHT_BUTTON_DOWN              0x000C
#define WINDOW_MESSAGE_MOUSE_RIGHT_BUTTON_UP                0x000D
#define WINDOW_MESSAGE_MOVED                                0x000E
#define WINDOW_MESSAGE_RESIZED                              0x000F
#define WINDOW_MESSAGE_CREATE                               0x0010
#define WINDOW_MESSAGE_FOCUS_GAINED                         0x0011
#define WINDOW_MESSAGE_FOCUS_LOST                           0x0012
#define WINDOW_MESSAGE_DEFAULT_GAINED                       0x0013
#define WINDOW_MESSAGE_DEFAULT_LOST                         0x0014
#define WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_CLICK              0x0015
#define WINDOW_MESSAGE_COMMAND                              0x0016
#define WINDOW_MESSAGE_ON_TIMER                             0x0017
#define WINDOW_MESSAGE_ON_SYSTEM_TICK                       0x0018
#define WINDOW_MESSAGE_KEYDOWN                              0x0019
#define WINDOW_MESSAGE_KEYUP                                0x0020

#define WINDOW_MESSAGE_GET_ENABLED                          0x0021
#define WINDOW_MESSAGE_GET_CHECKED                          0x0022
#define WINDOW_MESSAGE_GET_STATE                            0x0023
#define WINDOW_MESSAGE_GET_FOCUS                            0x0024
#define WINDOW_MESSAGE_GET_DEFAULT                          0x0025

#define WINDOW_MESSAGE_SET_ENABLED                          0x0026
#define WINDOW_MESSAGE_SET_CHECKED                          0x0027
#define WINDOW_MESSAGE_SET_STATE                            0x0028
#define WINDOW_MESSAGE_SET_FOCUS                            0x0029
#define WINDOW_MESSAGE_SET_DEFAULT                          0x002A

// Commands emitted by controls
#define COMMAND_BUTTON_CLICKED                              0x0001
#define COMMAND_SCROLL_UP                                   0x0002
#define COMMAND_SCROLL_DOWN                                 0x0003
#define COMMAND_SCROLL_RIGHT                                0x0004
#define COMMAND_SCROLL_LEFT                                 0x0005
#define COMMAND_SCROLL_TO_VERTICAL_POSITION                 0x0006
#define COMMAND_SCROLL_TO_HORIZONTAL_POSITION               0x0007
#define COMMAND_CHECKBOX_CLICKED                            0x0008
#define COMMAND_RADIOBUTTON_CLICKED                         0x0009

typedef UINT32 CLASS_FLAGS;
typedef UINT32 WINDOW_FLAGS;
typedef UINT32 WINDOW_MESSAGE;

typedef UINT32 HWINDOW_CLASS;
typedef UINT32 HWINDOW;
typedef UINT32 HBUTTON;
typedef UINT32 HICON;
typedef UINT32 HCURSOR;
typedef UINT32 HMENU;

typedef struct _WINDOW_CLASS
{
    UINT32 Signature;
    HWINDOW_CLASS hWindowClass;
    CHAR ClassName[MAX_CLASS_NAME_LENGTH];
    UINT32 PerClassExtra;
    UINT32 PerWindowExtra;
    HICON hIcon;
    HCURSOR hCursor;
    COLOR BackgroundColor;
    PUINT32 pWindowProcedure;
    CLASS_FLAGS Flags;
    PVOID pNext;
} WINDOW_CLASS, *PWINDOW_CLASS;

typedef struct _WINDOW
{
    UINT32 Signature;
    PWINDOW_CLASS pWindowClass;
    CHAR WindowName[MAX_WINDOW_NAME_LENGTH];
    INT32 X;
    INT32 Y;
    INT32 Width;
    INT32 Height;
    HMENU hMenu;
    WINDOW_FLAGS Flags;
    PRECTANGLE pWindowRectangle;  // This way we need PointerToHandle() function in MemMan
    PRECTANGLE pClientRectangle;
    PREGION ClippingRegion;
    UINT32 Extra[16];

    HWINDOW hWindow;
    HWINDOW hParent;
    HWINDOW hLastDefault;
    HWINDOW hLastFocused;

    PVOID pParent;
    PVOID pPrev;
    PVOID pNext;    
    PVOID pFirstChild;

} WINDOW, *PWINDOW;

//static BOOL IsWindowingInitialized;
static UINT32 IsWindowingInitialized;

VOID WindowingInit();
VOID WindowingStart();

KSTATUS RegisterWindow(PCHAR pClassName, UINT32 perClassExtra, UINT32 perWindowExtra, HICON hIcon, HCURSOR hCursor, COLOR backgroundColor, PVOID pWindowProcedure, CLASS_FLAGS flags);
KSTATUS UnregisterWindow(PCHAR pClassName);

HWINDOW CreateWindowByClass(PCHAR pClassName, PCHAR pWindowName, INT32 x, INT32 y, INT32 width, INT32 height, HWINDOW hParent, HMENU hMenu, WINDOW_FLAGS flags);
KSTATUS DeleteWindow(HWINDOW hWindow); // Deletes all child windows as well

UINT32 DefaultWindowProcedure(HWINDOW hWindow, UINT32 message, UINT32 param1, UINT32 param2);

//HWINDOW FindWindowA(PCHAR pClassName, PCHAR pWindowName);
//HWINDOW FindWindowB(HWINDOW hParent, HWINDOW hChildAfter);

PWINDOW_CLASS GetClass(PCHAR pClassName);
VOID GetClassName(HWINDOW hWindow, OUT PCHAR pClassNameBuffer, UINT32 bufferSize);

UINT32 GetClassExtra(PCHAR pClassName, UINT32 index);
UINT32 GetWindowExtra(HWINDOW hWindow, UINT32 index);  // This is the only function provided by the system to get extra bytes. This is enough to retrieve/store pointers to other data structures
VOID SetWindowExtra(HWINDOW hWindow, UINT32 index, UINT32 value);  // This is the only function provided by the system to get extra bytes. This is enough to retrieve/store pointers to other data structures

VOID CreateDialog();

VOID ShowWindow(HWINDOW hWindow);
VOID HideWindow(HWINDOW hWindow);

BOOL IsOfClass(HWINDOW hWindow, PCHAR pClassName);
BOOL IsValid(HWINDOW hWindow);   // >>> should be called IsWindow?
BOOL IsVisible(HWINDOW hWindow);
BOOL IsEnabled(HWINDOW hWindow);
BOOL IsChild(HWINDOW hWindow);   // >>> should be another flavour, called IsChildOf(HWINDOW hWindowParent, HWINDOW, hWindow) which searches all child descendant windows

VOID SetVisible(HWINDOW hWindow, BOOL isVisible);
HWINDOW GetParent(HWINDOW hWindow);
HWINDOW GetTopWindow(HWINDOW hWindow);
HWINDOW GetFirstChild(HWINDOW hWindowParent);
HWINDOW GetLastChild(HWINDOW hWindowParent);
VOID SetLastChild(HWINDOW hWindowParent, HWINDOW hWindowChild);
HWINDOW GetNextSibling(HWINDOW hWindow);
HWINDOW GetPreviousSibling(HWINDOW hWindow);

WINDOW_FLAGS GetFlags(HWINDOW hWindow);
VOID SetFlags(HWINDOW hWindow, WINDOW_FLAGS flags);
VOID SetFlag(HWINDOW hWindow, WINDOW_FLAGS flags);
VOID ClearFlag(HWINDOW hWindow, WINDOW_FLAGS flags);
BOOL HasFlag(HWINDOW hWindow, WINDOW_FLAGS flags);

PCHAR GetWindowTextPointer(HWINDOW hWindow);
VOID GetWindowText(HWINDOW hWindow, OUT PCHAR pBuffer, UINT32 bufferLength);
VOID SetWindowText(HWINDOW hWindow, PCHAR pText);

// Window Rectangle related
KSTATUS GetScreenRectangle(HWINDOW hWindow, OUT PRECTANGLE pWindowRectangle);       // USAGE: 14
KSTATUS SetScreenRectangle(HWINDOW hWindow, PRECTANGLE pWindowRectangle);           // USAGE: -

KSTATUS GetWindowRectangle(HWINDOW hWindow, OUT PRECTANGLE pWindowRectangle);       // USAGE: 1
KSTATUS SetWindowRectangle(HWINDOW hWindow, PRECTANGLE pWindowRectangle);           // USAGE: 1

KSTATUS GetClientRectangle(HWINDOW hWindow, OUT PRECTANGLE pClientRectangle);       // USAGE: 6
KSTATUS SetClientRectangle(HWINDOW hWindow, PRECTANGLE pClientRectangle);           // USAGE: -

VOID GetWindowPosition(HWINDOW hWindow, OUT PPOINT pPoint);                         // USAGE: -
VOID SetWindowPosition(HWINDOW hWindow, PPOINT pPoint);                             // USAGE: -

VOID RectangleToScreen(HWINDOW hWindow, PRECTANGLE pRectangle);                     // USAGE: -
VOID RectangleToClient(HWINDOW hWindow, PRECTANGLE pRectangle);                     // USAGE: -

VOID PointToScreen(HWINDOW hWindow, OUT PPOINT pPoint);                             // USAGE: 4
VOID PointToClient(HWINDOW hWindow, OUT PPOINT pPoint);                             // USAGE: -

// NOT SURE
VOID RectangleReduceToClientArea(HWINDOW hWindow, PRECTANGLE pRectangle);           // USAGE: 5  // GIMAN AND WINDOW USES IT TO CALCULATE BOUNDARIES OF CLIENT AREA
                                                                                                 // THIS SHOULD BE REPLACED BY A SCREEN TO CLIENT METHOD
                                                                                                 // MOST OF THE STUFF SHOULD BE CALULATED IN SCREEN COORDINATES ANYWAYS

                                                                                                 // THIS WHOLE METHOD COULD BE REPLACED BY DIFFERENT CALLS (LOOK AT USAGE FOR MORE)

// Mouse related
VOID HandleMouseEvents();
HWINDOW GetCapture();
VOID SetCapture(HWINDOW hWindow);
VOID ReleaseCapture();

// Keyboard related
VOID HandleKeyboardEvents();
VOID SetFocus(HWINDOW hWindow);

VOID SetDefault(HWINDOW hWindow);

VOID BringWindowToTop(HWINDOW hWindow);
VOID ResetWindows();
VOID BroadcastMessage(HWINDOW hWindow, WINDOW_MESSAGE message);

HWINDOW GetFocusedWindow();
HWINDOW GetActiveWindow();

#endif /* __WINDOWING_H */