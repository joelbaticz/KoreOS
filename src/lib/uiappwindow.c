#include "kldr.h"

static CHAR UiAwWindowClass[]        = "WINDOW";

//static CHAR UiAwTextBuffer[50];

static BOOL        UawIsActive;
static HWINDOW     hUawDraggedWindow;
static POINT       UawClientMousePosition;
static POINT       UawPreviousMousePosition;
static RECTANGLE   UawPreviousDragRectangle;

VOID DrawModernWindow(HWINDOW hWindow);
VOID DrawWindow95StyleWindow(HWINDOW hWindow);

VOID RegisterApplicationWindow()
{
    UawIsActive = FALSE;
    hUawDraggedWindow = (HWINDOW) NULL;
    UawClientMousePosition.X = 0;
    UawClientMousePosition.Y = 0;

    RegisterWindow((PCHAR) &UiAwWindowClass, 0, 0, 0, 0, 255, (PVOID) DefaultWindowProcedure, 0);
}

UINT32 DefaultWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    UiDrawText("DefaultWindowProcedure()");
    if (message == WINDOW_MESSAGE_CREATE)
    {
        // Create default buttons, like Close, Min, Max, ContextMenu
    }

    if (message == WINDOW_MESSAGE_PAINT_NON_CLIENT)
    {
        //DrawModernWindow(hWindow);
        DrawWindow95StyleWindow(hWindow);

        return 0;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEAVE)
    {
        if (hUawDraggedWindow != (HWINDOW) NULL)
        {
            return 0;
        }

        HDEVICE_CONTEXT hDeviceContext = GetDeviceContextNonClient(hWindow);
        DrawBegin(hDeviceContext);

        RECTANGLE windowRectangle;
        GetWindowRectangle(hWindow, &windowRectangle);

        // UINT32 left = 0;
        // UINT32 top = 0;
        // UINT32 right = windowRectangle.Right - windowRectangle.Left;
        // UINT32 bottom = windowRectangle.Bottom - windowRectangle.Top;  

        SetForegroundColor(hDeviceContext, 0xFF000000);
        //DrawRectangleA(hDeviceContext, left, top, right, bottom);
        //DrawControlState(hDeviceContext, windowRectangle.Left, windowRectangle.Top, windowRectangle.Right, windowRectangle.Bottom, DRAW_CONTROL_STATE_EDGE_WINDOW);

        DrawEnd(hDeviceContext);
        ReleaseDeviceContext(hDeviceContext);

        return 0;
    }

    if (message == WINDOW_MESSAGE_MOUSE_ENTER)
    {
        if (hUawDraggedWindow != (HWINDOW) NULL)
        {
            return 0;
        }

        HDEVICE_CONTEXT hDeviceContext = GetDeviceContextNonClient(hWindow);
        DrawBegin(hDeviceContext);

        RECTANGLE windowRectangle;
        GetScreenRectangle(hWindow, &windowRectangle);

        UINT32 left = 0;
        UINT32 top = 0;
        UINT32 right = windowRectangle.Right - windowRectangle.Left;
        UINT32 bottom = windowRectangle.Bottom - windowRectangle.Top;  

        SetForegroundColor(hDeviceContext, 0xFFFF0000);
        //DrawRectangleA(hDeviceContext, left, top, right, bottom);

        DrawEnd(hDeviceContext);
        ReleaseDeviceContext(hDeviceContext);

        return 0;
    }

    if (message == WINDOW_MESSAGE_ACTIVATE)
    {
        // WINDOW_FLAGS flags = GetFlags(hWindow);
        // flags |= WINDOW_FLAG_IS_ACTIVE;
        // SetWindowFlags(hWindow, flags);

        SetFlag(hWindow, WINDOW_FLAG_IS_ACTIVE);
        
    }

    if (message == WINDOW_MESSAGE_DEACTIVATE)
    {
        // WINDOW_FLAGS flags = GetFlags(hWindow);
        // flags &= ~WINDOW_FLAG_IS_ACTIVE;
        // SetWindowFlags(hWindow, flags);
        
        ClearFlag(hWindow, WINDOW_FLAG_IS_ACTIVE);
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_DOWN)
    {
        POINT mousePosition = ((PPOINT) param1)[0];

        if (!(GetFlags(hWindow) & WINDOW_FLAG_HAS_CAPTION))
        {
            // No caption, cannot move
            return 0;
        }

        RECTANGLE titleBarRectangle;
        GetWindowRectangle(hWindow, &titleBarRectangle);
        titleBarRectangle.Left   += 3;
        titleBarRectangle.Top    += 3;
        titleBarRectangle.Right  -= 3;                
        titleBarRectangle.Bottom = titleBarRectangle.Top + GetSystemMetric(METRIC_WINDOW_TITLE_BAR_SIZE);

        if (!CheckIfPointInRectangle(&titleBarRectangle, &mousePosition))
        {
            // Mouse is not in the titlebar area
            return 0;
        }

        PointToScreen(hWindow, &mousePosition);
        hUawDraggedWindow = hWindow;

        // Draw the drag rectangle

        RECTANGLE windowRectangle;
        HDEVICE_CONTEXT hDeviceContext = GetDeviceContextNonClient(hDesktopWindow);
        GetScreenRectangle(hWindow, &windowRectangle);
        
        UINT32 left = windowRectangle.Left;
        UINT32 top = windowRectangle.Top;
        UINT32 right = windowRectangle.Right;
        UINT32 bottom = windowRectangle.Bottom;  

        UawPreviousDragRectangle = windowRectangle;
        UawClientMousePosition = mousePosition;

        DrawBegin(hDeviceContext);

        SetForegroundColor(hDeviceContext, 0xFFFFFFFF);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_WINDOW_RESIZE_FRAME_THIN);

        DrawEnd(hDeviceContext);
        ReleaseDeviceContext(hDeviceContext);

        SetCapture(hWindow);
    }
    
    if (message == WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_UP)
    {
        if (hUawDraggedWindow != hWindow)
        {
            return 0;
        }

        hUawDraggedWindow = (HWINDOW) NULL;

        // Draw move rectangle

        HDEVICE_CONTEXT hDeviceContext = GetDeviceContextNonClient(hDesktopWindow);
        
        UINT32 left = UawPreviousDragRectangle.Left;
        UINT32 top = UawPreviousDragRectangle.Top;
        UINT32 right = UawPreviousDragRectangle.Right;
        UINT32 bottom = UawPreviousDragRectangle.Bottom;  

        DrawBegin(hDeviceContext);

        SetForegroundColor(hDeviceContext, 0xFFFFFFFF);
        DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_WINDOW_RESIZE_FRAME_THIN);

        DrawEnd(hDeviceContext);
        ReleaseDeviceContext(hDeviceContext);

        SetWindowRectangle(hWindow, &UawPreviousDragRectangle);

        ReleaseCapture();
    }
    
    if (message == WINDOW_MESSAGE_MOUSE_MOVE)
    {
        if (hUawDraggedWindow != hWindow)
        {
            return 0;
        }

        POINT     mousePosition = ((PPOINT) param1)[0];
        POINT     offset;
        RECTANGLE windowRectangle;
        GetScreenRectangle(hWindow, &windowRectangle);
        PointToScreen(hWindow, &mousePosition);

        offset.X = mousePosition.X - UawClientMousePosition.X;
        offset.Y = mousePosition.Y - UawClientMousePosition.Y;

        windowRectangle.Left += offset.X;
        windowRectangle.Right += offset.X;
        windowRectangle.Top += offset.Y;
        windowRectangle.Bottom += offset.Y;

        if (windowRectangle.Left == UawPreviousDragRectangle.Left &&
            windowRectangle.Top == UawPreviousDragRectangle.Top &&
            windowRectangle.Right == UawPreviousDragRectangle.Right &&
            windowRectangle.Bottom == UawPreviousDragRectangle.Bottom)
        {
            return 0;
        }
       
        HDEVICE_CONTEXT hDeviceContextScreen = GetDeviceContextNonClient((HDEVICE_CONTEXT) hDesktopWindow);

        DrawBegin(hDeviceContextScreen);

        SetForegroundColor(hDeviceContextScreen, 0xFFFFFFFF);

        UINT32 left = UawPreviousDragRectangle.Left;
        UINT32 top = UawPreviousDragRectangle.Top;
        UINT32 right = UawPreviousDragRectangle.Right;
        UINT32 bottom = UawPreviousDragRectangle.Bottom;  

        DrawControlState(hDeviceContextScreen, left, top, right, bottom, DRAW_CONTROL_STATE_WINDOW_RESIZE_FRAME_THIN);

        left = windowRectangle.Left;
        top = windowRectangle.Top;
        right = windowRectangle.Right;
        bottom = windowRectangle.Bottom;  

        // Show window content while dragging
        POINT newLocation;
        newLocation.X = left;
        newLocation.Y = top;

        //SetWindowPosition(hWindow, &newLocation);
        //SendMessage(hWindow, WINDOW_MESSAGE_PAINT_NON_CLIENT, 0, 0);  // This is ok to do as they will grab NON-CLIENT and CLIENT DC respectively
        //SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        //

        DrawControlState(hDeviceContextScreen, left, top, right, bottom, DRAW_CONTROL_STATE_WINDOW_RESIZE_FRAME_THIN);

        DrawEnd(hDeviceContextScreen);
        ReleaseDeviceContext(hDeviceContextScreen);

        UawPreviousDragRectangle = windowRectangle;
    }

    if (message == WINDOW_MESSAGE_COMMAND)
    {
        if (param1 == COMMAND_RADIOBUTTON_CLICKED)
        {
            HWINDOW hChildWindow = GetFirstChild(hWindow);
            HWINDOW hRadiobutton = (HWINDOW) param2;

            while(hChildWindow != (HWINDOW) NULL)
            {
                if (hChildWindow != hRadiobutton && IsOfClass(hChildWindow, "RADIOBUTTON"))
                {
                    // Child window is of Radiobutton class
                    ClearFlag(hChildWindow, RADIOBUTTON_FLAG_IS_CHECKED);

                    SendMessage(hChildWindow, WINDOW_MESSAGE_PAINT, 0, 0);
                }

                hChildWindow = GetNextSibling(hChildWindow);
            }
        }
    }

    if (message == WINDOW_MESSAGE_KEYDOWN)
    {
        UINT8 virtualKey = (UINT8) param1;

        if (HasFlag(hWindow, WINDOW_FLAG_IS_CHILD))
        {
            if (virtualKey == VKEY_TAB)
            {
                HWINDOW hNextSibling = GetNextSibling(hWindow);

                if (hNextSibling != (HWINDOW) NULL)
                {
                    SetFocus(hNextSibling);
                }
            }
        }

        if (virtualKey == VKEY_ESCAPE)
        {
            HideWindow(GetTopWindow(hWindow));
            //DestroyWindow(hWindow);
        }
    }
}

