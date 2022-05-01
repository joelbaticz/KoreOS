#include "kldr.h"

#define TEXTBOX_EXTRA_COUNT             3
#define TEXTBOX_EXTRA_SELECTION_START   0
#define TEXTBOX_EXTRA_SELECTION_END     1
#define TEXTBOX_EXTRA_CARET_INDEX       2

static CHAR UiTbWindowClass[]           = "TEXTBOX";
static CHAR UiTbTextBuffer[256];

UINT32 TextBoxWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);
VOID DrawTextBoxText(HDEVICE_CONTEXT hDeviceContext, PCHAR pText, PRECTANGLE pRectangle, INT32 selectionStart, INT32 selectionEnd, INT32 cursorIndex);
VOID HandleDecrementCaretIndex(HWINDOW hWindow);
VOID HandleIncrementCaretIndex(HWINDOW hWindow);
VOID HandleRegularKeyPress(HWINDOW hWindow, UINT8 virtualKey);
VOID HandleDeleteFromCaret(HWINDOW hWindow);
VOID HandleDeleteBackFromCaret(HWINDOW hWindow);
VOID HandleHome(HWINDOW hWindow);
VOID HandleEnd(HWINDOW hWindow);

VOID RegisterTextBox()
{
    RegisterWindow((PCHAR) &UiTbWindowClass, 0, TEXTBOX_EXTRA_COUNT, 0, 0, 255, (PVOID) TextBoxWindowProcedure, 0);
}

HWINDOW CreateTextBox(PCHAR pText, INT32 x, INT32 y, INT32 width, INT32 height, HWINDOW hParent, WINDOW_FLAGS flags)
{
    flags |= WINDOW_FLAG_IS_CHILD;
    flags |= WINDOW_FLAG_IS_TAB_STOP;

    HWINDOW hWindow = CreateWindowByClass((PCHAR) &UiTbWindowClass, pText, x, y, width, height, hParent, 0, flags);

    SetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_START, -1);
    SetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_END, -1);
    SetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX, strlen(GetWindowTextPointer(hWindow)));

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

