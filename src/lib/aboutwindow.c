#include <kldr.h>

static HWINDOW hButtonOK;
static HICON hIconLogo;
static HBITMAP hBitmap;

static CHAR AwWindowClass[]      = "ABOUT";
static CHAR AwWindowTitle[]      = "About Kore OS";

static CHAR AwSystemName[]       = "Kore (R) OS";
static CHAR AwSystemVersion[]    = "Version 0.2 (Build 1246)";
static CHAR AwCopyright[]        = "Copyright (C) 2018-2022 Kore Technologies";
static CHAR AwEvaluation[]       = "Developer Preview";
static CHAR AwRegistration[]     = "Registered to:";
static CHAR AwRegistrationInfo[] = "Unregistered Developer Preview";
static CHAR AwPhysicalMemory[]   = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
static CHAR AwTextMemory[]       = " KB";

static CHAR AwMemory[]           = "Physical memory available to Kore OS:";

static CHAR AwOkButtonCaption[]  = "OK";

static UINT32 AwTextHeight;

static UINT32 physicalMemory;

// WRONG:
//static PCHAR pAwMemory           = (PCHAR) &"Physical memory available to Kore OS:";
// GOOD:
//static CHAR AwMemory[]           = "Physical memory available to Kore OS:";
//static TCHAR szAppName[] = TEXT ("HelloWin") ;
//wndclass.lpszClassName = szAppName ;

UINT32 AboutWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);

VOID RegisterAboutWindow()
{
    RegisterWindow((PCHAR) &AwWindowClass, 0, 0, 0, 0, GetColorFromComponents(20, 35, 80), (PVOID) AboutWindowProcedure, 0);
}

VOID CreateAboutWindow()
{
    physicalMemory = GetTotalPhysicalMemory();
    // uitoa(physicalMemory / 1024, (char*) &AwPhysicalMemory, 10);
    
    // UINT32 textLength = strlen((const char*) &AwPhysicalMemory);
    // strcpy((char*) &AwPhysicalMemory + textLength, (char*) &AwTextMemory);

    //hIconLogo = LoadIconFromMemory(pLogoIcon);
    //hIconLogo = LoadIconFromMemory(pProgramsIcon);
    //CreateCompatibleIcon(hIconLogo);
    //hBitmap = LoadBitmapFromMemory(pBarBitmap);
    //hBitmap = CreateCompatibleBitmap(LoadBitmapFromMemory(pLogoBitmap));        
    //hBitmap = LoadBitmapFromMemory(pLogoBitmap);        

    hAboutWindow = CreateWindowByClass((PCHAR) &AwWindowClass, (PCHAR) &AwWindowTitle, 205, 205, 420, 250, 0, 0, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_HAS_DIALOG_BORDER | WINDOW_FLAG_HAS_CAPTION);
    //hAboutWindow = CreateWindowByClass((PCHAR) &AwWindowClass, (PCHAR) &AwWindowTitle, 205, 205, 420, 350, 0, 0, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_HAS_DIALOG_BORDER | WINDOW_FLAG_HAS_CAPTION);
    hButtonOK = CreateButton((PCHAR) &AwOkButtonCaption, 330, 195, 75, 23, hAboutWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);
}

UINT32 AboutWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
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
        AwTextHeight = GetTextHeight(hDeviceContext, (PCHAR) &AwSystemName) + 5;

        left = windowRectangle.Left;
        top = windowRectangle.Top;
        right = windowRectangle.Right;
        bottom = windowRectangle.Bottom; 
       
        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        DrawFilledRectangleA(hDeviceContext, left, top, right, bottom);

        SetTextColor(hDeviceContext, ColorControlContentAreaText);

        // -- System --
        windowRectangle.Left += 120;
        windowRectangle.Top += 15;
        windowRectangle.Right -= 10;
        DrawText(hDeviceContext, (PCHAR) &AwSystemName, &windowRectangle, 0);

        windowRectangle.Top += AwTextHeight;
        DrawText(hDeviceContext, (PCHAR) &AwSystemVersion, &windowRectangle, 0);

        windowRectangle.Top += AwTextHeight;
        DrawText(hDeviceContext, (PCHAR) &AwCopyright, &windowRectangle, 0);

        windowRectangle.Top += AwTextHeight;
        DrawText(hDeviceContext, (PCHAR) &AwEvaluation, &windowRectangle, 0);

        // -- Registration --
        windowRectangle.Top += AwTextHeight * 2;
        DrawText(hDeviceContext, (PCHAR) &AwRegistration, &windowRectangle, 0);

        windowRectangle.Top += AwTextHeight;
        DrawText(hDeviceContext, (PCHAR) &AwRegistrationInfo, &windowRectangle, 0);

        DrawControlState(hDeviceContext, 120, 147, 405, 149, DRAW_CONTROL_STATE_EDGE_THIN_SUNKEN);

        // -- Memory --
        windowRectangle.Top += AwTextHeight * 2;
        DrawText(hDeviceContext, (PCHAR) &AwMemory, &windowRectangle, 0);
        DrawText(hDeviceContext, (PCHAR) GetFormattedText(&AwPhysicalMemory[0], "%u KB", physicalMemory / 1024), &windowRectangle, DRAW_TEXT_FLAG_ALIGN_RIGHT);

        // DrawIcon causes problems with moving windows =S

        //HBITMAP hBitmap = CreateCompatibleBitmap(LoadBitmapFromMemory(pBarBitmap));
        //HBITMAP hBitmap = CreateCompatibleBitmap(LoadBitmapFromMemory(pLogoBitmap));        
        //HBITMAP hBitmap = LoadBitmapFromMemory(pLogoBitmap);
        //DrawBitmap(hDeviceContext, 0, 0, hBitmap);
        //DrawIcon(hDeviceContext, 40, 20, hIconLogo);

        DrawEnd(hDeviceContext);
        ReleaseDeviceContext(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_COMMAND)
    {
        if (param1 = COMMAND_BUTTON_CLICKED)
        {
            HideWindow(hWindow);
        }
    }


    DefaultWindowProcedure(hWindow, message, param1, param2);
}