#include "kldr.h"

#define SCROLLBAR_WINDOW_EXTRA_COUNT                   2

#define SCROLLBAR_EXTRA_SMALL_CHANGE                   0
#define SCROLLBAR_EXTRA_LARGE_CHANGE                   1
#define SCROLLBAR_EXTRA_MINIMUM_POSITON                2
#define SCROLLBAR_EXTRA_MAXIMUM_POSITON                3
#define SCROLLBAR_EXTRA_CURRENT_POSITON                4
#define SCROLLBAR_EXTRA_TRACKING_POSITON               5

static CHAR UiScWindowClass[]        = "SCROLLBAR";

static INT32 Extra[16];

UINT32 ScrollbarWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);
VOID DrawScrollbarButton(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 right, INT32 bottom, UINT32 state);

VOID RedrawScrollArea(HWINDOW hWindow);

VOID RegisterScrollbar()
{
    for(UINT32 i = 0; i < 16; i++)
    {
        Extra[i] = 0;
    }

    Extra[SCROLLBAR_EXTRA_SMALL_CHANGE]     = 5;
    Extra[SCROLLBAR_EXTRA_LARGE_CHANGE]     = 50;
    Extra[SCROLLBAR_EXTRA_MINIMUM_POSITON]  = 0;
    Extra[SCROLLBAR_EXTRA_MAXIMUM_POSITON]  = 200;
    Extra[SCROLLBAR_EXTRA_CURRENT_POSITON]  = 0;
    Extra[SCROLLBAR_EXTRA_TRACKING_POSITON] = 0;    

    RegisterWindow((PCHAR) &UiScWindowClass, 0, SCROLLBAR_WINDOW_EXTRA_COUNT, 0, 0, 255, (PVOID) ScrollbarWindowProcedure, 0);
}

HWINDOW CreateScrollbar(PCHAR pName, INT32 x, INT32 y, INT32 width, INT32 height, HWINDOW hParent, WINDOW_FLAGS flags)
{
    flags |= WINDOW_FLAG_IS_CHILD;

    HWINDOW hWindow = CreateWindowByClass((PCHAR) &UiScWindowClass, pName, x, y, width, height, hParent, 0, flags);

    //SetScrollRange(hWindow, 50);
    //SetScrollPosition(hWindow, 49);

    if (flags & WINDOW_FLAG_IS_VISIBLE)
    {
        //SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);
    }
    
    // There are scrollbars which can be tabstops (not supported yet)
    // if (flags & BUTTON_FLAG_IS_DEFAULT)
    // {
    //     SetDefault(hButton);
    // }

    // if (flags & BUTTON_FLAG_IS_FOCUSED)
    // {
    //     SetFocus(hButton);
    // }

    return hWindow;
}