VOID DrawModernWindow(HWINDOW hWindow)
{    
    INT32 left;
    INT32 top;
    INT32 right;
    INT32 bottom;

    HDEVICE_CONTEXT hDeviceContext = GetDeviceContextNonClient(hWindow);
    DrawBegin(hDeviceContext);

    RECTANGLE windowRectangle;
    GetScreenRectangle(hWindow, &windowRectangle);

    windowRectangle.Right -= windowRectangle.Left;
    windowRectangle.Left -= windowRectangle.Left;
    windowRectangle.Bottom -= windowRectangle.Top;
    windowRectangle.Top -= windowRectangle.Top;

    left = windowRectangle.Left;
    top = windowRectangle.Top;
    right = windowRectangle.Right - windowRectangle.Left;
    bottom = windowRectangle.Bottom - windowRectangle.Top;;

    if (UawIsActive == TRUE)
    {
        SetBackgroundColor(hDeviceContext, ColorWindowActiveBorder);
        SetForegroundColor(hDeviceContext, ColorWindowActiveFrame);
        SetTextColor(hDeviceContext, ColorWindowActiveText);
    }
    else
    {
        SetBackgroundColor(hDeviceContext, ColorWindowInactiveBorder);
        SetForegroundColor(hDeviceContext, ColorWindowInactiveFrame);
        SetTextColor(hDeviceContext, ColorWindowInactiveText);
    }

    if (GetFlags(hWindow) & WINDOW_FLAG_HAS_SIZING_BORDER || GetFlags(hWindow) & WINDOW_FLAG_HAS_DIALOG_BORDER)
    {
        // Top
        DrawFilledRectangleA(hDeviceContext, left, top, right - GetSystemMetric(METRIC_BORDER_SIZE), top + GetSystemMetric(METRIC_BORDER_SIZE));
        
        // Bottom
        DrawFilledRectangleA(hDeviceContext, left + GetSystemMetric(METRIC_BORDER_SIZE), bottom - GetSystemMetric(METRIC_BORDER_SIZE), right, bottom);

        // Left
        DrawFilledRectangleA(hDeviceContext, left, top + GetSystemMetric(METRIC_BORDER_SIZE), left + GetSystemMetric(METRIC_BORDER_SIZE), bottom);

        // Right
        DrawFilledRectangleA(hDeviceContext, right - GetSystemMetric(METRIC_BORDER_SIZE), top, right, bottom - GetSystemMetric(METRIC_BORDER_SIZE));
    }

    if (GetFlags(hWindow) & WINDOW_FLAG_HAS_BORDER || GetFlags(hWindow) & WINDOW_FLAG_HAS_SIZING_BORDER || GetFlags(hWindow) & WINDOW_FLAG_HAS_DIALOG_BORDER)
    {
        for(UINT32 i = 0; i < GetSystemMetric(METRIC_FRAME_SIZE); i++)
        {
            DrawRectangleA(hDeviceContext, left + i, top + i, right - i, bottom - i);
        }
    }

    if (GetFlags(hWindow) & WINDOW_FLAG_HAS_CAPTION)
    {
        windowRectangle.Bottom = windowRectangle.Top + GetSystemMetric(METRIC_WINDOW_TITLE_BAR_SIZE);

        if (GetFlags(hWindow) & WINDOW_FLAG_HAS_SIZING_BORDER || GetFlags(hWindow) & WINDOW_FLAG_HAS_DIALOG_BORDER)
        {
            windowRectangle.Left   += GetSystemMetric(METRIC_BORDER_SIZE);
            windowRectangle.Top    += GetSystemMetric(METRIC_BORDER_SIZE);
            windowRectangle.Bottom += GetSystemMetric(METRIC_BORDER_SIZE);
            windowRectangle.Right  -= GetSystemMetric(METRIC_BORDER_SIZE);                
        }
        else if (GetFlags(hWindow) & WINDOW_FLAG_HAS_BORDER)
        {
            windowRectangle.Left   += GetSystemMetric(METRIC_FRAME_SIZE);
            windowRectangle.Top    += GetSystemMetric(METRIC_FRAME_SIZE);
            windowRectangle.Bottom += GetSystemMetric(METRIC_FRAME_SIZE);
            windowRectangle.Right  -= GetSystemMetric(METRIC_FRAME_SIZE);                
        }

        left = windowRectangle.Left;
        top = windowRectangle.Top;
        right = windowRectangle.Right;
        bottom = windowRectangle.Bottom;
            
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);

        windowRectangle.Top -= GetSystemMetric(METRIC_BORDER_SIZE);

        DrawText(hDeviceContext, GetWindowTextPointer(hWindow), &windowRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);

        windowRectangle.Left  += 1;
        windowRectangle.Right += 1;
        DrawText(hDeviceContext, GetWindowTextPointer(hWindow), &windowRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
    }

    DrawEnd(hDeviceContext);
    ReleaseDeviceContext(hDeviceContext);
}

