#include <kldr.h>

static HWINDOW hButtonWelcome;
static HWINDOW hButtonAbout;
static HWINDOW hButtonSystem;
static HWINDOW hButtonMystify;
static HWINDOW hButtonPalette;
static HWINDOW hButtonMemoryMap;
static HWINDOW hButtonClock;

static HWINDOW hButtonReboot;
static HWINDOW hButtonShutdown;

static HWINDOW hButtonSet640x480x2;
static HWINDOW hButtonSet640x480x16;
static HWINDOW hButtonSet320x200x256;
static HWINDOW hButtonSet320x240x256;

static CHAR LwWindowClass[]      = "LAUNCHER";
static CHAR LwWindowTitle[]      = "Launcher";

// Bad:
//static PCHAR pLwButtonCaptionWelcome   = (PCHAR) &"Welcome";
// Good:

static CHAR LwButtonCaptionWelcome[]     = "Welcome";
static CHAR LwButtonCaptionAbout[]       = "About";
static CHAR LwButtonCaptionSystem[]      = "System";
static CHAR LwButtonCaptionMystify[]     = "Mystify";
static CHAR LwButtonCaptionPalette[]     = "Palette";
static CHAR LwButtonCaptionMemoryMap[]   = "Memory Map";
static CHAR LwButtonCaptionClock[]   = "Clock";

static CHAR LwButtonCaptionReboot[]      = "Reboot";
static CHAR LwButtonCaptionShutdown[]    = "Shutdown...";

static CHAR LwButtonCaption640x480x2[]   = "640x480x2";
static CHAR LwButtonCaption640x480x16[]  = "640x480x16";
static CHAR LwButtonCaption320x200x256[] = "320x200x256";
static CHAR LwButtonCaption320x240x256[] = "320x240x256 (ModeX)";

static CHAR LwTextShutdownMessageLine1[] = "It's now safe to turn off";
static CHAR LwTextShutdownMessageLine2[] = "your computer.";

UINT32 LauncherWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);

VOID RegisterLauncherWindow()
{
    hLauncherWindow         = (HWINDOW) NULL;

    hButtonWelcome          = (HWINDOW) NULL;
    hButtonAbout            = (HWINDOW) NULL;
    hButtonSystem           = (HWINDOW) NULL;
    hButtonMystify          = (HWINDOW) NULL;

    hButtonPalette          = (HWINDOW) NULL;
    hButtonMemoryMap        = (HWINDOW) NULL;
    hButtonClock            = (HWINDOW) NULL;

    hButtonReboot           = (HWINDOW) NULL;
    hButtonShutdown         = (HWINDOW) NULL;

    hButtonSet640x480x2     = (HWINDOW) NULL;
    hButtonSet640x480x16    = (HWINDOW) NULL;
    hButtonSet320x200x256   = (HWINDOW) NULL;
    hButtonSet320x240x256   = (HWINDOW) NULL;    

    RegisterWindow((PCHAR) &LwWindowClass, 0, 0, 0, 0, GetColorFromComponents(20, 35, 80), (PVOID) LauncherWindowProcedure, 0);
}

VOID CreateLauncherWindow()
{
    UINT32 windowSizeX = 100;
    UINT32 windowSizeY = 335;

    hLauncherWindow = CreateWindowByClass((PCHAR) &LwWindowClass, (PCHAR) &LwWindowTitle, 5, 10, windowSizeX + 6, windowSizeY + 26, 0, 0, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_HAS_DIALOG_BORDER | WINDOW_FLAG_HAS_CAPTION);

    hButtonWelcome        = CreateButton(&LwButtonCaptionWelcome[0], 5,  5, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);
    hButtonAbout          = CreateButton(&LwButtonCaptionAbout[0],   5, 30, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);
    hButtonSystem         = CreateButton(&LwButtonCaptionSystem[0],  5, 55, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);
    hButtonMystify        = CreateButton(&LwButtonCaptionMystify[0], 5, 80, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_IS_DISABLED);

    hButtonPalette        = CreateButton(&LwButtonCaptionPalette[0], 5, 105, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE);
    hButtonMemoryMap      = CreateButton(&LwButtonCaptionMemoryMap[0], 5, 130, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE);
    hButtonClock          = CreateButton(&LwButtonCaptionClock[0], 5, 155, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_IS_DISABLED);
    
    hButtonReboot         = CreateButton(&LwButtonCaptionReboot[0],   5, 180, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE);
    hButtonShutdown       = CreateButton(&LwButtonCaptionShutdown[0], 5, 205, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE);

    hButtonSet640x480x2   = CreateButton(&LwButtonCaption640x480x2[0], 5, 230, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE);
    hButtonSet640x480x16  = CreateButton(&LwButtonCaption640x480x16[0], 5, 255, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE);
    hButtonSet320x200x256 = CreateButton(&LwButtonCaption320x200x256[0], 5, 280, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE);
    hButtonSet320x240x256 = CreateButton(&LwButtonCaption320x240x256[0], 5, 305, 90, 23, hLauncherWindow, WINDOW_FLAG_IS_VISIBLE);
}

