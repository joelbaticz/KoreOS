#include <kldr.h>

HBUTTON hTestButton1;
HBUTTON hTestButton2;

static CHAR PwWindowClass[]      = "PALETTE";
static CHAR PwWindowTitle[]      = "Palette";

//static CHAR SswButtonCaptionWhatsNew[] = "What's New";
//static CHAR SswButtonCaptionRegistration[] = "Online Registration";

UINT32 PaletteWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);

VOID RegisterPaletteWindow()
{
    hTestButton1 = (HBUTTON) NULL;
    hTestButton2 = (HBUTTON) NULL;
    RegisterWindow((PCHAR) &PwWindowClass, 0, 0, 0, 0, GetColorFromComponents(20, 35, 80), (PVOID) PaletteWindowProcedure, 0);
}

VOID CreatePaletteWindow()
{
    hPaletteWindow = CreateWindowByClass((PCHAR) &PwWindowClass, (PCHAR) &PwWindowTitle, 20, 20, 128 + 6, 128 + 26, 0, 0, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_HAS_DIALOG_BORDER | WINDOW_FLAG_HAS_CAPTION);
    
    //hTestButton1 = CreateButton((PCHAR) &SswButtonCaptionWhatsNew, 10, 10, 114, 23, hPaletteWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);
    //hTestButton2 = CreateButton((PCHAR) &SswButtonCaptionRegistration, 10, 40, 114, 23, hPaletteWindow, WINDOW_FLAG_IS_VISIBLE);
}

UINT32 PaletteWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
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

        // clientLeft = 0;
        // clientTop  = 0;
        // clientRight  = 20;
        // clientBottom = 20;        

        SetBackgroundColor(hDeviceContext, ColorControlSurfaceNormal);
        SetTextColor(hDeviceContext, 0xFFFF00FF);
        DrawFilledRectangleA(hDeviceContext, clientLeft, clientTop, clientRight, clientBottom);

        DrawEnd(hDeviceContext);

//return;


        //static CHAR Text[] = "Kore Systems";
        //HBITMAP hBitmap = LoadBitmapFromMemory(pLogoBitmap);
        //HBITMAP hBitmap = CreateCompatibleBitmap(hDeviceContext, LoadBitmapFromMemory(pLogoBitmap));

        //HICON hIcon = LoadIconFromMemory(pLogoIcon);
        // HICON hIcon1 = LoadIconFromMemory(pPenIcon);
        // HICON hIcon2 = LoadIconFromMemory(pDesktopIcon);
        // HICON hIcon3 = LoadIconFromMemory(pProgramsIcon);
        // HICON hIcon4 = LoadIconFromMemory(pFloppyIcon);
        // HICON hIcon5 = LoadIconFromMemory(pArrowCursor);
        //ShowVariableNameAndValue(hDeviceContext, (PCHAR) &"Icon", (UINT32) hIcon, 10, &clientRectangle);

        // INT32 textWidth = GetTextWidth(hDeviceContext, &Text[0], 0) + 1;
        // INT32 textHeight = GetTextHeight(hDeviceContext, &Text[0]);;
        // INT32 deltaX = -1;
        // INT32 deltaY = -1;

        // clientRectangle.Left = 0;
        // clientRectangle.Right = clientRectangle.Left + textWidth;
        // clientRectangle.Top = (clientRectangle.Top + clientRectangle.Bottom) / 2;
        // clientRectangle.Bottom = clientRectangle.Top + textHeight;



        // CHAR textBuffer[40] = { 0 };
        // size_t textLength;

        // for(UINT32 i = 0; i < 1000; i++)
        // {
        //     itoa(i, &textBuffer[0], 10);


        //     DrawBegin(hDeviceContext);

        //     clientRectangle.Left += deltaX;
        //     clientRectangle.Right += deltaX;
        //     clientRectangle.Top += deltaY;
        //     clientRectangle.Bottom += deltaY;

        //     if (clientRectangle.Left < 3 || clientRectangle.Right > clientRight - 3)
        //     {
        //         deltaX *= -1;
        //     }

        //     if (clientRectangle.Top < 3 || clientRectangle.Bottom > clientBottom - 3)
        //     {
        //         deltaY *= -1;
        //     }

        //     // clientLeft = clientRectangle.Left;
        //     // clientRight = clientRectangle.Right;
        //     SetBackgroundColor(hDeviceContext, 0xFF000000);
        //     DrawFilledRectangle(hDeviceContext, clientRectangle.Left - 1, clientRectangle.Top - 1, clientRectangle.Right + 1, clientRectangle.Bottom + 1);
        //     //DrawFilledRectangle(hDeviceContext, clientLeft, clientTop, clientRight, clientBottom);
        //     //DrawBitmap(hDeviceContext, clientRectangle.Left, clientRectangle.Top, hBitmap);
        //     DrawIcon(hDeviceContext, clientRectangle.Left, clientRectangle.Top, hIcon4);
        //     //DrawText(hDeviceContext, pText, &clientRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);
        //     //DrawText(hDeviceContext, &textBuffer[0], &clientRectangle, DRAW_TEXT_FLAG_ALIGN_HORIZONTAL_CENTER | DRAW_TEXT_FLAG_ALIGN_VERTICAL_CENTER);

        //     DrawEnd(hDeviceContext);
        // }

        // INT32 iconLeft = 30;
        // INT32 iconTop = 10;

        // //for(UINT32 i = 0; i < 1000; i++)
        // {
        //     iconLeft += deltaX;

        //     if (iconLeft < 2 || iconLeft > 262 - 64)
        //     {
        //         deltaX *= -1;
        //     }

            

        //     //DrawBegin(hDeviceContext);
        //     //DrawFilledRectangle(hDeviceContext, iconLeft - 1, iconTop - 1, iconLeft + 64 + 1, iconTop + 64 +1);
        //     //DrawIcon(hDeviceContext, iconLeft, iconTop, hIcon1);
        //     //DrawBitmap(hDeviceContext, iconLeft, iconTop, hBitmap);
        //     //DrawEnd(hDeviceContext);
        // }

