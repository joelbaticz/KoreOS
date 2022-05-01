#include "kldr.h"

#define MAX_NUMBER_OF_WINDOW_CLASSES    100
#define MAX_NUMBER_OF_WINDOWS           1000

static CHAR WiWindowClass[]            = "WINDOWCLASS";
static CHAR WiWindowClassName[]        = "WINDOW";

static CHAR WiTextBuffer[255];

static PWINDOW_CLASS pWindowClassListHead;
static PWINDOW       pWindowOrderListHead;

static UINT32        windowClassCount;
static UINT32        windowCount;

static HWINDOW       hActiveWindow;  // NEVER INITIALIZE A VALUE HERE, USE THE INIT METHOD INSTEAD AS IT IS NOT RELIABLE!!!
static HWINDOW       hFocusedWindow;
static HWINDOW       hWindowUnderMouse;
static HWINDOW       hLastWindowUnderMouse;
static HWINDOW       hWindowMouseCapture;  // The handle of a window which should receive all the mouse events

static UINT32        mouseX;
static UINT32        mouseY;
static UINT32        mouseXOld;
static UINT32        mouseYOld;
static BOOL          isMouseLeftButtonDown;
static BOOL          isMouseMiddleButtonDown;
static BOOL          isMouseRightButtonDown;
static BOOL          isMouseLeftButtonDownOld;
static BOOL          isMouseMiddleButtonDownOld;
static BOOL          isMouseRightButtonDownOld;

VOID WindowClassListInit();
VOID WindowOrderListInit();
VOID RegisterTestWindow();

VOID AddToWindowClassList(PWINDOW_CLASS pWindowClass);
VOID AddToWindowOrderList(PWINDOW pWindow);

PWINDOW GetWindowEntry(HWINDOW hWindow);
HWINDOW GetWindowHandle(PWINDOW pWindow);

VOID DrawChildren(HWINDOW hWindow);

HWINDOW GetTopMostWindowUnderPoint(INT32 pointX, INT32 pointY);
PWINDOW GetTopMostWindowUnderPointInChildren(PWINDOW pWindow, PWINDOW pTopWindow, INT32 pointX, INT32 pointY);
VOID ActivateWindow(HWINDOW hWindow);

VOID        HandleMouseMove();
VOID        HandleMouseLeftButtonChange();

VOID WindowingInit()
{
    UiDrawText("WindowingInit()");

    //IsWindowingInitialized = FALSE;

    WindowClassListInit();
    WindowOrderListInit();

    hActiveWindow = (HWINDOW) NULL;
    hFocusedWindow = (HWINDOW) NULL;
    hWindowUnderMouse = (HWINDOW) NULL;
    hLastWindowUnderMouse = (HWINDOW) NULL;
    hWindowMouseCapture = (HWINDOW) NULL;

    UiDrawText("WindowingInit()...........DONE");
}

VOID WindowingUninit()
{
    // TODO: needs to go thru all top level windows (that belong to the desktop, or don't have parent - to be determined)
    //       and need to destroy those first, freeing the memory on the way
    //       Needs to switch back video mode to text mode?
}

VOID WindowingStart()
{
    // Start graphics
    DISPLAY_DEVICE displayDevice;
    EnumerateDevices(&displayDevice, sizeof(displayDevice));
    HDEVICE hDevice = displayDevice.hDevice;
    SetVideoMode(hDevice, 2);

    // Register UI Components
    RegisterDesktop();
    RegisterApplicationWindow();
    RegisterButton();
    RegisterCheckbox();
    RegisterRadiobutton();
    RegisterScrollbar();
    RegisterTextBox();

    SetMousePosition(160, 100);
    ShowMouse();

    IsWindowingInitialized = TRUE;

    UiDrawText("Windowing started.");
}

VOID WindowClassListInit()
{
    pWindowClassListHead = NULL;

    windowClassCount = 0;
}

VOID WindowOrderListInit()
{
    pWindowOrderListHead = NULL;

    windowCount = 0;
}

VOID AddToWindowClassList(PWINDOW_CLASS pNewWindowClass)
{
    if (pWindowClassListHead == NULL)
    {
        // Register first entry
        pWindowClassListHead = pNewWindowClass;
        pNewWindowClass->pNext = NULL;
    }
    else
    {
        // Add entry at the end of the list
        PWINDOW_CLASS pWindowClassEntry = pWindowClassListHead;

        while(pWindowClassEntry->pNext != NULL)
        {
            pWindowClassEntry = pWindowClassEntry->pNext;
        }

        pWindowClassEntry->pNext = pNewWindowClass;
        pNewWindowClass->pNext = NULL;
    }

    windowClassCount++;
}

VOID AddToWindowOrderList(PWINDOW pNewWindow)
{
    if (pWindowOrderListHead == NULL)
    {
        // Register first entry

        pWindowOrderListHead = pNewWindow;
        pNewWindow->pNext = NULL;
    }
    else
    {
        // Add entry at the end of the list

        PWINDOW pWindowEntry = pWindowOrderListHead;

        while(pWindowEntry->pNext != NULL)
        {
            pWindowEntry = pWindowEntry->pNext;
        }

        pWindowEntry->pNext = pNewWindow;
        pNewWindow->pNext = NULL;
    }

    windowCount++;
}

KSTATUS RegisterWindow(PCHAR pClassName, UINT32 perClassExtra, UINT32 perWindowExtra, HICON hIcon, HCURSOR hCursor, COLOR backgroundColor, PVOID pWindowProcedure, CLASS_FLAGS flags)
{
    UiDrawText("RegisterWindow()");

    // Validation

    if (strlen(pClassName) >= MAX_CLASS_NAME_LENGTH)
    {
        return STATUS_ERROR_VALIDATION_CHECK_FAILED;
    }

    // Allocate memory for new Window Class

    HWINDOW_CLASS hNewWindowClass = MemoryAllocate(sizeof(WINDOW_CLASS) + perClassExtra * sizeof(UINT32), MEMORY_FLAG_ZERO_INIT, (PCHAR) &WiWindowClass);
    
    if (!hNewWindowClass)
    {
        return STATUS_ERROR_OUT_OF_MEMORY;
    }

    PWINDOW_CLASS pNewWindowClass = MemoryLock(hNewWindowClass);

    if (!pNewWindowClass)
    {
        return STATUS_ERROR_UNABLE_TO_LOCK_OBJECT;
    }

    // Fill in Window Class details

    pNewWindowClass->Signature = CLASS_SIGNATURE;
    pNewWindowClass->hWindowClass = hNewWindowClass;
    strcpy((char*) &pNewWindowClass->ClassName[0], (char*) pClassName);
    pNewWindowClass->PerClassExtra = perClassExtra;
    pNewWindowClass->PerWindowExtra = perWindowExtra;
    pNewWindowClass->hIcon = hIcon;
    pNewWindowClass->hCursor = hCursor;
    pNewWindowClass->BackgroundColor = backgroundColor;
    pNewWindowClass->pWindowProcedure = pWindowProcedure;
    pNewWindowClass->Flags = flags;
    pNewWindowClass->pNext = NULL;
    
    // Register entry in the list
    AddToWindowClassList(pNewWindowClass);

    return STATUS_SUCCESS;
}