UINT32 TextBoxWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);

    if (message == WINDOW_MESSAGE_PAINT)
    {
        WINDOW_FLAGS flags = GetFlags(hWindow);

        DrawBegin(hDeviceContext);

        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);

        RECTANGLE textRectangle;
        SetRectangleA(&textRectangle, clientRectangle.Left + 3, clientRectangle.Top + 3, clientRectangle.Right - 3, clientRectangle.Bottom - 5);

        //SetBackgroundColor(hDeviceContext, 0xFF00FF00);
        //DrawFilledRectangleC(hDeviceContext, &clientRectangle);

        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            DrawControlState(hDeviceContext, clientRectangle.Left, clientRectangle.Top, clientRectangle.Right, clientRectangle.Bottom, DRAW_CONTROL_STATE_FIELD_DISABLED);    

            SetTextColor(hDeviceContext, ColorControlTextDisabled);
            DrawText(hDeviceContext, GetWindowTextPointer(hWindow), &textRectangle, DRAW_TEXT_FLAG_ALIGN_LEFT);
        }
        else
        {
            DrawControlState(hDeviceContext, clientRectangle.Left, clientRectangle.Top, clientRectangle.Right, clientRectangle.Bottom, DRAW_CONTROL_STATE_FIELD);

            SetTextColor(hDeviceContext, ColorControlTextNormal);

            if (!HasFlag(hWindow, TEXTBOX_FLAG_IS_FOCUSED))
            {
                DrawText(hDeviceContext, GetWindowTextPointer(hWindow), &textRectangle, DRAW_TEXT_FLAG_ALIGN_LEFT);
            }
            else
            {
                INT32 selectionStart = GetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_START);
                INT32 selectionEnd = GetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_END);
                INT32 caretIndex = GetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX);

                DrawTextBoxText(hDeviceContext, GetWindowTextPointer(hWindow), &textRectangle, selectionStart, selectionEnd, caretIndex);
            }
        }   

        if (HasFlag(hWindow, TEXTBOX_FLAG_IS_FOCUSED))
        {
            ShowCaret(hWindow);
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

        // SetFlag(hWindow, BUTTON_FLAG_IS_ON_HOVER);

        // if (GetCapture() == hWindow)
        // {
        //     SetFlag(hWindow, BUTTON_FLAG_IS_PRESSED);
        // }

        // SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEAVE)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return;
        }

        // ClearFlag(hWindow, BUTTON_FLAG_IS_ON_HOVER);

        // if (GetCapture() == hWindow)
        // {
        //     ClearFlag(hWindow, BUTTON_FLAG_IS_PRESSED);
        // }

        // SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return;
    }

    if (message == WINDOW_MESSAGE_MOUSE_LEFT_BUTTON_DOWN)
    {
        if (HasFlag(hWindow, WINDOW_FLAG_IS_DISABLED))
        {
            return;
        }

        // Determine caret position based on where we clicked with the mouse
        POINT mousePosition = ((PPOINT) param1)[0];
        PCHAR pWindowText = GetWindowTextPointer(hWindow);
        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
        UINT32 caretIndex = 0;

        for(UINT32 i = 0; i < strlen(pWindowText); i++)
        {
            strncpy(&UiTbTextBuffer[0], pWindowText, i);
            UINT32 textWidth1 = GetTextWidth(hDeviceContext, &UiTbTextBuffer[0], 0);
            strncpy(&UiTbTextBuffer[0], pWindowText, i + 1);
            UINT32 textWidth2 = GetTextWidth(hDeviceContext, &UiTbTextBuffer[0], 0);
            UINT32 charWidth = textWidth2 - textWidth1;

            if (3 +  textWidth1 - charWidth / 2 < mousePosition.X)
            {
                caretIndex = i;
            }
        }

        strcpy(&UiTbTextBuffer[0], pWindowText);
        if (3 + GetTextWidth(hDeviceContext, &UiTbTextBuffer[0], 0) < mousePosition.X)
        {
            caretIndex = strlen(pWindowText);
        }

        CreateCaret(hWindow, 1, 13);
        SetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX, caretIndex);        


        // SetFlag(hWindow, BUTTON_FLAG_IS_PRESSED);

        SetFocus(hWindow);
        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        // SetCapture(hWindow);

        return;
    }  

    if (message == WINDOW_MESSAGE_FOCUS_GAINED)
    {
        SetFlag(hWindow, TEXTBOX_FLAG_IS_FOCUSED);

        CreateCaret(hWindow, 1, 13);
        SetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX, 0);    
        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        return;
    }

    if (message == WINDOW_MESSAGE_FOCUS_LOST)
    {
        ClearFlag(hWindow, TEXTBOX_FLAG_IS_FOCUSED);

        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

        //DestroyCaret(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_KEYDOWN)
    {
        UINT8 virtualKey = (UINT8) param1;

        // HandleKeyPress(hWindow, virtualKey);

        if (virtualKey == VKEY_NUMPAD4 || virtualKey == VKEY_NUMPAD8)
        //if (virtualKey == VKEY_LEFT || virtualKey == VKEY_UP)
        {
            HandleDecrementCaretIndex(hWindow);

            return;
        }

        if (virtualKey == VKEY_NUMPAD6 || virtualKey == VKEY_NUMPAD2)
        //if (virtualKey == VKEY_RIGHT || virtualKey == VKEY_DOWN)
        {
            HandleIncrementCaretIndex(hWindow);

            return;
        }

        // if (virtualKey == VKEY_TAB)
        // {
        //     HandleTabPressed(hWindow); // Add 4 spaces

        //     return;
        // }

        if (virtualKey == VKEY_NUMPADDELETE)
        {
            HandleDeleteFromCaret(hWindow);
        }

        if (virtualKey == VKEY_BACKSPACE)
        {
            HandleDeleteBackFromCaret(hWindow);
        }

        if (virtualKey == VKEY_NUMPAD7)
        {
            HandleHome(hWindow);
        }

        if (virtualKey == VKEY_NUMPAD1)
        {
            HandleEnd(hWindow);
        }

        HandleRegularKeyPress(hWindow, virtualKey);

        DefaultWindowProcedure(hWindow, WINDOW_MESSAGE_KEYDOWN, param1, param2);
    }
}

