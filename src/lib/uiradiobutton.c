#include "kldr.h"

static CHAR UiRbWindowClass[]        = "RADIOBUTTON";

UINT32 RadiobuttonWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);

VOID RegisterRadiobutton()
{
    RegisterWindow((PCHAR) &UiRbWindowClass, 0, 0, 0, 0, 255, (PVOID) RadiobuttonWindowProcedure, 0);
}

HWINDOW CreateRadiobutton(PCHAR pName, INT32 x, INT32 y, INT32 width, INT32 height, HWINDOW hParent, WINDOW_FLAGS flags)
{
    flags |= WINDOW_FLAG_IS_CHILD;
    flags |= WINDOW_FLAG_IS_TAB_STOP;

    HWINDOW hWindow = CreateWindowByClass((PCHAR) &UiRbWindowClass, pName, x, y, width, height, hParent, 0, flags);

    // if (flags & CHECKBOX_FLAG_IS_DEFAULT)
    // {
    //     SetDefault(hWindow);
    // }

    // if (flags & CHECKBOX_FLAG_IS_FOCUSED)
    // {
    //     SetFocus(hWindow);
    // }

    return hWindow;
}

UINT32 RadiobuttonWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    WINDOW_FLAGS flags = GetFlags(hWindow);
    HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
    RECTANGLE clientRectangle;
    RECTANGLE checkboxRectangle;
    RECTANGLE textRectangle;
    UINT32 checkboxSize = GetSystemMetric(METRIC_CHECKBOX_SIZE);
    UINT32 textWidth = GetTextWidth(hDeviceContext, GetWindowTextPointer(hWindow), 0);
    UINT32 textHeight = GetTextHeight(hDeviceContext, GetWindowTextPointer(hWindow));
    
    GetClientRectangle(hWindow, &clientRectangle);
    SetRectangleB(&checkboxRectangle, 0, (clientRectangle.Bottom - checkboxSize) / 2, checkboxSize, checkboxSize);
    SetRectangleB(&textRectangle, checkboxRectangle.Right + 4, checkboxRectangle.Top - 1, textWidth + 2, textHeight + 3);

    if (message == WINDOW_MESSAGE_PAINT)
    {
        DrawBegin(hDeviceContext);

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        DrawFilledRectangleC(hDeviceContext, &clientRectangle);

        if (flags & WINDOW_FLAG_IS_DISABLED)
        {
            // DISABLED STATE

            DrawControlState(hDeviceContext, checkboxRectangle.Left, checkboxRectangle.Top, checkboxRectangle.Right, checkboxRectangle.Bottom, DRAW_CONTROL_STATE_RADIOBUTTON_DISABLED);

            if (flags & RADIOBUTTON_FLAG_IS_CHECKED)
            {
                DrawControlState(hDeviceContext, checkboxRectangle.Left, checkboxRectangle.Top, checkboxRectangle.Right, checkboxRectangle.Bottom, DRAW_CONTROL_STATE_RADIOBUTTON_SELECTION_DISABLED);
            }

            SetTextColor(hDeviceContext, ColorControlTextDisabled);

            textRectangle.Left += 1;
            DrawText(hDeviceContext, GetWindowTextPointer(hWindow), &textRectangle, DRAW_TEXT_FLAG_ALIGN_LEFT | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        }
        else
        {
            // ENABLED STATE
            if (flags & RADIOBUTTON_FLAG_IS_PRESSED)
            {
                DrawControlState(hDeviceContext, checkboxRectangle.Left, checkboxRectangle.Top, checkboxRectangle.Right, checkboxRectangle.Bottom, DRAW_CONTROL_STATE_RADIOBUTTON_DISABLED);
            }
            else
            {
                DrawControlState(hDeviceContext, checkboxRectangle.Left, checkboxRectangle.Top, checkboxRectangle.Right, checkboxRectangle.Bottom, DRAW_CONTROL_STATE_RADIOBUTTON);
            }

            if (flags & RADIOBUTTON_FLAG_IS_CHECKED)
            {
                DrawControlState(hDeviceContext, checkboxRectangle.Left, checkboxRectangle.Top, checkboxRectangle.Right, checkboxRectangle.Bottom, DRAW_CONTROL_STATE_RADIOBUTTON_SELECTION);
            }

            if (flags & RADIOBUTTON_FLAG_IS_FOCUSED)
            {
                SetForegroundColor(hDeviceContext, ColorControlSurfaceNormal);
             
                DrawControlState(hDeviceContext, textRectangle.Left, textRectangle.Top, textRectangle.Right, textRectangle.Bottom, DRAW_CONTROL_STATE_WINDOW_RESIZE_FRAME_THIN);
            }        

            if (flags & RADIOBUTTON_FLAG_IS_ON_HOVER)
            {
                SetTextColor(hDeviceContext, ColorControlTextOnHover);
            }
            else
            {
                SetTextColor(hDeviceContext, ColorControlTextNormal);
            }

            textRectangle.Left += 1;
            DrawText(hDeviceContext, GetWindowTextPointer(hWindow), &textRectangle, DRAW_TEXT_FLAG_ALIGN_LEFT | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        }

        DrawEnd(hDeviceContext);

        return;
    }
    
    if (message == WINDOW_MESSAGE_MOUSE_ENTER)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return;
        }

        SetFlag(hWindow, RADIOBUTTON_FLAG_IS_ON_HOVER);

        if (GetCapture() == hWindow)
        {
            SetFlag(hWindow, RADIOBUTTON_FLAG_IS_PRESSED);
        }

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEAVE)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return;
        }

        ClearFlag(hWindow, RADIOBUTTON_FLAG_IS_ON_HOVER);

        if (GetCapture() == hWindow)
        {
            ClearFlag(hWindow, RADIOBUTTON_FLAG_IS_PRESSED);
        }

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_DOWN)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return;
        }

        SetFlag(hWindow, RADIOBUTTON_FLAG_IS_PRESSED);

        SetFocus(hWindow);
        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        SetCapture(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_UP)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return;
        }

        ClearFlag(hWindow, RADIOBUTTON_FLAG_IS_PRESSED);

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        ReleaseCapture();

        POINT mousePosition = ((PPOINT) param1)[0];
        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);

        if (CheckIfPointInRectangle(&clientRectangle, &mousePosition))
        {
            SendMessage(hWindow, WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_CLICK, 0, 0);
        }

        return;
    }

    if (message ==  WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_CLICK)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return;
        }

        if (!HasFlag(hWindow, RADIOBUTTON_FLAG_IS_CHECKED))
        {
            SetFlag(hWindow, RADIOBUTTON_FLAG_IS_CHECKED);
        }

        ClearFlag(hWindow, RADIOBUTTON_FLAG_IS_PRESSED);

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        SendMessage(GetParent(hWindow), WINDOW_MESSAGE_COMMAND, COMMAND_RADIOBUTTON_CLICKED, hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_FOCUS_GAINED)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return;
        }

        SetFlag(hWindow, RADIOBUTTON_FLAG_IS_FOCUSED);

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return;
    }

    if (message == WINDOW_MESSAGE_FOCUS_LOST)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return;
        }

        ClearFlag(hWindow, RADIOBUTTON_FLAG_IS_FOCUSED);

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return;
    }

    DefaultWindowProcedure(hWindow, message, param1, param2);
}
