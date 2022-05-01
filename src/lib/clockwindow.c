#include <kldr.h>

static CHAR CwWindowClass[]        = "CLOCK";
static CHAR CwWindowTitle[]        = "Kore Clock";

static CHAR CwButtonCaptionClose[] = "Close";

static HBUTTON hButtonClose;

static DATETIME dateTime;

static CHAR textBuffer[40]  = { 0 };
static CHAR amText[10]      = " AM";
static CHAR pmText[10]      = " PM";

UINT32 ClockWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);
VOID ShowDateTime(HWINDOW hWindow);

VOID RegisterClockWindow()
{
    RegisterWindow((PCHAR) &CwWindowClass, 0, 0, 0, 0, GetColorFromComponents(20, 35, 80), (PVOID) ClockWindowProcedure, 0);
}

VOID CreateClockWindow()
{
    hClockWindow = CreateWindowByClass((PCHAR) &CwWindowClass, (PCHAR) &CwWindowTitle, 120, 20, 200, 200, 0, 0, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_HAS_DIALOG_BORDER | WINDOW_FLAG_HAS_CAPTION);
    
    hButtonClose = CreateButton((PCHAR) &CwButtonCaptionClose, 115, 150, 75, 23, hClockWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);

    dateTime.Year = 0;
    dateTime.Month = 0;
    dateTime.Day = 0;
    dateTime.Hour = 0;
    dateTime.Minute = 0;
    dateTime.Second = 0;
}

UINT32 ClockWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    if (message == WINDOW_MESSAGE_PAINT)
    {
        INT32 clientLeft;
        INT32 clientTop;
        INT32 clientRight;
        INT32 clientBottom;

        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
        DrawBegin(hDeviceContext);

        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);
        
        clientLeft = clientRectangle.Left;
        clientTop = clientRectangle.Top;
        clientRight = clientRectangle.Right;
        clientBottom = clientRectangle.Bottom;        

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetTextColor(hDeviceContext, 0xFF000000);
        DrawFilledRectangleA(hDeviceContext, clientLeft, clientTop, clientRight, clientBottom);

        ShowDateTime(hWindow);

        DrawEnd(hDeviceContext);

        return;
    }

    if (message == WINDOW_MESSAGE_ON_TIMER)
    {
        if (GetCapture() != (HWINDOW) NULL)
        {
            return;
        }

        if (!IsVisible(hWindow))
        {
            return;
        }

        ShowDateTime(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_COMMAND)
    {
        HideWindow(hWindow);
    }

    DefaultWindowProcedure(hWindow, message, param1, param2);
}

VOID ShowDateTime(HWINDOW hWindow)
{
    INT32 clientLeft;
    INT32 clientTop;
    INT32 clientRight;
    INT32 clientBottom;
    UINT32 textLength;
    UINT32 textFlags = DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER | DRAW_TEXT_FLAG_BOLD;

    DATETIME newDateTime;
    BOOL isPM;

    GetSystemDateTime(&newDateTime);

    if (dateTime.Hour > 12)
    {
        isPM = TRUE;
        dateTime.Hour -= 12;
    }

    if (dateTime.Hour == 0)
    {
        dateTime.Hour = 12;
    }

    if (dateTime.Hour == newDateTime.Hour &&
        dateTime.Minute == newDateTime.Minute &&
        dateTime.Second == newDateTime.Second)
    {
            return;
    }

    dateTime = newDateTime;

    // Drawing

    HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
    DrawBegin(hDeviceContext);

    RECTANGLE clientRectangle;
    GetClientRectangle(hWindow, &clientRectangle);
        
    clientLeft = clientRectangle.Left;
    clientTop = clientRectangle.Top;
    clientRight = clientRectangle.Right;
    clientBottom = clientRectangle.Bottom - 30;        

    SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
    SetTextColor(hDeviceContext, 0xFF000000);
    DrawFilledRectangleA(hDeviceContext, clientLeft, clientTop, clientRight, clientBottom);

    // Time
    for(UINT32 i = 0; i < 40; i++) { textBuffer[i] = 0; }
    
    if (dateTime.Minute >= 10 && dateTime.Second >= 10)
    {
        GetFormattedText(&textBuffer[0], "%d:%d:%d ", dateTime.Hour, dateTime.Minute, dateTime.Second);
    }
    else if (dateTime.Minute >= 10 && dateTime.Second < 10)
    {
        GetFormattedText(&textBuffer[0], "%d:%d:0%d ", dateTime.Hour, dateTime.Minute, dateTime.Second);
    }
    else if (dateTime.Minute < 10 && dateTime.Second >= 10)
    {
        GetFormattedText(&textBuffer[0], "%d:0%d:%d ", dateTime.Hour, dateTime.Minute, dateTime.Second);
    }
    else if (dateTime.Minute < 10 && dateTime.Second < 10)
    {
        GetFormattedText(&textBuffer[0], "%d:0%d:0%d ", dateTime.Hour, dateTime.Minute, dateTime.Second);
    }
    
    if (isPM)
    {
        strcpy(&textBuffer[strlen(&textBuffer[0])], "PM");
    }
    else
    {
        strcpy(&textBuffer[strlen(&textBuffer[0])], "AM");
    }

    clientRectangle.Bottom -= 20;
    DrawText(hDeviceContext, &textBuffer[0], &clientRectangle, textFlags);

    // Date
    for(UINT32 i = 0; i < 40; i++) { textBuffer[i] = 0; }
    GetFormattedText(&textBuffer[0], "%d/%d/%d ", dateTime.Year, dateTime.Month, dateTime.Day);
    clientRectangle.Top += 30;
    textFlags &= ~DRAW_TEXT_FLAG_BOLD;   
    DrawText(hDeviceContext, &textBuffer[0], &clientRectangle, textFlags);

    DrawEnd(hDeviceContext);
}