UINT32 ScrollbarWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
    RECTANGLE clientRectangle;
    GetClientRectangle(hWindow, &clientRectangle);
    WINDOW_FLAGS flags = GetFlags(hWindow);
    UINT32 scrollbarSize = GetSystemMetric(METRIC_SCROLLBAR_SIZE);

    if (message == WINDOW_MESSAGE_PAINT)
    {
        DrawBegin(hDeviceContext);

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        DrawFilledRectangleC(hDeviceContext, &clientRectangle);

        // Top Button
        DrawControlState(hDeviceContext, 0, 0, scrollbarSize, scrollbarSize, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_NORMAL);

        // Bottom Button
        DrawControlState(hDeviceContext, 0, clientRectangle.Bottom - scrollbarSize, scrollbarSize, clientRectangle.Bottom, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_NORMAL);
 
        DrawEnd(hDeviceContext);

        RedrawScrollArea(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_MOUSE_ENTER || message == WINDOW_MESSAGE_MOUSE_MOVE || message == WINDOW_MESSAGE_MOUSE_LEAVE)
    {
        POINT mousePosition = ((PPOINT) param1)[0];
        RECTANGLE buttonRectangle;

        BOOL isMousePressed;
        GetMouseLeftButtonState(&isMousePressed);

        if (isMousePressed)
        {
            return;
        }

        // UP BUTTON 
        buttonRectangle.Left = 0;
        buttonRectangle.Right = scrollbarSize;
        buttonRectangle.Top = 0;
        buttonRectangle.Bottom = scrollbarSize;

        if (CheckIfPointInRectangle(&buttonRectangle, &mousePosition))
        {
            if (!(flags & SCROLLBAR_FLAG_BUTTON1_IS_ON_HOVER))
            {
                flags |= SCROLLBAR_FLAG_BUTTON1_IS_ON_HOVER;
                DrawBegin(hDeviceContext);
                DrawControlState(hDeviceContext, buttonRectangle.Left, buttonRectangle.Top, buttonRectangle.Right, buttonRectangle.Bottom, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_ONHOVER);
                DrawEnd(hDeviceContext);
            }
        }
        else
        {
            if (flags & SCROLLBAR_FLAG_BUTTON1_IS_ON_HOVER)
            {
                flags &= ~SCROLLBAR_FLAG_BUTTON1_IS_ON_HOVER;
                //flags &= ~SCROLLBAR_FLAG_BUTTON1_IS_PRESSED;
                DrawBegin(hDeviceContext);
                DrawControlState(hDeviceContext, buttonRectangle.Left, buttonRectangle.Top, buttonRectangle.Right, buttonRectangle.Bottom, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_NORMAL);
                DrawEnd(hDeviceContext);
            }
        }

        // DOWN BUTTON 
        buttonRectangle.Left = 0;
        buttonRectangle.Right = scrollbarSize;
        buttonRectangle.Top = clientRectangle.Bottom - scrollbarSize;
        buttonRectangle.Bottom = clientRectangle.Bottom;

        if (CheckIfPointInRectangle(&buttonRectangle, &mousePosition))
        {
            if (!(flags & SCROLLBAR_FLAG_BUTTON2_IS_ON_HOVER))
            {
                flags |= SCROLLBAR_FLAG_BUTTON2_IS_ON_HOVER;
                DrawBegin(hDeviceContext);
                DrawControlState(hDeviceContext, buttonRectangle.Left, buttonRectangle.Top, buttonRectangle.Right, buttonRectangle.Bottom, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_ONHOVER);
                DrawEnd(hDeviceContext);
            }
        }
        else
        {
            if (flags & SCROLLBAR_FLAG_BUTTON2_IS_ON_HOVER)
            {
                flags &= ~SCROLLBAR_FLAG_BUTTON2_IS_ON_HOVER;
                //flags &= ~SCROLLBAR_FLAG_BUTTON2_IS_PRESSED;
                DrawBegin(hDeviceContext);
                DrawControlState(hDeviceContext, buttonRectangle.Left, buttonRectangle.Top, buttonRectangle.Right, buttonRectangle.Bottom, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_NORMAL);
                DrawEnd(hDeviceContext);
            }
        }

        SetFlags(hWindow, flags);

        return;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_DOWN)
    {
        RECTANGLE buttonRectangle;
        
        // UP BUTTON
        if (flags & SCROLLBAR_FLAG_BUTTON1_IS_ON_HOVER)
        {
            buttonRectangle.Left = 0;
            buttonRectangle.Top = 0;
            buttonRectangle.Right = scrollbarSize;
            buttonRectangle.Bottom = scrollbarSize;            

            flags |= SCROLLBAR_FLAG_BUTTON1_IS_PRESSED;
            DrawBegin(hDeviceContext);
            DrawControlState(hDeviceContext, buttonRectangle.Left, buttonRectangle.Top, buttonRectangle.Right, buttonRectangle.Bottom, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_PRESSED);
            DrawEnd(hDeviceContext);

            INT32 position = GetScrollCurrentPosition(hWindow);
            INT32 change = GetScrollSmallChange(hWindow);

            position -= change;
            
            SetScrollCurrentPosition(hWindow, position);

            if (position == GetScrollCurrentPosition(hWindow))
            {
                // The new position was valid and has been taken by the scrollbar
                SendMessage(GetParent(hWindow), WINDOW_MESSAGE_COMMAND, COMMAND_SCROLL_DOWN, 0);
            }
            
        }

        // DOWN BUTTON
        if (flags & SCROLLBAR_FLAG_BUTTON2_IS_ON_HOVER)
        {
            buttonRectangle.Left = 0;
            buttonRectangle.Top = clientRectangle.Bottom - scrollbarSize;
            buttonRectangle.Right = scrollbarSize;
            buttonRectangle.Bottom = clientRectangle.Bottom;          

            flags |= SCROLLBAR_FLAG_BUTTON2_IS_PRESSED;
            DrawBegin(hDeviceContext);
            DrawControlState(hDeviceContext, buttonRectangle.Left, buttonRectangle.Top, buttonRectangle.Right, buttonRectangle.Bottom, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_PRESSED);
            DrawEnd(hDeviceContext);

            INT32 position = GetScrollCurrentPosition(hWindow);
            INT32 change = GetScrollSmallChange(hWindow);

            position += change;

            SetScrollCurrentPosition(hWindow, position);         

            if (position == GetScrollCurrentPosition(hWindow))
            {
                // The new position was valid and has been taken by the scrollbar
                SendMessage(GetParent(hWindow), WINDOW_MESSAGE_COMMAND, COMMAND_SCROLL_UP, 0);
            }
        }

        SetFlags(hWindow, flags);       

        //SetCapture(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_ON_SYSTEM_TICK)
    {
        if (flags & SCROLLBAR_FLAG_BUTTON1_IS_PRESSED)
        {
            INT32 position = GetScrollCurrentPosition(hWindow);
            INT32 change = GetScrollSmallChange(hWindow);

            position -= change;

            SetScrollCurrentPosition(hWindow, position); 

            if (position == GetScrollCurrentPosition(hWindow))
            {
                // The new position was valid and has been taken by the scrollbar
                SendMessage(GetParent(hWindow), WINDOW_MESSAGE_COMMAND, COMMAND_SCROLL_DOWN, 0);
            }
        }

        if (flags & SCROLLBAR_FLAG_BUTTON2_IS_PRESSED)
        {
            INT32 position = GetScrollCurrentPosition(hWindow);
            INT32 change = GetScrollSmallChange(hWindow);

            position += change;
            
            SetScrollCurrentPosition(hWindow, position);   

            if (position == GetScrollCurrentPosition(hWindow))
            {
                // The new position was valid and has been taken by the scrollbar
                SendMessage(GetParent(hWindow), WINDOW_MESSAGE_COMMAND, COMMAND_SCROLL_UP, 0);
            }
        }

        return;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_UP)
    {
        RECTANGLE buttonRectangle;
        
        // UP BUTTON
        if (flags & SCROLLBAR_FLAG_BUTTON1_IS_PRESSED)
        {
            buttonRectangle.Left = 0;
            buttonRectangle.Top = 0;
            buttonRectangle.Right = scrollbarSize;
            buttonRectangle.Bottom = scrollbarSize;            

            flags &= ~SCROLLBAR_FLAG_BUTTON1_IS_PRESSED;
            DrawBegin(hDeviceContext);
            DrawControlState(hDeviceContext, buttonRectangle.Left, buttonRectangle.Top, buttonRectangle.Right, buttonRectangle.Bottom, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_UP_ONHOVER);
            DrawEnd(hDeviceContext);
        }

        // DOWN BUTTON
        if (flags & SCROLLBAR_FLAG_BUTTON2_IS_PRESSED)
        {
            buttonRectangle.Left = 0;
            buttonRectangle.Top = clientRectangle.Bottom - scrollbarSize;
            buttonRectangle.Right = scrollbarSize;
            buttonRectangle.Bottom = clientRectangle.Bottom;          

            flags &= ~SCROLLBAR_FLAG_BUTTON2_IS_PRESSED;
            DrawBegin(hDeviceContext);
            DrawControlState(hDeviceContext, buttonRectangle.Left, buttonRectangle.Top, buttonRectangle.Right, buttonRectangle.Bottom, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_DOWN_ONHOVER);
            DrawEnd(hDeviceContext);
        }

        SetFlags(hWindow, flags);    

        return;
    }
}

INT32 GetScrollSmallChange(HWINDOW hWindow)
{
    //return GetWindowExtra(hWindow, SCROLLBAR_EXTRA_SMALL_CHANGE);
    return Extra[SCROLLBAR_EXTRA_SMALL_CHANGE];
}

INT32 GetScrollLargeChange(HWINDOW hWindow)
{
    //return GetWindowExtra(hWindow, SCROLLBAR_EXTRA_LARGE_CHANGE);
    return Extra[SCROLLBAR_EXTRA_LARGE_CHANGE];
}

INT32 GetScrollMinimumPosition(HWINDOW hWindow)
{
    //return GetWindowExtra(hWindow, SCROLLBAR_EXTRA_MINIMUM_POSITON);
    return Extra[SCROLLBAR_EXTRA_MINIMUM_POSITON];
}

INT32 GetScrollMaximumPosition(HWINDOW hWindow)
{
    //return GetWindowExtra(hWindow, SCROLLBAR_EXTRA_MAXIMUM_POSITON);
    return Extra[SCROLLBAR_EXTRA_MAXIMUM_POSITON];
}

INT32 GetScrollCurrentPosition(HWINDOW hWindow)
{
    //return GetWindowExtra(hWindow, SCROLLBAR_EXTRA_CURRENT_POSITON);
    return Extra[SCROLLBAR_EXTRA_CURRENT_POSITON];
}

VOID SetScrollSmallChange(HWINDOW hWindow, INT32 value)
{
    //SetWindowExtra(hWindow, SCROLLBAR_EXTRA_SMALL_CHANGE, value);
    Extra[SCROLLBAR_EXTRA_SMALL_CHANGE] = value;
}

VOID SetScrollLargeChange(HWINDOW hWindow, INT32 value)
{
    //SetWindowExtra(hWindow, SCROLLBAR_EXTRA_LARGE_CHANGE, value);
    Extra[SCROLLBAR_EXTRA_LARGE_CHANGE] = value;
}

VOID SetScrollMinimumPosition(HWINDOW hWindow, INT32 position)
{
        //SetWindowExtra(hWindow, SCROLLBAR_EXTRA_MINIMUM_POSITON, position);
        Extra[SCROLLBAR_EXTRA_MINIMUM_POSITON] = position;
}

VOID SetScrollMaximumPosition(HWINDOW hWindow, INT32 position)
{
        //SetWindowExtra(hWindow, SCROLLBAR_EXTRA_MAXIMUM_POSITON, position);
        Extra[SCROLLBAR_EXTRA_MAXIMUM_POSITON] = position;
}

VOID SetScrollCurrentPosition(HWINDOW hWindow, INT32 position)
{
    INT32 min = GetScrollMinimumPosition(hWindow);
    INT32 max = GetScrollMaximumPosition(hWindow);
    INT32 curPos = GetScrollCurrentPosition(hWindow);

    if (position < min) position = min;
    if (position > max) position = max;

    if (curPos != position)
    {
        //SetWindowExtra(hWindow, SCROLLBAR_EXTRA_CURRENT_POSITON, position);
        Extra[SCROLLBAR_EXTRA_CURRENT_POSITON] = position;
        RedrawScrollArea(hWindow);
    }    
}

VOID RedrawScrollArea(HWINDOW hWindow)
{
    HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
    RECTANGLE clientRectangle;
    GetClientRectangle(hWindow, &clientRectangle);
    INT32 scrollbarSize = GetSystemMetric(METRIC_SCROLLBAR_SIZE);
    INT32 pos = GetScrollCurrentPosition(hWindow);
    INT32 minPos = GetScrollMinimumPosition(hWindow);
    INT32 maxPos = GetScrollMaximumPosition(hWindow);
    INT32 largeChange = GetScrollLargeChange(hWindow);

    // Validation
    if (maxPos == minPos)
    {
        BugCheck(NULL, "Min and Max position for the scrollbar was the same", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL ,0);

        return;
    }

    if (pos < minPos)
    {
        pos = minPos;
    }

    if (pos > maxPos)
    {
        pos = maxPos;
    }

    UINT32 thumbSize = (clientRectangle.Bottom - clientRectangle.Top - 2 * scrollbarSize) * largeChange / (maxPos - minPos);
    UINT32 thumbPosition = (clientRectangle.Bottom - scrollbarSize - thumbSize) * pos / (maxPos - minPos);
    if (pos == maxPos) thumbPosition = clientRectangle.Bottom - scrollbarSize - thumbSize;
    UINT32 thumbTop = thumbPosition + scrollbarSize;
    UINT32 thumbBottom = thumbPosition + thumbSize;

    DrawBegin(hDeviceContext);

    SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
    SetForegroundColor(hDeviceContext, 0xFFFFFFFF);

    // Scroll Area To Thumb Button
    
    if (thumbTop > scrollbarSize)
    {
        DrawFilledRectangleA(hDeviceContext, 0, scrollbarSize, scrollbarSize, thumbTop);
        // SetDrawMode(hDeviceContext, DRAW_MODE_PEN_DOT);
        // for(UINT32 y = scrollbarSize; y < thumbTop; y++)
        // {
        //     DrawLineA(hDeviceContext, 0, y, scrollbarSize, y);
        // }
        // SetDrawMode(hDeviceContext, DRAW_MODE_NORMAL);
    }

    // Thumb Button
    DrawControlState(hDeviceContext, 0, thumbTop, scrollbarSize, thumbBottom, DRAW_CONTROL_STATE_SCROLLBAR_BUTTON_THUMB_NORMAL);

    // Scroll Area From Thumb Button
    if (thumbBottom < clientRectangle.Bottom - scrollbarSize)
    {
        DrawFilledRectangleA(hDeviceContext, 0, thumbBottom, scrollbarSize, clientRectangle.Bottom - scrollbarSize);
        // SetDrawMode(hDeviceContext, DRAW_MODE_PEN_DOT);
        // for(UINT32 y = thumbBottom; y < clientRectangle.Bottom - scrollbarSize; y++)
        // {
        //     DrawLineA(hDeviceContext, 0, y, scrollbarSize, y);
        // }
        // SetDrawMode(hDeviceContext, DRAW_MODE_NORMAL);
    }

    DrawEnd(hDeviceContext);
}

// STUFF
//
// Needed:
//   - Minimum Position (0)
//   - Maximum Position (100)
//   - Current Position (0-100)
//   - Tracking Position (where the user currently dragging the scrollbox)
//   - Small Change  (1)
//   - Large Change  (10)
//   - Height
//
// ThumbPosition = (MaxPos - MinPos) / Pos + TopmostThumbPosition
// ThumbSize = (MaxPos - MinPos) / LargeChange


// Windows Info
// WM_HSCROLL
//   SB_ENDSCROLL
//   SB_LEFT
//   SB_RIGHT
//   SB_LINELEFT
//   SB_LINERIGHT
//   SB_PAGELEFT
//   SB_PAGERIGHT
//   SB_THUMBPOSITION
//   SB_THUMBTRACK

// WM_VSCROLL
//   SB_BOTTOM
//   SB_ENDSCROLL
//   SB_LINEDOWN
//   SB_LINEUP
//   SB_PAGEDOWN
//   SB_PAGEUP
//   SB_THUMBPOSITON
//   SB_THUMBTRACT
//   SB_TOP

// int GetScrollPos(
//   [in] HWND hWnd,
//   [in] int  nBar
// );

// hWnd
// Handle to a scroll bar control or a window with a standard scroll bar, depending on the value of the nBar parameter.

// nBar
// Specifies the scroll bar to be examined. This parameter can be one of the following values.

// SB_CTL
// SB_HORZ
// SB_VERT


