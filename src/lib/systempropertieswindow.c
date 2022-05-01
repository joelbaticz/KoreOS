#include <kldr.h>

HICON   hLogoIcon;
HWINDOW hButtonOK;
HWINDOW hButtonCancel;

UINT32  SpwTextHeight;
UINT32  SpwWindowWidth;
UINT32  SpwWindowHeight;

static CHAR SpwWindowClass[]          = "SYSPROP";
static CHAR SpwWindowTitle[]          = "System Properties";

static CHAR SpwTextSystem[]           = "System:";
static CHAR SpwTextSystemName[]       = "Kore OS 2022";
static CHAR SpwTextSystemEdition[]    = "Developer Preview";
static CHAR SpwTextSystemVersion[]    = "Version 0.2.2022";

static CHAR SpwTextRegistration[]     = "Registered to:";
static CHAR SpwTextRegistrationInfo[] = "Unregistered Developer Preview";

static CHAR SpwTextDevice[]           = "Device:";
static CHAR SpwTextProcessor[]        = "x86 compatible processor";
static CHAR SpwTextMemory[]           = " KB RAM";
static CHAR SpwPhysicalMemory[]       = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

static CHAR SpwButtonCaptionOK[]      = "OK";
static CHAR SpwButtonCaptionCancel[]  = "Cancel";

UINT32 SystemPropertiesWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);

VOID SpwShowVariableNameAndValue(HDEVICE_CONTEXT hDeviceContext, PCHAR pVariableName, UINT32 variableValue, UINT32 base, PRECTANGLE pWindowRectangle);
VOID ToStyledString(PCHAR source, PCHAR dest, CHAR separator);

VOID RegisterSystemPropertiesWindow()
{
    RegisterWindow((PCHAR) &SpwWindowClass, 0, 0, 0, 0, GetColorFromComponents(20, 35, 80), (PVOID) SystemPropertiesWindowProcedure, 0);
}

VOID CreateSystemPropertiesWindow()
{
    UINT32 physicalMemory = GetTotalPhysicalMemory();
    uitoa(physicalMemory / 1024, (char*) &SpwPhysicalMemory, 10);
    
    UINT32 textLength = strlen((char*) &SpwPhysicalMemory);
    strcpy((char*) &SpwPhysicalMemory + textLength, (const char*)&SpwTextMemory);

    SpwWindowWidth = 400;
    SpwWindowHeight = 420;

    hLogoIcon = LoadIconFromMemory(pLogoIcon);
    CreateCompatibleIcon(hLogoIcon);

    hSystemPropertiesWindow = CreateWindowByClass((PCHAR) &SpwWindowClass, (PCHAR) &SpwWindowTitle, 115, 10, SpwWindowWidth + 6, SpwWindowHeight + 26, 0, 0, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_HAS_DIALOG_BORDER | WINDOW_FLAG_HAS_CAPTION);
    hButtonOK = CreateButton((PCHAR) &SpwButtonCaptionOK, 239, 395, 75, 23, hSystemPropertiesWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);
    hButtonCancel = CreateButton((PCHAR) &SpwButtonCaptionCancel, 319, 395, 75, 23, hSystemPropertiesWindow, WINDOW_FLAG_IS_VISIBLE);    
}

