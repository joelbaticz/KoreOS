#include "kldr.h"

static CHAR UiDeWindowClass[]        = "DESKTOP";
static CHAR UiDeWindowTitle[]        = "Desktop";
static CHAR UiDeBuildText[]          = "Kore OS Developer Preview";

static VIDEO_MODE_INFORMATION videoModeInfo;
static UINT32 desktopWidth;
static UINT32 desktopHeight;
static HICON hIcon;

UINT32 DesktopWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);

VOID RegisterDesktop()
{
    RegisterWindow((PCHAR) &UiDeWindowClass, 0, 0, 0, 0, GetColorFromComponents(20, 35, 80), (PVOID) DesktopWindowProcedure, 0);
}

VOID CreateDesktop()
{
    GetCurrentVideoModeInformation(GetPrimaryDisplayDevice(), &videoModeInfo);

    desktopWidth = videoModeInfo.Width;
    desktopHeight = videoModeInfo.Height;

    hIcon = LoadIconFromMemory(pProgramsIcon);

    // NEVER ADD THE VISIBLE FLAG TO THE DESKTOP AS A BUNCH OF FUNCTIONS NEED THE HDESKTOPWINDOW
    hDesktopWindow = CreateWindowByClass((PCHAR) &UiDeWindowClass, (PCHAR) &UiDeWindowTitle, 0, 0, desktopWidth, desktopHeight, 0, 0, 0);

    SetVisible(hDesktopWindow, TRUE);    
    SendMessage(hDesktopWindow, WINDOW_MESSAGE_PAINT, 0, 0);

//BugCheck(NULL, (PCHAR) &"DESKTOP CREATED", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Desktop window handle:", hDesktopWindow);    
}

UINT32 DesktopWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{

// if (hWindow == 0)
// {
//     BugCheck(NULL, (PCHAR) &"GOGOGO", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);
// }

    UiDrawText("DesktopWindowProcedure()");

    if (message == WINDOW_MESSAGE_PAINT)
    {
        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
        DrawBegin(hDeviceContext);

        RECTANGLE desktopRectangle;
        GetScreenRectangle(hWindow, &desktopRectangle);

        UINT32 desktopWidth = desktopRectangle.Right;
        UINT32 desktopHeight = desktopRectangle.Bottom;

        // Draw background
        SetBackgroundColor(hDeviceContext, ColorDesktop);
        DrawFilledRectangleA(hDeviceContext, 0, 0, desktopWidth, desktopHeight);

        // for(UINT32 y = 0; y < desktopHeight ; y+=32)
        // {
        //     for(UINT32 x = 0; x < desktopWidth; x+=32)
        //     {
        //         DrawIcon(hDeviceContext, x, y, hIcon);
        //     }
        // }

        // Draw build text
        RECTANGLE rectangle;
        PCHAR buildText = (PCHAR) &UiDeBuildText;

        rectangle.Left = 0;
        rectangle.Top = 0;
        rectangle.Right = desktopWidth - 2;
        rectangle.Bottom = desktopHeight - 1;

        SetTextColor(hDeviceContext, ColorDesktopText);
        DrawText(hDeviceContext, buildText, &rectangle, DRAW_TEXT_FLAG_ALIGN_RIGHT | DRAW_TEXT_FLAG_ALIGN_BOTTOM);

        DrawEnd(hDeviceContext);
        ReleaseDeviceContext(hWindow);

        return 0;
    }

    // if (message == WINDOW_MESSAGE_MOUSE_LEAVE)
    // {
    //     HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
    //     DrawBegin(hDeviceContext);

    //     RECTANGLE windowRectangle;
    //     GetClientRectangle(hWindow, &windowRectangle);

    //     UINT32 left = windowRectangle.Left;
    //     UINT32 top = windowRectangle.Top;
    //     UINT32 right = windowRectangle.Right;
    //     UINT32 bottom = windowRectangle.Bottom;  

    //     SetForegroundColor(hDeviceContext, 0xFF000000);
    //     DrawRectangleA(hDeviceContext, left, top, right, bottom);

    //     DrawEnd(hDeviceContext);
    //     ReleaseDeviceContext(hWindow);

    //     return 0;
    // }
    
    // if (message == WINDOW_MESSAGE_MOUSE_ENTER)
    // {
    //     HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
    //     DrawBegin(hDeviceContext);

    //     RECTANGLE windowRectangle;
    //     GetClientRectangle(hWindow, &windowRectangle);

    //     UINT32 left = windowRectangle.Left;
    //     UINT32 top = windowRectangle.Top;
    //     UINT32 right = windowRectangle.Right;
    //     UINT32 bottom = windowRectangle.Bottom;  

    //     SetForegroundColor(hDeviceContext, 0xFFFF0000);
    //     DrawRectangleA(hDeviceContext, left, top, right, bottom);

    //     DrawEnd(hDeviceContext);
    //     ReleaseDeviceContext(hWindow);

    //     return 0;
    // }
}