VOID DrawTextBoxText(HDEVICE_CONTEXT hDeviceContext, PCHAR pText, PRECTANGLE pRectangle, INT32 selectionStart, INT32 selectionEnd, INT32 caretIndex)
{
    RECTANGLE textRectangle;
    UINT32 textLength;
    UINT32 textWidth;

    if (selectionStart < 0 || selectionEnd < 0)
    {
        // No selection
        SetRectangleB(&textRectangle, pRectangle->Left, pRectangle->Top, pRectangle->Right - pRectangle->Left, pRectangle->Bottom - pRectangle->Top);

        textWidth = GetTextWidth(hDeviceContext, &pText[0], 0);

        SetBackgroundColor(hDeviceContext, ColorControlContentArea);
        SetTextColor(hDeviceContext, ColorControlTextNormal);

        SetRectangleB(&textRectangle, textRectangle.Left, textRectangle.Top, textWidth, textRectangle.Bottom - textRectangle.Top);
        DrawFilledRectangleC(hDeviceContext, &textRectangle);

        DrawText(hDeviceContext, &pText[0], &textRectangle, 0);        
    }
    else
    {
        // Part before selection
        SetRectangleB(&textRectangle, pRectangle->Left, pRectangle->Top, pRectangle->Right - pRectangle->Left, pRectangle->Bottom - pRectangle->Top);

        strncpy(&UiTbTextBuffer[0], &pText[0], selectionStart);
        textLength = strlen(&UiTbTextBuffer[0]);
        textWidth = GetTextWidth(hDeviceContext, &UiTbTextBuffer[0], 0);

        SetBackgroundColor(hDeviceContext, ColorControlContentArea);
        SetTextColor(hDeviceContext, ColorControlTextNormal);

        SetRectangleB(&textRectangle, textRectangle.Left, textRectangle.Top, textWidth, textRectangle.Bottom - textRectangle.Top);
        DrawFilledRectangleC(hDeviceContext, &textRectangle);

        DrawText(hDeviceContext, &UiTbTextBuffer[0], &textRectangle, 0);

        // Selected part
        textRectangle.Left += textWidth;

        strncpy(&UiTbTextBuffer[0], &pText[selectionStart], selectionEnd - selectionStart + 1);
        textLength = strlen(&UiTbTextBuffer[0]);
        textWidth = GetTextWidth(hDeviceContext, &UiTbTextBuffer[0], 0);

        SetBackgroundColor(hDeviceContext, ColorControlContentAreaSelected);
        SetTextColor(hDeviceContext, ColorControlContentAreaTextSelected);

        SetRectangleB(&textRectangle, textRectangle.Left, textRectangle.Top, textWidth, textRectangle.Bottom - textRectangle.Top);
        DrawFilledRectangleC(hDeviceContext, &textRectangle);

        DrawText(hDeviceContext, &UiTbTextBuffer[0], &textRectangle, 0);

        // Part after selection
        textRectangle.Left += textWidth;

        strncpy(&UiTbTextBuffer[0], &pText[selectionEnd + 1], strlen(pText) - selectionEnd - 1);
        textLength = strlen(&UiTbTextBuffer[0]);
        textWidth = GetTextWidth(hDeviceContext, &UiTbTextBuffer[0], 0);

        SetBackgroundColor(hDeviceContext, ColorControlContentArea);
        SetTextColor(hDeviceContext, ColorControlTextNormal);

        SetRectangleB(&textRectangle, textRectangle.Left, textRectangle.Top, textWidth, textRectangle.Bottom - textRectangle.Top);
        DrawFilledRectangleC(hDeviceContext, &textRectangle);

        DrawText(hDeviceContext, &UiTbTextBuffer[0], &textRectangle, 0);
    }

    if (caretIndex >= 0)
    {
        // Set caret position
        strncpy(&UiTbTextBuffer[0], &pText[0], caretIndex);
        textWidth = GetTextWidth(hDeviceContext, &UiTbTextBuffer[0], 0);

        SetCaretPosition(pRectangle->Left + textWidth, pRectangle->Top);    
    }
}

VOID HandleDecrementCaretIndex(HWINDOW hWindow)
{
    INT32 selectionStart = GetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_START);
    INT32 selectionEnd = GetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_END);
    UINT32 caretIndex = GetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX);
    UINT32 prevCaretIndex = caretIndex;

    selectionStart = -1;
    selectionEnd = -1;

    if (caretIndex > 0)
    {
        caretIndex--;
    }

    SetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_START, selectionStart);
    SetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_END, selectionEnd);
    SetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX, caretIndex);

    if (prevCaretIndex != caretIndex)
    {
        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);
    }    
}