PWINDOW_CLASS GetClass(PCHAR pClassName)
{
    UiDrawText("GetClass()");

    PWINDOW_CLASS pWindowClassEntry = pWindowClassListHead;

    if (pWindowClassEntry == NULL)
    {
        return NULL;
    }

    while(pWindowClassEntry != NULL)
    {

        if (strcmp(&pWindowClassEntry->ClassName[0], pClassName) == 0)
        {
            // BugCheck(NULL, (PCHAR)&pWindowClassEntry->ClassName[0], (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);
            return pWindowClassEntry;
        }

        pWindowClassEntry = pWindowClassEntry->pNext;
    }

    return NULL;
}

VOID GetClassName(HWINDOW hWindow, OUT PCHAR pClassNameBuffer, UINT32 bufferSize)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    if (pClassNameBuffer == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Buffer was null!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Window not found!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);
    }  

    PWINDOW_CLASS pWindowClassEntry = (PWINDOW_CLASS) pWindowEntry->pWindowClass;

    if (pWindowClassEntry == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Window class not found!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);
    }  

    PCHAR pWindowClassName = (PCHAR) &pWindowClassEntry->ClassName[0];
    UINT32 classNameLength = strlen(pWindowClassName);

    if (classNameLength > bufferSize)
    {
        classNameLength = bufferSize;
    }

    strncpy(pClassNameBuffer, pWindowClassName, classNameLength);

    //BugCheck(NULL, pClassNameBuffer, (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);
}

HWINDOW CreateWindowByClass(PCHAR pClassName, PCHAR pWindowName, INT32 x, INT32 y, INT32 width, INT32 height, HWINDOW hParent, HMENU hMenu, WINDOW_FLAGS flags)
{
    UiDrawText("CreateWindowByClass()");

    // Validation
    if (strlen(pClassName) >= MAX_CLASS_NAME_LENGTH)
    {
        return (HWINDOW) NULL;
    }

    if (strlen(pWindowName) >= MAX_WINDOW_NAME_LENGTH)
    {
        return (HWINDOW) NULL;
    }

    // TODO: Check if Window Class exist
    PWINDOW_CLASS pWindowClass = GetClass(pClassName);
    
    if (pWindowClass == NULL)
    {
        return (HWINDOW) NULL;
    }

    // Allocate memory for new Window 

    //HWINDOW hNewWindow = MemoryAllocate(sizeof(WINDOW) + pWindowClass->PerWindowExtra * sizeof(UINT32), MEMORY_FLAG_ZERO_INIT, (PCHAR) &WiWindowClassName);
    HWINDOW hNewWindow = MemoryAllocate(sizeof(WINDOW), MEMORY_FLAG_ZERO_INIT, (PCHAR) &WiWindowClassName);
    //BugCheck(NULL, (PCHAR) &"Is this the real HWINDOW?", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hNewWindow);
    if (hNewWindow == (HWINDOW) NULL)
    {
        return (HWINDOW) NULL;
    }

    PWINDOW pNewWindow = MemoryLock(hNewWindow);

    if (pNewWindow == NULL)
    {
        return (HWINDOW) NULL;
    }

    if (hParent == (HWINDOW) NULL)
    {
        if (strcmp(pClassName, (const char*) &"DESKTOP") != 0)
        {
            // If we are creating the Desktop window the parent is NULL, but the hDesktopWindow hasn't been initialized yet.
            // So only set it for not the Desktop
            hParent = hDesktopWindow;
        }
    }

    // Fill in Window details
    pNewWindow->Signature = WINDOW_SIGNATURE;
    pNewWindow->pWindowClass = pWindowClass;
    strcpy((char*) &pNewWindow->WindowName[0], (char*) pWindowName);
    pNewWindow->X = x;
    pNewWindow->Y = y;
    pNewWindow->Width = width;
    pNewWindow->Height = height;
    pNewWindow->hMenu = hMenu;
    pNewWindow->Flags = flags;

    pNewWindow->hWindow = hNewWindow;
    pNewWindow->hParent = hParent;
    pNewWindow->hLastDefault = (HWINDOW) NULL;
    pNewWindow->hLastFocused = (HWINDOW) NULL;

    pNewWindow->pParent     = NULL;
    pNewWindow->pPrev       = NULL;
    pNewWindow->pNext       = NULL;
    pNewWindow->pFirstChild = NULL;
//BugCheck(NULL, (PCHAR) &"XXXXXX", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);
    //Window Extra Bytes should be 0-ed already as we zero init the allocated memory
    //PUINT8 pNewWindowExtraBytes = pNewWindow + sizeof()
    //memset((char*) pUsedListEntry->base, 0, pUsedListEntry->size);



    // Register entry in the children order collection
    if (hParent != (HWINDOW) NULL)
    {
        pNewWindow->pParent = GetWindowEntry(hParent);

        pNewWindow->pPrev   = GetWindowEntry(GetLastChild(pNewWindow->hParent));
        
        SetLastChild(pNewWindow->hParent, hNewWindow);
    }

    if (flags & WINDOW_FLAG_IS_VISIBLE)
    {
        
        ShowWindow(hNewWindow);
//BugCheck(NULL, (PCHAR) &pWindowClass->ClassName, (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);
    }

    if (flags & BUTTON_FLAG_IS_DEFAULT)  // IS_DEFAULT_CHILD
    {
        SetDefault(hNewWindow);
    }

    if (flags & BUTTON_FLAG_IS_FOCUSED)  // IS_DEFAULT_FOCUSED
    {
        SetFocus(hNewWindow);
    }

    if (flags & WINDOW_FLAG_VERTICAL_SCROLLBAR)
    {
        UINT32 scrollbarSize = GetSystemMetric(METRIC_SCROLLBAR_SIZE);
        RECTANGLE clientRectangle;
        GetClientRectangle(hNewWindow, &clientRectangle);
        CreateScrollbar((PCHAR) &"VerticalScrollbar", clientRectangle.Right - scrollbarSize, 0, scrollbarSize, clientRectangle.Bottom, hNewWindow, WINDOW_FLAG_IS_CHILD | WINDOW_FLAG_IS_VISIBLE);
    }

    return hNewWindow;
}

