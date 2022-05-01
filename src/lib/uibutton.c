#include "kldr.h"

static CHAR UiBuWindowClass[]        = "BUTTON";

UINT32 ButtonWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);

VOID RegisterButton()
{
    RegisterWindow((PCHAR) &UiBuWindowClass, 0, 0, 0, 0, 255, (PVOID) ButtonWindowProcedure, 0);
}

HWINDOW CreateButton(PCHAR pName, INT32 x, INT32 y, INT32 width, INT32 height, HWINDOW hParent, WINDOW_FLAGS flags)
{
    flags |= WINDOW_FLAG_IS_CHILD;
    flags |= WINDOW_FLAG_IS_TAB_STOP;

    HWINDOW hWindow = CreateWindowByClass((PCHAR) &UiBuWindowClass, pName, x, y, width, height, hParent, 0, flags);
    
    return hWindow;
}

UINT32 ButtonWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    if (message == WINDOW_MESSAGE_PAINT)
    {
        WINDOW_FLAGS flags = GetFlags(hWindow);

        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
        DrawBegin(hDeviceContext);

        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        DrawFilledRectangleC(hDeviceContext, &clientRectangle);

        if (flags & BUTTON_FLAG_IS_DEFAULT)
        {
            SetForegroundColor(hDeviceContext, ColorControlFrameFocused);
            DrawRectangleC(hDeviceContext, &clientRectangle);

            if (flags & BUTTON_FLAG_IS_PRESSED)
            {
                DrawControlState(hDeviceContext, clientRectangle.Left + 1, clientRectangle.Top + 1, clientRectangle.Right - 1, clientRectangle.Bottom - 1, DRAW_CONTROL_STATE_EDGE_SUNKEN);
            }
            else
            {
                DrawControlState(hDeviceContext, clientRectangle.Left + 1, clientRectangle.Top + 1, clientRectangle.Right - 1, clientRectangle.Bottom - 1, DRAW_CONTROL_STATE_EDGE_RAISED);
            }
            
        }
        else
        {
            if (flags & BUTTON_FLAG_IS_PRESSED)
            {
                DrawControlState(hDeviceContext, clientRectangle.Left, clientRectangle.Top, clientRectangle.Right, clientRectangle.Bottom, DRAW_CONTROL_STATE_EDGE_SUNKEN);
            }
            else
            {
                DrawControlState(hDeviceContext, clientRectangle.Left, clientRectangle.Top, clientRectangle.Right, clientRectangle.Bottom, DRAW_CONTROL_STATE_EDGE_RAISED);
            }
        }        

        if (flags & BUTTON_FLAG_IS_ON_HOVER)
        {
            SetTextColor(hDeviceContext, ColorControlTextOnHover);
        }
        else
        {
            SetTextColor(hDeviceContext, ColorControlTextNormal);
        }
        
        if (flags & WINDOW_FLAG_IS_DISABLED)
        {
            SetTextColor(hDeviceContext, ColorControlTextDisabled);
        }

        if (flags & BUTTON_FLAG_IS_PRESSED)
        {
            clientRectangle.Left   += 1;
            clientRectangle.Top    += 1;
            clientRectangle.Right  += 1;
            clientRectangle.Bottom += 1;

            DrawText(hDeviceContext, GetWindowTextPointer(hWindow), &clientRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);

            clientRectangle.Left   -= 1;
            clientRectangle.Top    -= 1;
            clientRectangle.Right  -= 1;
            clientRectangle.Bottom -= 1;
        }
        else
        {
            DrawText(hDeviceContext, GetWindowTextPointer(hWindow), &clientRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        }

        if (flags & BUTTON_FLAG_IS_FOCUSED)
        {
            SetForegroundColor(hDeviceContext, ColorControlSurfaceNormal);
            DrawControlState(hDeviceContext, clientRectangle.Left + 4, clientRectangle.Top + 4, clientRectangle.Right - 4, clientRectangle.Bottom - 4, DRAW_CONTROL_STATE_WINDOW_RESIZE_FRAME_THIN);
        }        

        DrawEnd(hDeviceContext);

        return 0;
    }
    
    if (message == WINDOW_MESSAGE_MOUSE_ENTER)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return 0;
        }

        SetFlag(hWindow, BUTTON_FLAG_IS_ON_HOVER);

        if (GetCapture() == hWindow)
        {
            SetFlag(hWindow, BUTTON_FLAG_IS_PRESSED);
        }

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return 0;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEAVE)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return 0;
        }

        ClearFlag(hWindow, BUTTON_FLAG_IS_ON_HOVER);

        if (GetCapture() == hWindow)
        {
            ClearFlag(hWindow, BUTTON_FLAG_IS_PRESSED);
        }

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return 0;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_DOWN)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return 0;
        }

        SetFlag(hWindow, BUTTON_FLAG_IS_PRESSED);

        SetDefault(hWindow);
        SetFocus(hWindow);
        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        SetCapture(hWindow);

        return 0;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_UP)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return 0;
        }

        ClearFlag(hWindow, BUTTON_FLAG_IS_PRESSED);

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        ReleaseCapture();

        POINT mousePosition = ((PPOINT) param1)[0];
        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);

        if (CheckIfPointInRectangle(&clientRectangle, &mousePosition))
        {
            SendMessage(hWindow, WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_CLICK, 0, 0);
        }

        return 0;
    }

    if (message ==  WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_CLICK)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return 0;
        }

        SendMessage(GetParent(hWindow), WINDOW_MESSAGE_COMMAND, COMMAND_BUTTON_CLICKED, hWindow);

        return 0;
    }

    if (message == WINDOW_MESSAGE_FOCUS_GAINED)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return 0;
        }

        SetFlag(hWindow, BUTTON_FLAG_IS_FOCUSED);

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return 0;
    }

    if (message == WINDOW_MESSAGE_FOCUS_LOST)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return 0;
        }

        ClearFlag(hWindow, BUTTON_FLAG_IS_FOCUSED);        
        
        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return 0;
    }

    if (message == WINDOW_MESSAGE_DEFAULT_GAINED)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return 0;
        }

        SetFlag(hWindow, BUTTON_FLAG_IS_DEFAULT);

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return 0;
    }

    if (message == WINDOW_MESSAGE_DEFAULT_LOST)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return 0;
        }

        ClearFlag(hWindow, BUTTON_FLAG_IS_DEFAULT);

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return 0;
    }

    // if (message == WINDOW_MESSAGE_GET_ENABLED)
    // {
    //     if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
    //     {
    //         return FALSE;
    //     }

    //     return TRUE;
    // }

    DefaultWindowProcedure(hWindow, message, param1, param2);
}