VOID DrawWindow95StyleWindow(HWINDOW hWindow)
{
    WINDOW_FLAGS flags = GetFlags(hWindow);

    INT32 left;
    INT32 top;
    INT32 right;
    INT32 bottom;

    HDEVICE_CONTEXT hDeviceContext = GetDeviceContextNonClient(hWindow);
    DrawBegin(hDeviceContext);

    RECTANGLE windowRectangle;
    GetScreenRectangle(hWindow, &windowRectangle);

    windowRectangle.Right -= windowRectangle.Left;
    windowRectangle.Left -= windowRectangle.Left;
    windowRectangle.Bottom -= windowRectangle.Top;
    windowRectangle.Top -= windowRectangle.Top;

    left = windowRectangle.Left;
    top = windowRectangle.Top;
    right = windowRectangle.Right - windowRectangle.Left;
    bottom = windowRectangle.Bottom - windowRectangle.Top;;

    SetForegroundColor(hDeviceContext, ColorControlSurfaceNormal);
    DrawControlState(hDeviceContext, windowRectangle.Left, windowRectangle.Top, windowRectangle.Right, windowRectangle.Bottom, DRAW_CONTROL_STATE_EDGE_WINDOW);
    DrawRectangleA(hDeviceContext, left + 2, top + 2, right - 2, bottom - 2);

    if (GetFlags(hWindow) & WINDOW_FLAG_HAS_CAPTION)
    {
        windowRectangle.Left   += 3;
        windowRectangle.Top    += 3;
        windowRectangle.Bottom -= 3;
        windowRectangle.Right  -= 3;                

        windowRectangle.Bottom = windowRectangle.Top + GetSystemMetric(METRIC_WINDOW_TITLE_BAR_SIZE);

        left = windowRectangle.Left;
        top = windowRectangle.Top;
        right = windowRectangle.Right;
        bottom = windowRectangle.Bottom;

        if (flags & WINDOW_FLAG_IS_ACTIVE)
        {
            SetBackgroundColor(hDeviceContext, ColorWindowActiveBorder);
            SetTextColor(hDeviceContext, ColorWindowActiveText);

            //DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
            DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_ACTIVE_CAPTION_GRADIENT);

            windowRectangle.Left  += 2;
            windowRectangle.Right -= 2;

            DrawText(hDeviceContext, GetWindowTextPointer(hWindow), &windowRectangle, DRAW_TEXT_FLAG_ALIGN_LEFT | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER | DRAW_TEXT_FLAG_BOLD);
        }
        else
        {
            SetBackgroundColor(hDeviceContext, ColorWindowInactiveBorder);
            SetTextColor(hDeviceContext, ColorWindowInactiveText);

            //DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
            DrawControlState(hDeviceContext, left, top, right, bottom, DRAW_CONTROL_STATE_INACTIVE_CAPTION_GRADIENT);

            windowRectangle.Left  += 2;
            windowRectangle.Right -= 2;

            DrawText(hDeviceContext, GetWindowTextPointer(hWindow), &windowRectangle, DRAW_TEXT_FLAG_ALIGN_LEFT | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER | DRAW_TEXT_FLAG_BOLD);
        }
    }

    DrawEnd(hDeviceContext);
    ReleaseDeviceContext(hDeviceContext);
}