UINT32 GetWindowExtra(HWINDOW hWindow, UINT32 index)
{
    if (!IsValid(hWindow))
    {
        return 0;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL)
    {
        return 0;
    }

    UINT32 perWindowExtra = pWindowEntry->pWindowClass->PerWindowExtra;

    if (perWindowExtra <= index)
    {
        return 0;
    }

    // PUINT32 pExtra = (PUINT32) (((PUINT8) pWindowEntry) + sizeof(WINDOW) + (index * sizeof(UINT32)));

    // return pExtra[0];

    return pWindowEntry->Extra[index];
}

VOID SetWindowExtra(HWINDOW hWindow, UINT32 index, UINT32 value)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL)
    {
        return;
    }

    UINT32 perWindowExtra = pWindowEntry->pWindowClass->PerWindowExtra;

    if (perWindowExtra <= index)
    {
        return;
    }

    // PUINT32 pExtra = (PUINT32) (((PUINT8) pWindowEntry) + sizeof(WINDOW) + (index * sizeof(UINT32)));

    // pExtra[0] = value;

    pWindowEntry->Extra[index] = value;
}


VOID ShowWindow(HWINDOW hWindow)
{
    UiDrawText("ShowWindow()");

    if (!IsValid(hWindow))
    {
        return;
    }

    // If already shown, don't worry
    // if (IsVisible(hWindow))
    // {
    //     return;
    // }

    // If not child window, active this new window

    // if (!IsChild(hWindow) && hActiveWindow != (HWINDOW) NULL)
    // //if (hActiveWindow != (HWINDOW) NULL)
    // {
    //     // UiDrawText("Why???");
    //     // HALT;

    //     SendMessage(hActiveWindow, WINDOW_MESSAGE_DEACTIVATE, 0, 0);
    //     SendMessage(hActiveWindow, WINDOW_MESSAGE_PAINT_NON_CLIENT, 0, 0);
    //     //SendMessage(hActiveWindow, WINDOW_MESSAGE_PAINT, 0, 0);
    //     SendMessage(hWindow, WINDOW_MESSAGE_ACTIVATE, 0, 0);
    // }

    // Add window to the ZOrder
    // Already done???

    // Mark visible
    SetVisible(hWindow, TRUE);
//BugCheck(NULL, (PCHAR) &"XXXXXX", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);
    // Send paint events to new active window
//BugCheck(NULL, (PCHAR) &"CCC", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);   
    SendMessage(hWindow, WINDOW_MESSAGE_PAINT_NON_CLIENT, 0, 0);
//BugCheck(NULL, (PCHAR) &"???", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);    
    SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

    // WINDOW_FLAGS flags = GetFlags(hWindow);

    // if (flags & BUTTON_FLAG_IS_DEFAULT)
    // {
    //     SetDefault(hWindow);
    //     SetFocus(hWindow);
    // }

    if (!IsChild(hWindow))
    {
        ActivateWindow(hWindow);
    }
}

VOID HideWindow(HWINDOW hWindow)
{
    // Check if window exist
    if (!IsValid(hWindow))
    {
        return;
    }

    // If already hidden, don't worry
    if (!IsVisible(hWindow))
    {
        return;
    }

    if (hWindow == hDesktopWindow)
    {
        return;
    }

    SetFocus(0);    // This call don't do anything as NULL is an invalid window handle
    SetDefault(0);  // This call don't do anything as NULL is an invalid window handle
    ClearFlag(hWindow, WINDOW_FLAG_IS_VISIBLE);

    if (IsChild(hWindow))
    {
        SendMessage(GetParent(hWindow), WINDOW_MESSAGE_PAINT, 0, 0);
        DrawChildren(GetParent(hWindow));

        return;
    }
    else
    {
        // If not child window it is a toplevel window, so make the next window in line the active window
        if (hActiveWindow == hWindow)
        {
            //SendMessage(hWindow, WINDOW_MESSAGE_DEACTIVATE, 0, 0);   
            //hActiveWindow = GetWindowHandle(GetWindowEntry(hWindow)->pPrev);
            //BringWindowToTop(GetWindowHandle(GetWindowEntry(hWindow)->pPrev));
            //ActivateWindow(hLauncherWindow);
            HWINDOW hToActivate = GetPreviousSibling(hWindow);

            if (hToActivate == (HWINDOW) NULL)
            {
                hToActivate = GetFirstChild(hDesktopWindow);
            }

            ActivateWindow(hToActivate);
            //ActivateWindow(GetWindowHandle(GetWindowEntry(hWindow)->pPrev));
            
        }
    }

    // Take out window from the ZOrder
    //DisableUiUpdate();
    SendMessage(hDesktopWindow, WINDOW_MESSAGE_PAINT, 0, 0);    
    DrawChildren(hDesktopWindow);
    //EnableUiUpdate();
}

BOOL IsOfClass(HWINDOW hWindow, PCHAR pClassName)
{
    if (!IsValid(hWindow))
    {
        return FALSE;
    }

    if (pClassName == NULL)
    {
        BugCheck(NULL, (PCHAR) &"ClassName was null!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);

    }

    GetClassName(hWindow, &WiTextBuffer[0], 255);

    if (strcmp(&WiTextBuffer[0], (const char*) pClassName) == 0)
    {
        return TRUE;
    }

    return FALSE;
}

BOOL IsValid(HWINDOW hWindow)
{
    UiDrawText("IsValid()");

    if (hWindow == (HWINDOW) NULL)
    {
        //BugCheck(NULL, (PCHAR) &"NULL Window Handle.", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);
        //HALT;
        return FALSE;
    }

    PWINDOW pWindow = MemoryLock(hWindow);

    //UiDrawText((PCHAR) &pWindow->WindowName[0]);

    if (pWindow == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Window pointer was null!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);   
    }

    if (pWindow->Signature != WINDOW_SIGNATURE)
    {
        BugCheck(NULL, (PCHAR) &"INVALID Window Handle.", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);

        return FALSE;
    }

    return TRUE;
}