BOOL ButtonGetEnabled(HWINDOW hButton)
{
    return SendMessage(hButton, WINDOW_MESSAGE_GET_ENABLED, 0, 0);
}

BOOL ButtonGetChecked(HWINDOW hButton)
{
    return SendMessage(hButton, WINDOW_MESSAGE_GET_CHECKED, 0, 0);
}

UINT32 ButtonGetState(HWINDOW hButton)
{
    return SendMessage(hButton, WINDOW_MESSAGE_GET_STATE, 0, 0);
}

BOOL ButtonGetFocus(HWINDOW hButton)
{
    return SendMessage(hButton, WINDOW_MESSAGE_GET_FOCUS, 0, 0);
}

BOOL ButtonGetDefault(HWINDOW hButton)
{
    return SendMessage(hButton, WINDOW_MESSAGE_GET_DEFAULT, 0, 0);
}

VOID ButtonGetText(HWINDOW hButton, PCHAR pTextBuffer, UINT32 bufferSize)
{
    return GetWindowText(hButton, pTextBuffer, bufferSize);
}

VOID ButtonSetEnabled(HWINDOW hButton, BOOL isEnabled)
{
    SendMessage(hButton, WINDOW_MESSAGE_SET_ENABLED, (UINT32) isEnabled, 0);
}

VOID ButtonSetChecked(HWINDOW hButton, BOOL isChecked)
{
    SendMessage(hButton, WINDOW_MESSAGE_SET_CHECKED, (UINT32) isChecked, 0);
}

VOID ButtonSetState(HWINDOW hButton, UINT32 stateFlags)
{
    SendMessage(hButton, WINDOW_MESSAGE_SET_STATE, stateFlags, 0);
}

VOID ButtonSetFocus(HWINDOW hButton)
{
    SendMessage(hButton, WINDOW_MESSAGE_SET_FOCUS, 0, 0);
}

VOID ButtonSetDefault(HWINDOW hButton)
{
    SendMessage(hButton, WINDOW_MESSAGE_SET_DEFAULT, 0, 0);
}

VOID ButtonSetText(HWINDOW hButton, PCHAR pText)
{
    SetWindowText(hButton, pText);
}