UINT32 SystemPropertiesWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    if (message == WINDOW_MESSAGE_PAINT)
    {
        INT32 left;
        INT32 top;
        INT32 right;
        INT32 bottom;

        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
        SpwTextHeight = 17;//GetTextHeight(hDeviceContext, SpwTextSystem) + 2;

        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);
        
        left = clientRectangle.Left;
        top = clientRectangle.Top;
        right = clientRectangle.Right;
        bottom = clientRectangle.Bottom;        

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetTextColor(hDeviceContext, ColorControlContentAreaText);

        DrawBegin(hDeviceContext);

        // -- Window --
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);
        DrawControlState(hDeviceContext, left + 6, top + 26, right - 6, bottom - 36, DRAW_CONTROL_STATE_EDGE_RAISED);

        // -- Logo --
        UINT32 logoIconSize = 32;
        RECTANGLE logoRectangle;
        logoRectangle.Left   = 55;
        logoRectangle.Top    = 75;
        logoRectangle.Right  = logoRectangle.Left + 98;
        logoRectangle.Bottom = logoRectangle.Top  + 98;

        SetBackgroundColor(hDeviceContext, GetColorFromComponents(128, 128, 128));
        DrawControlState(hDeviceContext, logoRectangle.Left, logoRectangle.Top, logoRectangle.Right, logoRectangle.Bottom, DRAW_CONTROL_STATE_EDGE_THIN_SUNKEN);
        DrawFilledRectangleA(hDeviceContext, logoRectangle.Left + 1, logoRectangle.Top + 1, logoRectangle.Right - 1, logoRectangle.Bottom - 1);
        DrawIcon(hDeviceContext, (logoRectangle.Left + logoRectangle.Right - logoIconSize) / 2, (logoRectangle.Top + logoRectangle.Bottom - logoIconSize) / 2, hLogoIcon);

        // -- System --

        clientRectangle.Left += 200;
        clientRectangle.Top  += 50;
        DrawText(hDeviceContext, (PCHAR) &SpwTextSystem, &clientRectangle, 0);

        clientRectangle.Left += 10;
        clientRectangle.Top  += SpwTextHeight;
        DrawText(hDeviceContext, (PCHAR) &SpwTextSystemName, &clientRectangle, 0);

        clientRectangle.Top  += SpwTextHeight;
        DrawText(hDeviceContext, (PCHAR) &SpwTextSystemEdition, &clientRectangle, 0);

        clientRectangle.Top  += SpwTextHeight;
        DrawText(hDeviceContext, (PCHAR) &SpwTextSystemVersion, &clientRectangle, 0);

        // -- Registration --
        clientRectangle.Left -= 10;
        clientRectangle.Top  += SpwTextHeight * 2;
        DrawText(hDeviceContext, (PCHAR) &SpwTextRegistration, &clientRectangle, 0);

        clientRectangle.Left += 10;
        clientRectangle.Top  += SpwTextHeight;
        DrawText(hDeviceContext, (PCHAR) &SpwTextRegistrationInfo, &clientRectangle, 0);

        // -- Device --
        clientRectangle.Left -= 10;
        clientRectangle.Top += SpwTextHeight * 2;
        DrawText(hDeviceContext, (PCHAR) &SpwTextDevice, &clientRectangle, 0);

        clientRectangle.Left += 10;
        clientRectangle.Top += SpwTextHeight;
        DrawText(hDeviceContext, (PCHAR) &SpwTextProcessor, &clientRectangle, 0);

        clientRectangle.Top += SpwTextHeight;
        DrawText(hDeviceContext, (PCHAR) &SpwPhysicalMemory, &clientRectangle, 0);

        // Bitmap
        //LogoBitmapInit();
        //HBITMAP hBitmap = LoadBitmapFromMemory(pLogoBitmap);
        //BOOL success = DrawBitmap(hDeviceContext, 0, 0, hBitmap);

        // SetTextColor(hDeviceContext, 0xFFFF0000);
        // clientRectangle.Top += SpwTextHeight;
        //SpwShowVariableNameAndValue(hDeviceContext, (PCHAR) &"hBitmap", (UINT32) hBitmap, 10, &clientRectangle);

        // if (success)
        // {
        //     BITMAPINFOHEADER biHeader;
        //     GetBitmapInfo(hBitmap, &biHeader);

        //     SetTextColor(hDeviceContext, 0xFFFF0000);
        //     //DrawText(hDeviceContext, SpwTextMemory, &clientRectangle, 0);
        //     clientRectangle.Top += SpwTextHeight;
        //     SpwShowVariableNameAndValue(hDeviceContext, (PCHAR) &"BitmapWidth", biHeader.Width, 10, &clientRectangle);
        //     clientRectangle.Top += SpwTextHeight;
        //     SpwShowVariableNameAndValue(hDeviceContext, (PCHAR) &"BitmapHeight", biHeader.Height, 10, &clientRectangle);
            
        // }

        DrawEnd(hDeviceContext);
        ReleaseDeviceContext(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_COMMAND)
    {
        if (param1 = COMMAND_BUTTON_CLICKED)
        {
            //if (param2 == hButtonOK || param2 == hButtonCancel)
            {
                HideWindow(hWindow);
            }
        }
    }

    DefaultWindowProcedure(hWindow, message, param1, param2);
}


VOID SpwShowVariableNameAndValue(HDEVICE_CONTEXT hDeviceContext, PCHAR pVariableName, UINT32 variableValue, UINT32 base, PRECTANGLE pWindowRectangle)
{
    SetTextColor(hDeviceContext, 0xFFFF0000);

    CHAR textBuffer[40] = { 0 };
    size_t textLength;

    textLength = strlen((const char*)pVariableName);
    strcpy(&textBuffer[0], (const char*)pVariableName);
    textBuffer[textLength] = '=';
    itoa(variableValue, &textBuffer[textLength + 1], base);

    DrawText(hDeviceContext, &textBuffer[0], pWindowRectangle, 0);
}