BOOL IsVisible(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return FALSE;
    }

    if (hWindow == hDesktopWindow)
    {
        return TRUE;
    }

    PWINDOW pWindow = GetWindowEntry(hWindow);

    WINDOW_FLAGS flags = pWindow->Flags;

    if (!(flags & WINDOW_FLAG_IS_VISIBLE))
    {
        return FALSE;
    }        

    HWINDOW hParent = GetParent(hWindow);

    return IsVisible(hParent);
}

BOOL IsEnabled(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return FALSE;
    }

    PWINDOW pWindow = GetWindowEntry(hWindow);

    WINDOW_FLAGS flags = pWindow->Flags;

    if (flags & WINDOW_FLAG_IS_DISABLED)    
    {
        return FALSE;
    }

    return TRUE;
}

BOOL IsChild(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return FALSE;
    }

    PWINDOW pWindow = GetWindowEntry(hWindow);

    WINDOW_FLAGS flags = pWindow->Flags;

    if (flags & WINDOW_FLAG_IS_CHILD)    
    {
        return TRUE;
    }

    return FALSE;
}

HWINDOW GetWindowHandle(PWINDOW pWindow)
{
    if (pWindow == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Window pointer was null!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window pointer:", (UINT32) pWindow);   
        //return (HWINDOW) NULL;
    }

    if (pWindow->Signature != WINDOW_SIGNATURE)
    {
        BugCheck(NULL, (PCHAR) &"INVALID Window Pointer.", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window pointer:", (UINT32) pWindow);
        //return (HWINDOW) NULL;
    }

    return pWindow->hWindow;
}

HWINDOW GetParent(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return (HWINDOW) NULL;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL) return (HWINDOW) NULL;

    return pWindowEntry->hParent;
}

HWINDOW GetTopWindow(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return (HWINDOW) NULL;
    }

    while (GetParent(hWindow) != (HWINDOW) NULL && GetParent(hWindow) != hDesktopWindow)
    {
        hWindow = GetParent(hWindow);
    }

    return hWindow;
}

HWINDOW GetFirstChild(HWINDOW hWindowParent)
{
    if (!IsValid(hWindowParent))
    {
        return (HWINDOW) NULL;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindowParent);

    if (pWindowEntry == NULL)
    {
        return (HWINDOW) NULL;
    }

    if (pWindowEntry->pFirstChild == NULL)
    {
        return (HWINDOW) NULL;        
    }

    return GetWindowHandle(pWindowEntry->pFirstChild);
}

HWINDOW GetLastChild(HWINDOW hWindowParent)
{
    if (!IsValid(hWindowParent))
    {
        return (HWINDOW) NULL;
    }

    PWINDOW pWindowEntry = GetWindowEntry(GetFirstChild(hWindowParent));

    if (pWindowEntry == NULL)
    {
        return (HWINDOW) NULL;
    }

    if (pWindowEntry->pNext == NULL)
    {
        return GetWindowHandle(pWindowEntry);
    }

    while(pWindowEntry != NULL)
    {
        if (pWindowEntry->pNext == NULL)
        {
            return GetWindowHandle(pWindowEntry);
        }

        pWindowEntry = pWindowEntry->pNext;
    }

    return (HANDLE) NULL;
}

VOID SetLastChild(HWINDOW hWindowParent, HWINDOW hWindowChild)
{
    if (!IsValid(hWindowParent))
    {
        return;
    }

    PWINDOW pWindowEntry = GetWindowEntry(GetLastChild(hWindowParent));

    if (pWindowEntry == NULL)
    {
        pWindowEntry = GetWindowEntry(hWindowParent);
        pWindowEntry->pFirstChild = GetWindowEntry(hWindowChild);

        return;
    }

    pWindowEntry->pNext = GetWindowEntry(hWindowChild);
}

HWINDOW GetNextSibling(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return (HWINDOW) NULL;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);
    
    if (pWindowEntry == NULL)
    {
        return (HWINDOW) NULL;
    }

    if (pWindowEntry->pNext == NULL)
    {
        return (HWINDOW) NULL;
    }

    return GetWindowHandle(pWindowEntry->pNext);
}

HWINDOW GetPreviousSibling(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return (HWINDOW) NULL;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL)
    {
        return (HWINDOW) NULL;
    }

    if (pWindowEntry->pPrev == NULL)
    {
        return (HWINDOW) NULL;
    }

    return GetWindowHandle(pWindowEntry->pPrev);
}

VOID SetVisible(HWINDOW hWindow, BOOL isVisible)
{
    UiDrawText("SetVisible()");

    if (!IsValid(hWindow))
    {
        return;
    }

    PWINDOW pWindow = GetWindowEntry(hWindow);

    if (isVisible)
    {
        pWindow->Flags |= WINDOW_FLAG_IS_VISIBLE;
    }
    else
    {
        pWindow->Flags &= !WINDOW_FLAG_IS_VISIBLE;
    }
}

PWINDOW GetWindowEntry(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return NULL;
    }

    PWINDOW pWindow = MemoryLock(hWindow);

    return pWindow;
}

// Gets a window's area in screen coordinates (screenLeft, screenTop, screenRight, screenBottom)
KSTATUS GetScreenRectangle(HWINDOW hWindow, OUT PRECTANGLE pRectangle)
{
    // Validation
    if (!IsValid(hWindow))
    {
        return STATUS_ERROR_INVALID_HANDLE;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);
    if (pWindowEntry == NULL) return STATUS_ERROR_OBJECT_NOT_FOUND;

    // Calculation
    POINT parentsPosition;
    parentsPosition.X = 0;
    parentsPosition.Y = 0;
    PointToScreen(GetParent(hWindow), &parentsPosition);

    pRectangle->Left   = parentsPosition.X + pWindowEntry->X;
    pRectangle->Top    = parentsPosition.Y + pWindowEntry->Y;
    pRectangle->Right  = parentsPosition.X + pWindowEntry->X + pWindowEntry->Width;
    pRectangle->Bottom = parentsPosition.Y + pWindowEntry->Y + pWindowEntry->Height;

    return STATUS_SUCCESS;
}

// Retrieves the dimensions of a window's area in client coordinates.
KSTATUS GetWindowRectangle(HWINDOW hWindow, OUT PRECTANGLE pNonClientRectangle)
{
    if (!IsValid(hWindow))
    {
        return STATUS_ERROR_INVALID_HANDLE;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);
    if (pWindowEntry == NULL) return STATUS_ERROR_OBJECT_NOT_FOUND;

    // Calculation
    pNonClientRectangle->Left   = 0;
    pNonClientRectangle->Top    = 0;
    pNonClientRectangle->Right  = pWindowEntry->Width;
    pNonClientRectangle->Bottom = pWindowEntry->Height;

    return STATUS_SUCCESS;
}