//DrawBegin(hDeviceContext);
//DrawEnd(hDeviceContext);

        DrawBegin(hDeviceContext);
        SetTextColor(hDeviceContext, 0xFF00808F);
        GetClientRectangle(hWindow, &clientRectangle);
        //DrawFilledRectangle(hDeviceContext, clientLeft, clientTop, clientRight, clientBottom);

// DrawIcon(hDeviceContext, 0, 0, hIcon1);
// DrawIcon(hDeviceContext, 32, 0, hIcon2);
// DrawIcon(hDeviceContext, 64, 0, hIcon3);
// DrawIcon(hDeviceContext, 96, 0, hIcon4);
//DrawIcon(hDeviceContext, 128, 0, hIcon5);


        
        DrawPalette(hDeviceContext, 0, 0);

        DrawEnd(hDeviceContext);
        
        ReleaseDeviceContext(hWindow);

        //IsSSWindowReady = TRUE;
    }
    // else if (message == WINDOW_MESSAGE_MOUSE_ENTER)
    // {
    //     HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
    //     UINT32 textHeight = GetTextHeight(hDeviceContext, (PCHAR) &"Ok") + 2;

    //     RECTANGLE windowRectangle;
    //     GetClientRectangle(hWindow, &windowRectangle);

    //     DrawBegin(hDeviceContext);

    //     SetForegroundColor(hDeviceContext, 0xFF00FF00);
    //     DrawRectangle(hDeviceContext, windowRectangle.Left, windowRectangle.Top, windowRectangle.Right, windowRectangle.Bottom);

    //     DrawEnd(hDeviceContext);
    // }

    if (message == WINDOW_MESSAGE_COMMAND)
    {
        UINT32 commandType = param1;
        HWINDOW    hButton = param2;

        if (commandType == COMMAND_BUTTON_CLICKED)
        {
            if (hButton == hTestButton1)
            {
                //SendMessage(hWindow, WINDOW_MESSAGE_DEACTIVATE, 0, 0);
                // SetVideoMode(GetPrimaryDisplayDevice(), 0);
                // SendMessage(hDesktopWindow, WINDOW_MESSAGE_PAINT, 0, 0);
                // SendMessage(hWindow, WINDOW_MESSAGE_PAINT_NON_CLIENT, 0, 0);
                // SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

                return;
            }

            if (hButton == hTestButton2)
            {
                //SendMessage(hWindow, WINDOW_MESSAGE_DEACTIVATE, 0, 0);
                // SetVideoMode(GetPrimaryDisplayDevice(), 1);
                // SendMessage(hDesktopWindow, WINDOW_MESSAGE_PAINT, 0, 0);
                // SendMessage(hWindow, WINDOW_MESSAGE_PAINT_NON_CLIENT, 0, 0);
                // SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

                return;
            }
        }
    }
    
    DefaultWindowProcedure(hWindow, message, param1, param2);
}