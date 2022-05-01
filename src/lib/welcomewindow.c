#include <kldr.h>

static CHAR WwWindowClass[]        = "WELCOME";
static CHAR WwWindowTitle[]        = "Welcome";

static CHAR WwCloseButtonCaption[] = "Close";

static CHAR WwTextWelcome[]        = "Welcome to Kore OS!";

HWINDOW hButtonClose;

UINT32 WelcomeWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);

VOID RegisterWelcomeWindow()
{
    RegisterWindow((PCHAR) &WwWindowClass, 0, 0, 0, 0, GetColorFromComponents(20, 35, 80), (PVOID) WelcomeWindowProcedure, 0);
}

VOID CreateWelcomeWindow()
{
    hWelcomeWindow = CreateWindowByClass((PCHAR) &WwWindowClass, (PCHAR) &WwWindowTitle, 150, 150, 280, 120, 0, 0, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_HAS_DIALOG_BORDER | WINDOW_FLAG_HAS_CAPTION);
    hButtonClose   = CreateButton((PCHAR) &WwCloseButtonCaption, 95, 65, 90, 23, hWelcomeWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);

    CreateCheckbox((PCHAR) &"Show small icons in Start menu", 75, 10, 190, 40, hWelcomeWindow, WINDOW_FLAG_IS_VISIBLE);
}

UINT32 WelcomeWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    if (message == WINDOW_MESSAGE_PAINT)
    {
        INT32 left;
        INT32 top;
        INT32 right;
        INT32 bottom;

        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
        DrawBegin(hDeviceContext);

        RECTANGLE windowRectangle;
        GetClientRectangle(hWindow, &windowRectangle);
        
        left = windowRectangle.Left;
        top = windowRectangle.Top;
        right = windowRectangle.Right;
        bottom = windowRectangle.Bottom;

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);

        SetTextColor(hDeviceContext, ColorControlContentAreaText);
        windowRectangle.Bottom -= 30;
        DrawText(hDeviceContext, &WwTextWelcome[0], &windowRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);

        SetForegroundColor(hDeviceContext, 0xFFFF00FF);
        SetBackgroundColor(hDeviceContext, 0xFFFF00FF);
        
        DrawRectangleA(hDeviceContext,       20, 20, 40, 40);
        DrawFilledRectangleA(hDeviceContext, 20, 42, 40, 62);
        DrawFilledRectangleA(hDeviceContext, 42, 20, 62, 40);
        DrawRectangleA(hDeviceContext,       42, 42, 62, 62);

        DrawEnd(hDeviceContext);
        ReleaseDeviceContext(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_COMMAND)
    {
        if (param1 == COMMAND_BUTTON_CLICKED)
        {
            HideWindow(hWindow);
            //BugCheck(NULL, (PCHAR) &"BLUE SCREEN OF DEATH TEST", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "This is extra info:", 0xBAADB002);
        }

        return;
    }

    DefaultWindowProcedure(hWindow, message, param1, param2);
}