// Retrieves the dimensions of a window's client area in client coordinates. 
KSTATUS GetClientRectangle(HWINDOW hWindow, OUT PRECTANGLE pClientRectangle)
{
    if (!IsValid(hWindow))
    {
        return STATUS_ERROR_INVALID_HANDLE;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);
    if (pWindowEntry == NULL) return STATUS_ERROR_OBJECT_NOT_FOUND;

    // Calculation
    GetWindowRectangle(hWindow, pClientRectangle);

    // Limit to only client area
    RectangleReduceToClientArea(hWindow, pClientRectangle);

    // Set top-left coordinate to 0
    pClientRectangle->Right  -= pClientRectangle->Left;
    pClientRectangle->Bottom -= pClientRectangle->Top;
    pClientRectangle->Left    = 0;
    pClientRectangle->Top     = 0;

    return STATUS_SUCCESS;
}

VOID GetWindowPosition(HWINDOW hWindow, OUT PPOINT pPoint)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL) return;

    pPoint->X = pWindowEntry->X;
    pPoint->Y = pWindowEntry->Y;
}

KSTATUS SetWindowRectangle(HWINDOW hWindow, PRECTANGLE pRectangle)
{
    if (!IsValid(hWindow))
    {
        return STATUS_ERROR_INVALID_HANDLE;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL) return STATUS_ERROR_OBJECT_NOT_FOUND;

    if (pWindowEntry->X == pRectangle->Left &&
        pWindowEntry->Y == pRectangle->Top &&
        pWindowEntry->Width  == pRectangle->Right - pRectangle->Left &&
        pWindowEntry->Height == pRectangle->Bottom - pRectangle->Top)
    {
        // No change is position or size
        return STATUS_SUCCESS;
    }

    pWindowEntry->X = pRectangle->Left;
    pWindowEntry->Y = pRectangle->Top;
    pWindowEntry->Width = pRectangle->Right - pRectangle->Left;
    pWindowEntry->Height = pRectangle->Bottom - pRectangle->Top;

    
    SendMessage(hWindow, WINDOW_MESSAGE_MOVED, 0, 0);
    //SendMessage(hWindow, WINDOW_MESSAGE_PAINT_NON_CLIENT, 0, 0);
    //SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

    // DrawChildren(hWindow);

    MachineDisableInterrupts();    
    //DisableUiUpdate();


    SendMessage(hDesktopWindow, WINDOW_MESSAGE_PAINT, 0, 0);
    DrawChildren(hDesktopWindow);

    //EnableUiUpdate();
    MachineEnableInterrupts();


    return STATUS_SUCCESS;
}

VOID SetWindowPosition(HWINDOW hWindow, PPOINT pPoint)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL) return;

    pWindowEntry->X = pPoint->X;
    pWindowEntry->Y = pPoint->Y;


    SendMessage(hWindow, WINDOW_MESSAGE_MOVED, 0, 0);
    // SendMessage(hDesktopWindow, WINDOW_MESSAGE_PAINT, 0, 0);
    // SendMessage(hWindow, WINDOW_MESSAGE_PAINT_NON_CLIENT, 0, 0);
    // SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);
    // DrawChildren(hWindow);

    SendMessage(hDesktopWindow, WINDOW_MESSAGE_PAINT, 0, 0);
    DrawChildren(hDesktopWindow);
   
}

VOID DrawChildren(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    if (!IsVisible(hWindow))
    {
        return;
    }

    HWINDOW hWindowChild = GetFirstChild(hWindow);

    while(hWindowChild != (HWINDOW) NULL)
    {
        if (IsVisible(hWindowChild))
        {
            SendMessage(hWindowChild, WINDOW_MESSAGE_PAINT_NON_CLIENT, 0, 0);
            SendMessage(hWindowChild, WINDOW_MESSAGE_PAINT, 0, 0);

            DrawChildren(hWindowChild);
        }

        hWindowChild = GetNextSibling(hWindowChild);
    }
}

// Reduces the area occupied by the borders and caption to the coordinates
// This function is being used by the Graphical Interface Manager
VOID RectangleReduceToClientArea(HWINDOW hWindow, PRECTANGLE pRectangle)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    if (GetFlags(hWindow) & WINDOW_FLAG_HAS_BORDER || GetFlags(hWindow) & WINDOW_FLAG_HAS_SIZING_BORDER || GetFlags(hWindow) & WINDOW_FLAG_HAS_DIALOG_BORDER)
    {
        if (GetFlags(hWindow) & WINDOW_FLAG_HAS_SIZING_BORDER || GetFlags(hWindow) & WINDOW_FLAG_HAS_DIALOG_BORDER)
        {
            pRectangle->Left   += GetSystemMetric(METRIC_BORDER_SIZE);
            pRectangle->Top    += GetSystemMetric(METRIC_BORDER_SIZE);
            pRectangle->Right  -= GetSystemMetric(METRIC_BORDER_SIZE);                
            pRectangle->Bottom -= GetSystemMetric(METRIC_BORDER_SIZE);
        }
        else
        {
            pRectangle->Left   += GetSystemMetric(METRIC_FRAME_SIZE);
            pRectangle->Top    += GetSystemMetric(METRIC_FRAME_SIZE);
            pRectangle->Right  -= GetSystemMetric(METRIC_FRAME_SIZE);
            pRectangle->Bottom -= GetSystemMetric(METRIC_FRAME_SIZE);
        }
    }

    if (GetFlags(hWindow) & WINDOW_FLAG_HAS_CAPTION)
    {
        pRectangle->Top += GetSystemMetric(METRIC_WINDOW_TITLE_BAR_SIZE);
    }
}

VOID PointToScreen(HWINDOW hWindow, OUT PPOINT pPoint)
{
    // Validation
    if (!IsValid(hWindow))
    {
        return;
    }

    PWINDOW pWindow = GetWindowEntry(hWindow);

    // Calculation
    WINDOW_FLAGS flags = GetFlags(hWindow);

    if (flags & WINDOW_FLAG_HAS_SIZING_BORDER ||
        flags & WINDOW_FLAG_HAS_DIALOG_BORDER)
    {
        pPoint->X += GetSystemMetric(METRIC_BORDER_SIZE);
        pPoint->Y += GetSystemMetric(METRIC_BORDER_SIZE);
    }
    else if (flags & WINDOW_FLAG_HAS_BORDER)
    {
        pPoint->X += GetSystemMetric(METRIC_FRAME_SIZE);
        pPoint->Y += GetSystemMetric(METRIC_FRAME_SIZE);
    }

    if (flags & WINDOW_FLAG_HAS_CAPTION)
    {
        pPoint->Y += GetSystemMetric(METRIC_WINDOW_TITLE_BAR_SIZE);
    }    

    pPoint->X += pWindow->X;
    pPoint->Y += pWindow->Y;

    HWINDOW hParent = GetParent(hWindow);

    if (hParent != (HWINDOW) NULL)
    {
        PointToScreen(hParent, pPoint);
    }    
}