UINT32 LauncherWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    if (message == WINDOW_MESSAGE_PAINT)
    {
        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);

        DrawBegin(hDeviceContext);

        DrawFilledRectangleC(hDeviceContext, &clientRectangle);

        DrawEnd(hDeviceContext);
        
        ReleaseDeviceContext(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_COMMAND)
    {
        if (param1 == COMMAND_BUTTON_CLICKED)
        {
            if (param2 == hButtonWelcome)
            {
                CreateWelcomeWindow();

                return;
            }

            if (param2 == hButtonAbout)
            {
                CreateAboutWindow();

                return;
            }

            if (param2 == hButtonSystem)
            {
                CreateSystemPropertiesWindow();

                return;
            }

            if (param2 == hButtonMystify)
            {
                CreateMystifyWindow();

                return;
            }

            if (param2 == hButtonPalette)
            {
                CreatePaletteWindow();

                return;
            }

            if (param2 == hButtonMemoryMap)
            {
                CreateMemoryMapWindow();

                return;
            }

            if (param2 == hButtonClock)
            {
                CreateClockWindow();

                return;
            }
 
            if (param2 == hButtonReboot)
            {
                MachineUtilityReboot();

                return;
            }
 
            if (param2 == hButtonShutdown)
            {
                HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hDesktopWindow);
                RECTANGLE windowRectangle;
                GetWindowRectangle(hDesktopWindow, &windowRectangle);

                DrawBegin(hDeviceContext);

                //SetBackgroundColor(hDeviceContext, 0xFF000000);
                //DrawFilledRectangleC(hDeviceContext, &windowRectangle);
                SetForegroundColor(hDeviceContext, 0xFF000000);
                SetDrawMode(hDeviceContext, DRAW_MODE_PEN_DOT);

                for(UINT32 i = 0; i < windowRectangle.Bottom / 2; i++)
                {
                    DrawLineA(hDeviceContext, windowRectangle.Left, i, windowRectangle.Right, i);
                }

                SetDrawMode(hDeviceContext, DRAW_MODE_NORMAL);

                DrawEnd(hDeviceContext);


                SetVideoMode(GetPrimaryDisplayDevice(), 0);

                windowRectangle.Left = 0;
                windowRectangle.Right = 320;
                windowRectangle.Top = 0;
                windowRectangle.Bottom = 200;

                DrawBegin(hDeviceContext);

                SetBackgroundColor(hDeviceContext, 0xFF000000);
                DrawFilledRectangleC(hDeviceContext, &windowRectangle);

                SetTextColor(hDeviceContext, 0xFF800000);
                
                UINT32 textHeight = GetTextHeight(hDeviceContext, (PCHAR) &LwTextShutdownMessageLine1);
                DrawText(hDeviceContext, (PCHAR) &LwTextShutdownMessageLine1, &windowRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER | DRAW_TEXT_FLAG_BOLD);
                windowRectangle.Top += 2 * textHeight;
                DrawText(hDeviceContext, (PCHAR) &LwTextShutdownMessageLine2, &windowRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER | DRAW_TEXT_FLAG_BOLD);
                windowRectangle.Top -= 2 * textHeight;

                SetTextColor(hDeviceContext, 0xFFFF8000);
                windowRectangle.Left -= 1;
                windowRectangle.Right -= 1;
                windowRectangle.Top -= 1;
                windowRectangle.Bottom -= 1;
                DrawText(hDeviceContext, (PCHAR) &LwTextShutdownMessageLine1, &windowRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER | DRAW_TEXT_FLAG_BOLD);
                windowRectangle.Top += 2 * textHeight;
                DrawText(hDeviceContext, (PCHAR) &LwTextShutdownMessageLine2, &windowRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER | DRAW_TEXT_FLAG_BOLD);
                windowRectangle.Top -= 2 * textHeight;

                DrawEnd(hDeviceContext);

                HideMouse();

                MachineDisableInterrupts();
                HALT;
            }

            if (param2 == hButtonSet640x480x2)
            {
                SetVideoMode(GetPrimaryDisplayDevice(), 1);
                ResetWindows();

                return;
            } 

            if (param2 == hButtonSet640x480x16)
            {
                SetVideoMode(GetPrimaryDisplayDevice(), 2);
                ResetWindows();

                return;
            } 

            if (param2 == hButtonSet320x200x256)
            {
                SetVideoMode(GetPrimaryDisplayDevice(), 0);
                ResetWindows();

                return;
            } 

            if (param2 == hButtonSet320x240x256)
            {
                SetVideoMode(GetPrimaryDisplayDevice(), 3);
                ResetWindows();

                return;
            } 

        }
    }

    DefaultWindowProcedure(hWindow, message, param1, param2);
}