VOID HandleIncrementCaretIndex(HWINDOW hWindow)
{
    INT32 selectionStart = GetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_START);
    INT32 selectionEnd = GetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_END);
    INT32 caretIndex = GetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX);
    INT32 prevCaretIndex = caretIndex;
    UINT32 textLength = strlen(GetWindowTextPointer(hWindow));

    selectionStart = -1;
    selectionEnd = -1;

    if (caretIndex < textLength)
    {
        caretIndex++;
    }

    SetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_START, selectionStart);
    SetWindowExtra(hWindow, TEXTBOX_EXTRA_SELECTION_END, selectionEnd);
    SetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX, caretIndex);

    if (prevCaretIndex != caretIndex)
    {
        SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);
    }    
}

VOID HandleRegularKeyPress(HWINDOW hWindow, UINT8 virtualKey)
{
    UINT8 asciiCode = VirtualKeyToAscii(virtualKey);       

    if (asciiCode == 0)
    {
        return;
    }

    PCHAR pWindowText = GetWindowTextPointer(hWindow);
    UINT32 windowTextLength = strlen(pWindowText);
    INT32 caretIndex = GetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX);

    // Copy from start to caret position
    strncpy(&UiTbTextBuffer[0], pWindowText, caretIndex);

    // Add character
    UiTbTextBuffer[caretIndex] = asciiCode;

    // Copy from caret position until the end
    strncpy(&UiTbTextBuffer[caretIndex + 1], &pWindowText[caretIndex], windowTextLength - caretIndex);

    // Make sure 0 termination added
    UiTbTextBuffer[windowTextLength + 1] = '\0';

    caretIndex++;

    if (caretIndex > windowTextLength + 1)
    {   
        caretIndex = windowTextLength + 1;
    }

    SetWindowText(hWindow, &UiTbTextBuffer[0]);
    SetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX, caretIndex);
    SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);
}

VOID HandleDeleteFromCaret(HWINDOW hWindow)
{
    PCHAR pWindowText = GetWindowTextPointer(hWindow);
    UINT32 windowTextLength = strlen(pWindowText);
    INT32 caretIndex = GetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX);

    if (caretIndex == windowTextLength)
    {
        return;
    }

    // Copy from start to caret position
    strncpy(&UiTbTextBuffer[0], pWindowText, caretIndex);

    // Copy from caret position + 1 until the end
    strncpy(&UiTbTextBuffer[caretIndex], &pWindowText[caretIndex + 1], windowTextLength - caretIndex - 1);

    // Make sure 0 termination added
    UiTbTextBuffer[windowTextLength] = '\0';

    SetWindowText(hWindow, &UiTbTextBuffer[0]);
    SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);
}

VOID HandleDeleteBackFromCaret(HWINDOW hWindow)
{
    PCHAR pWindowText = GetWindowTextPointer(hWindow);
    UINT32 windowTextLength = strlen(pWindowText);
    INT32 caretIndex = GetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX);

    if (caretIndex == 0)
    {
        return;
    }
    
    // Copy from start to caret position
    strncpy(&UiTbTextBuffer[0], pWindowText, caretIndex - 1);

    // Copy from caret position + 1 until the end
    strncpy(&UiTbTextBuffer[caretIndex - 1], &pWindowText[caretIndex], windowTextLength - caretIndex);

    // Make sure 0 termination added
    UiTbTextBuffer[windowTextLength] = '\0';

    caretIndex--;

    if (caretIndex < 0)
    {   
        caretIndex = 0;
    }

    SetWindowText(hWindow, &UiTbTextBuffer[0]);
    SetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX, caretIndex);
    SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);
}

VOID HandleHome(HWINDOW hWindow)
{
    SetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX, 0);
    SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);
}

VOID HandleEnd(HWINDOW hWindow)
{
    PCHAR pWindowText = GetWindowTextPointer(hWindow);
    INT32 caretIndex = strlen(pWindowText);
    SetWindowExtra(hWindow, TEXTBOX_EXTRA_CARET_INDEX, caretIndex);
    SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);
}