VOID PointToClient(HWINDOW hWindow, OUT PPOINT pPoint)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    RECTANGLE windowRectangle;

    // THIS SHOULD BE REPLACED BY A GETCLIENTRECTANGLE + RECTANGLETOSCREEN CALL
    GetScreenRectangle(hWindow, &windowRectangle);
    RectangleReduceToClientArea(hWindow, &windowRectangle);

    pPoint->X = pPoint->X - windowRectangle.Left;
    pPoint->Y = pPoint->Y - windowRectangle.Top;
}

VOID PointScreenToNonClient(HWINDOW hWindow, OUT PPOINT pPoint)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    RECTANGLE windowRectangle;
    GetScreenRectangle(hWindow, &windowRectangle);

    pPoint->X -= windowRectangle.Left;
    pPoint->Y -= windowRectangle.Top;
}

WINDOW_FLAGS GetFlags(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return 0;
    }

    WINDOW_FLAGS flags = 0;
    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL) return 0;

    flags = pWindowEntry->Flags;

    return flags;
}

VOID SetFlags(HWINDOW hWindow, WINDOW_FLAGS flags)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL) return;

    pWindowEntry->Flags = flags;
}

VOID SetFlag(HWINDOW hWindow, WINDOW_FLAGS flag)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Window entry was not found!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);     
        //return NULL;
    } 

    pWindowEntry->Flags |= flag;
}

VOID ClearFlag(HWINDOW hWindow, WINDOW_FLAGS flag)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Window entry was not found!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);     
        //return NULL;
    } 

    pWindowEntry->Flags &= ~flag;
}

BOOL HasFlag(HWINDOW hWindow, WINDOW_FLAGS flag)
{
    if (!IsValid(hWindow))
    {
        return FALSE;
    }

    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Window entry was not found!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);     
        //return NULL;
    } 

    if (pWindowEntry->Flags & flag)
    {
        return TRUE;
    }

    return FALSE;
}

PCHAR GetWindowTextPointer(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return NULL;
    }

    PCHAR pWindowText;
    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Window entry was not found!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);     
        //return NULL;
    } 

    pWindowText = (PCHAR) &pWindowEntry->WindowName;

    return pWindowText;
}

VOID GetWindowText(HWINDOW hWindow, OUT PCHAR pBuffer, UINT32 bufferLength)
{
    if (!IsValid(hWindow))
    {
        pBuffer[0] = '\0';

        return;
    }

    PCHAR pWindowText;
    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Window entry was not found!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);     
        //return NULL;
    } 

    pWindowText = (PCHAR) &pWindowEntry->WindowName;

    strncpy(&pBuffer[0], pWindowText, bufferLength-1);
    pBuffer[bufferLength] = '\0';
}

VOID SetWindowText(HWINDOW hWindow, PCHAR pText)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    PCHAR pWindowText;
    PWINDOW pWindowEntry = GetWindowEntry(hWindow);

    if (pWindowEntry == NULL)
    {
        BugCheck(NULL, (PCHAR) &"Window entry was not found!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Window handle:", hWindow);     
        //return NULL;
    } 

    pWindowText = (PCHAR) &pWindowEntry->WindowName;

    UINT32 textLength = strlen(pText);

    if (textLength > MAX_WINDOW_NAME_LENGTH)
    {
        textLength = MAX_WINDOW_NAME_LENGTH;
    }

    strncpy(&pWindowText[0], pText, textLength);
    pWindowText[textLength] = '\0';
}

HWINDOW GetTopLevelWindow(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return (HWINDOW) NULL;
    }

    if (hWindow == hDesktopWindow)
    {
        return hWindow;
    }

    HWINDOW hParent = GetParent(hWindow);

    if (hParent == hDesktopWindow)
    {
        return hWindow;
    }

    if (hParent == (HWINDOW) NULL)
    {
        BugCheck(NULL, (PCHAR) &"Parent window handle is null, but not Desktop window?", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Memory handle:", hParent);   
    }

    return GetTopLevelWindow(hParent);
}

HWINDOW GetTopMostWindowUnderPoint(INT32 pointX, INT32 pointY)
{
    PWINDOW pDesktopWindow;
    PWINDOW pTopWindow;

    pDesktopWindow = GetWindowEntry(hDesktopWindow);

    pTopWindow = GetTopMostWindowUnderPointInChildren(pDesktopWindow, pDesktopWindow, pointX, pointY);

    return GetWindowHandle(pTopWindow);
}

PWINDOW GetTopMostWindowUnderPointInChildren(PWINDOW pWindow, PWINDOW pTopWindow, INT32 pointX, INT32 pointY)
{
    while(pWindow != NULL)
    {
        if (pWindow->Flags & WINDOW_FLAG_IS_VISIBLE)
        {
            RECTANGLE rectangle;
            GetScreenRectangle(pWindow->hWindow, &rectangle);
        
            if (rectangle.Left   <= pointX &&
                rectangle.Right  >= pointX &&
                rectangle.Top    <= pointY &&
                rectangle.Bottom >= pointY)
            {
                pTopWindow = pWindow;
            }

            if (pWindow->pFirstChild != NULL)
            {
                pTopWindow = GetTopMostWindowUnderPointInChildren(pWindow->pFirstChild, pTopWindow, pointX, pointY);
            }
        }

        pWindow = pWindow->pNext;
    }

    return pTopWindow;
}

