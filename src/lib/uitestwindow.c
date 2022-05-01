#include <kldr.h>

static CHAR UiWindowClass[]        = "UITEST";
static CHAR UiWindowTitle[]        = "User Interface Test";

static CHAR UiButtonCaptionClose[] = "Close";
static CHAR UiButtonCaptionBsod[]  = "Invoke BSOD";

static HWINDOW hUiButtonClose;
static HWINDOW hUiButtonBsod;

UINT32 UiTestWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);

VOID RegisterUiTestWindow()
{
    RegisterWindow((PCHAR) &UiWindowClass, 0, 0, 0, 0, GetColorFromComponents(20, 35, 80), (PVOID) UiTestWindowProcedure, 0);
}

VOID CreateUiTestWindow()
{
    hUiTestWindow = CreateWindowByClass((PCHAR) &UiWindowClass, (PCHAR) &UiWindowTitle, 120, 10, 400, 400, 0, 0, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_HAS_DIALOG_BORDER | WINDOW_FLAG_HAS_CAPTION);

    hUiButtonClose   = CreateButton((PCHAR) &UiButtonCaptionClose, 300, 350, 90, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);
    hUiButtonBsod   = CreateButton((PCHAR) &UiButtonCaptionBsod, 205, 350, 90, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);

    CreateButton((PCHAR) &"Button 1 (Default)", 5, 10, 120, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_DEFAULT | BUTTON_FLAG_IS_FOCUSED);
    CreateButton((PCHAR) &"Button 2", 5, 40, 120, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE);
    CreateButton((PCHAR) &"Button 3 (Disabled)", 5, 70, 120, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_IS_DISABLED);

    CreateCheckbox((PCHAR) &"Checkbox 1", 5, 100, 190, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE);
    CreateCheckbox((PCHAR) &"Checkbox 2", 5, 130, 190, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE | CHECKBOX_FLAG_IS_CHECKED);
    CreateCheckbox((PCHAR) &"Checkbox 3 (Disabled)", 5, 160, 190, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_IS_DISABLED | CHECKBOX_FLAG_IS_CHECKED);

    CreateRadiobutton((PCHAR) &"Radiobutton 1", 5, 190, 190, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE);
    CreateRadiobutton((PCHAR) &"Radiobutton 2", 5, 220, 190, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE | RADIOBUTTON_FLAG_IS_CHECKED);
    CreateRadiobutton((PCHAR) &"Radiobutton 3 (Disabled)", 5, 250, 190, 23, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_IS_DISABLED | RADIOBUTTON_FLAG_IS_CHECKED);

    CreateTextBox((PCHAR) &"TextBox", 5, 280, 290, 21, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE);
    //CreateTextBox((PCHAR) &"TextBox", 5, 310, 290, 21, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_IS_DISABLED);
    CreateTextBox((PCHAR) &"TextBox2", 5, 310, 290, 21, hUiTestWindow, WINDOW_FLAG_IS_VISIBLE);
}

UINT32 UiTestWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    if (message == WINDOW_MESSAGE_PAINT)
    {
        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
        DrawBegin(hDeviceContext);

        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);
        
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        DrawFilledRectangleC(hDeviceContext, &clientRectangle);

        DrawEnd(hDeviceContext);
        ReleaseDeviceContext(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_COMMAND)
    {
        if (param1 == COMMAND_BUTTON_CLICKED && param2 == hUiButtonClose)
        {
            HideWindow(hWindow);
        }

        if (param1 == COMMAND_BUTTON_CLICKED && param2 == hUiButtonBsod)
        {
            BugCheck(NULL, (PCHAR) &"BLUE SCREEN OF DEATH TEST", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Extra info:", 0xBAADB002);
        }

        //return;
    }

    DefaultWindowProcedure(hWindow, message, param1, param2);
}