VOID ActivateWindow(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    if (IsChild(hWindow))
    {
        // Only top level windows can be activated
        return;
    }

    //if (IsChild(hWindow))
    //if (GetParent(hWindow) != hDesktopWindow) 
    //if (GetParent(hWindow) != (HWINDOW) NULL) 
    {
        //ActivateWindow(GetParent(hWindow));

        //return;
    }

    if (hActiveWindow == hWindow)
    {
        return;
    }

    if (hActiveWindow != (HWINDOW) NULL)
    {
        
        // Deactivate previous active window
        SendMessage(hActiveWindow, WINDOW_MESSAGE_DEACTIVATE, 0, 0);
        SendMessage(hActiveWindow, WINDOW_MESSAGE_PAINT_NON_CLIENT, 0, 0);

        PWINDOW pActiveWindow = GetWindowEntry(GetTopLevelWindow(hActiveWindow));

        HWINDOW hLastFocused = pActiveWindow->hLastFocused;
        HWINDOW hLastDefault = pActiveWindow->hLastDefault;

        SendMessage(hLastFocused, WINDOW_MESSAGE_FOCUS_LOST, 0, 0);
        SendMessage(hLastDefault, WINDOW_MESSAGE_DEFAULT_LOST, 0, 0);
    }

    SendMessage(hWindow, WINDOW_MESSAGE_ACTIVATE, 0, 0);

    if (hWindow != hDesktopWindow)
    {
        BringWindowToTop(hWindow);

        // Draw Active Window
        SendMessage(hWindow, WINDOW_MESSAGE_PAINT_NON_CLIENT, 0, 0);
        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);
        DrawChildren(hWindow);
    }

    hActiveWindow = hWindow;

    // Set Focus & Default controls
    PWINDOW pWindow = GetWindowEntry(hWindow);

    HWINDOW hLastFocused = pWindow->hLastFocused;
    HWINDOW hLastDefault = pWindow->hLastDefault;

    SetFocus(hLastFocused);
    SetDefault(hLastDefault);
}

HWINDOW GetActiveWindow()
{
    return hActiveWindow;
}

HWINDOW GetFocusedWindow()
{
    return hFocusedWindow;
}

// Handles any mouse event that occured between calls
VOID HandleMouseEvents()
{
    HandleMouseMove();
    HandleMouseLeftButtonChange();
}

// Handles when the mouse was moved, sends out messages to the affected windows
VOID HandleMouseMove()
{
    GetMousePosition(&mouseX, &mouseY);
    hWindowUnderMouse = GetTopMostWindowUnderPoint(mouseX, mouseY);

    if (hWindowMouseCapture == (HWINDOW) NULL)
    {
        // Normal mouse operation (mouse not captured by any window)
        POINT clientMousePosition;
        clientMousePosition.X = mouseX;
        clientMousePosition.Y = mouseY;

        RECTANGLE windowRectangle;
        GetScreenRectangle(hWindowUnderMouse, &windowRectangle);

        clientMousePosition.X -= windowRectangle.Left;
        clientMousePosition.Y -= windowRectangle.Top;

        // Send mouse leave and enter messages
        if (hLastWindowUnderMouse != hWindowUnderMouse)
        {
            SendMessage(hLastWindowUnderMouse, WINDOW_MESSAGE_MOUSE_LEAVE, 0, 0);
            SendMessage(hWindowUnderMouse, WINDOW_MESSAGE_MOUSE_ENTER, (UINT32) &clientMousePosition, 0);

            hLastWindowUnderMouse = hWindowUnderMouse;
        }        

        // Send mouse move message

        SendMessage(hWindowUnderMouse, WINDOW_MESSAGE_MOUSE_MOVE, (UINT32) &clientMousePosition, 0);

        return;
    }

    // Mouse captured
    POINT clientMousePosition;
    clientMousePosition.X = mouseX;
    clientMousePosition.Y = mouseY;

    RECTANGLE windowRectangle;
    GetScreenRectangle(hWindowMouseCapture, &windowRectangle);

    clientMousePosition.X -= windowRectangle.Left;
    clientMousePosition.Y -= windowRectangle.Top;

    // Send mouse leave and enter messages
    if (hWindowUnderMouse != hWindowMouseCapture && hLastWindowUnderMouse != hWindowUnderMouse)
    {
        SendMessage(hWindowMouseCapture, WINDOW_MESSAGE_MOUSE_LEAVE, 0, 0);

        hLastWindowUnderMouse = hWindowUnderMouse;
    }

    if (hWindowUnderMouse == hWindowMouseCapture && hLastWindowUnderMouse != hWindowUnderMouse)
    {
        SendMessage(hWindowMouseCapture, WINDOW_MESSAGE_MOUSE_ENTER, (UINT32) &clientMousePosition, 0);

        hLastWindowUnderMouse = hWindowUnderMouse;
    }

    // Even the mouse is outside of the window that captured the mouse send move messages

    SendMessage(hWindowMouseCapture, WINDOW_MESSAGE_MOUSE_MOVE, (UINT32) &clientMousePosition, 0);

    return;
}

// Handles when the mouse left button was pressed/released, sends out messages to the affected windows
VOID HandleMouseLeftButtonChange()
{
    // Get which window the mouse is over
    GetMouseLeftButtonState(&isMouseLeftButtonDown);

    if (isMouseLeftButtonDown == isMouseLeftButtonDownOld)
    {
        // Button state unchanged
        return;
    }

    if (hWindowMouseCapture == (HWINDOW) NULL)
    {
        // Normal mouse operation
        POINT clientMousePosition;
        clientMousePosition.X = mouseX;
        clientMousePosition.Y = mouseY;

        RECTANGLE windowRectangle;
        GetScreenRectangle(hWindowUnderMouse, &windowRectangle);

        clientMousePosition.X -= windowRectangle.Left;
        clientMousePosition.Y -= windowRectangle.Top;

        // Send mouse button down/up events to the right window
        if (isMouseLeftButtonDown)
        {
            if (IsChild(hWindowUnderMouse))
            {
                ActivateWindow(GetTopLevelWindow(hWindowUnderMouse));
            }
            else
            {
                ActivateWindow(hWindowUnderMouse);
            }

            SendMessage(hWindowUnderMouse, WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_DOWN, (UINT32) &clientMousePosition, 0);
        }
        else
        {
            SendMessage(hWindowUnderMouse, WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_UP, (UINT32) &clientMousePosition, 0);
        }

        isMouseLeftButtonDownOld = isMouseLeftButtonDown;

        return;
    }

    // Mouse captured
    POINT clientMousePosition;
    clientMousePosition.X = mouseX;
    clientMousePosition.Y = mouseY;

    RECTANGLE windowRectangle;
    GetScreenRectangle(hWindowMouseCapture, &windowRectangle);

    clientMousePosition.X -= windowRectangle.Left;
    clientMousePosition.Y -= windowRectangle.Top;

    // Send mouse button down/up events to the right window
    if (isMouseLeftButtonDown)
    {
        if (hWindowUnderMouse == hWindowMouseCapture)
        {
            ActivateWindow(hWindowMouseCapture);
            SendMessage(hWindowMouseCapture, WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_DOWN, (UINT32) &clientMousePosition, 0);
        }
    }
    else
    {
        SendMessage(hWindowMouseCapture, WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_UP, (UINT32) &clientMousePosition, 0);
    }

    isMouseLeftButtonDownOld = isMouseLeftButtonDown;
}


// Handles any keyboard event that occured between calls
VOID HandleKeyboardEvents()
{
    KeyboardProcessKey();
}

HWINDOW GetCapture()
{
    return hWindowMouseCapture;
}

VOID SetCapture(HWINDOW hWindow)
{
    if (hWindowMouseCapture == (HWINDOW) NULL)
    {
        if (IsValid(hWindow))
        {
            hWindowMouseCapture = hWindow;
        }       
    }
}

VOID ReleaseCapture()
{
    hWindowMouseCapture = (HWINDOW) NULL;
}

VOID SetFocus(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    if (!IsChild(hWindow))
    {
        return;
    }

    PWINDOW pWindow = GetWindowEntry(GetTopLevelWindow(hWindow));

    //if (hWindow != hWindowFocus)
    {
        if (pWindow->hLastFocused != (HWINDOW) NULL)
        {
            SendMessage(pWindow->hLastFocused, WINDOW_MESSAGE_FOCUS_LOST, 0, 0);
        }

        if (hWindow != (HWINDOW) NULL)
        {
            SendMessage(hWindow, WINDOW_MESSAGE_FOCUS_GAINED, 0, 0);
        }

        pWindow->hLastFocused = hWindow;
        hFocusedWindow = hWindow;
    }
}

VOID SetDefault(HWINDOW hWindow)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    if (!IsChild(hWindow))
    {
        return;
    }

    PWINDOW pWindow = GetWindowEntry(GetTopLevelWindow(hWindow));
    // Should we check whether the top level window is the actual active window?

    //if (hWindow != hWindowDefault)
    {
        if (pWindow->hLastDefault != (HWINDOW) NULL)
        {
            SendMessage(pWindow->hLastDefault, WINDOW_MESSAGE_DEFAULT_LOST, 0, 0);
        }

        if (hWindow != (HWINDOW) NULL)
        {
            SendMessage(hWindow, WINDOW_MESSAGE_DEFAULT_GAINED, 0, 0);
        }

        pWindow->hLastDefault = hWindow;
    }
}

VOID BringWindowToTop(HWINDOW hWindow)
{
    // Validation
    if (!IsValid(hWindow))
    {
        return;
    }

    if (hWindow == hDesktopWindow)
    {
        return;
    }

    // if (IsChild(hWindow))
    // {
    //     return;
    // }

    HWINDOW hParent       = GetParent(hWindow);
    HWINDOW hPrevSibling  = GetPreviousSibling(hWindow);
    HWINDOW hNextSibling  = GetNextSibling(hWindow);
    HWINDOW hFirstSibling = GetFirstChild(hParent);
    HWINDOW hLastSibling  = GetLastChild(hParent);


    PWINDOW pWindow       = GetWindowEntry(hWindow);
    PWINDOW pParent       = GetWindowEntry(hParent);
    PWINDOW pFirstSibling = GetWindowEntry(hFirstSibling);
    PWINDOW pPrevSibling  = GetWindowEntry(hPrevSibling);
    PWINDOW pNextSibling  = GetWindowEntry(hNextSibling);
    PWINDOW pLastSibling  = GetWindowEntry(hLastSibling);

    if (pLastSibling == NULL)
    {
        // WindowStack empty ???

        return;
    }

    if (pWindow == pLastSibling)
    {
        // Already top of the stack

        return;
    }

    if (pWindow == pFirstSibling)
    {
        if (pLastSibling != NULL)
        {
            pNextSibling->pPrev = NULL;
            pParent->pFirstChild = pNextSibling;
            
            pLastSibling->pNext = pWindow;
            pWindow->pPrev = pLastSibling;
            pWindow->pNext = NULL;
        }

        return;
    }

    // Move window to the top of the stack
    //if (pPrevSibling != NULL && pWindow->pNext != NULL && pPrevSibling->pNext != pWindow)
    if (pWindow->pNext != NULL)
    {
        if (pPrevSibling == NULL)
        {
            BugCheck(NULL, (PCHAR) &"pPrevSibling window pointer was null!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "pPrevSibling:", (UINT32) pPrevSibling);
        }

        pPrevSibling->pNext = pWindow->pNext;
        ((PWINDOW)(pWindow->pNext))->pPrev = pPrevSibling;
    }


    if (pLastSibling == NULL)
    {
        BugCheck(NULL, (PCHAR) &"pLastSibling window pointer was null!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "pLastSibling:", (UINT32) pLastSibling);
    }

    pLastSibling->pNext = pWindow;
    pWindow->pPrev = pLastSibling;
    pWindow->pNext = NULL;

    // if (pWindow->pNext != NULL && pLastSibling->pNext != pWindow)
    // {
        
        
    // }

    // pWindow is the last in the order
}

VOID ResetWindows()
{
    HideMouse();

    // Reset desktop dimensions
    VIDEO_MODE_INFORMATION videoModeInfo;
    GetCurrentVideoModeInformation(GetPrimaryDisplayDevice(), &videoModeInfo);
    UINT32 desktopWidth = videoModeInfo.Width;
    UINT32 desktopHeight = videoModeInfo.Height;

    RECTANGLE desktopRectangle;
    desktopRectangle.Left   = 0;
    desktopRectangle.Top    = 0;
    desktopRectangle.Right  = desktopWidth;
    desktopRectangle.Bottom = desktopHeight;
    SetWindowRectangle(hDesktopWindow, &desktopRectangle);

    // Redraw
    SendMessage(hDesktopWindow, WINDOW_MESSAGE_PAINT, 0, 0);
    DrawChildren(hDesktopWindow);
    SetMousePosition(desktopWidth / 2, desktopHeight / 2);

    ShowMouse();
}

VOID BroadcastMessage(HWINDOW hWindow, WINDOW_MESSAGE message)
{
    if (!IsValid(hWindow))
    {
        return;
    }

    if (!IsVisible(hWindow))
    {
        return;
    }

    HWINDOW hWindowChild = GetFirstChild(hWindow);

    while(hWindowChild != (HWINDOW) NULL)
    {
        if (IsVisible(hWindowChild))
        {
            SendMessage(hWindowChild, message, 0, 0);

            BroadcastMessage(hWindowChild, message);
        }

        hWindowChild = GetNextSibling(hWindowChild);
    }
}