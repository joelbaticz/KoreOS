#include <kldr.h>

static CHAR MwWindowClass[]      = "MYSTIFY";
static CHAR MwWindowTitle[]      = "Mystify";

// Without static it's trash =()
static INT32  mx0[25];
static INT32  my0[25];
static INT32  mx1[25];
static INT32  my1[25];
static INT32 mdx0[25];
static INT32 mdy0[25];
static INT32 mdx1[25];
static INT32 mdy1[25];
static COLOR mcol[25];
static UINT32 lineCount;
static UINT32 maxLineCount;

UINT32 MystifyWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);

VOID RegisterMystifyWindow()
{
    IsMystifyWindowCreated = 0;

    RegisterWindow((PCHAR) &MwWindowClass, 0, 0, 0, 0, GetColorFromComponents(20, 35, 80), (PVOID) MystifyWindowProcedure, 0);
}

VOID CreateMystifyWindow()
{
    UINT32 windowSizeX = 200;
    UINT32 windowSizeY = 200;

    hMystifyWindow = CreateWindowByClass((PCHAR) &MwWindowClass, (PCHAR) &MwWindowTitle, 120, 20, windowSizeX + 6, windowSizeY + 26, 0, 0, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_HAS_DIALOG_BORDER | WINDOW_FLAG_HAS_CAPTION);

    maxLineCount = 25;

    for(UINT32 i = 0; i < maxLineCount; i++)
    {
        mx0[i]  = 0;
        my0[i]  = 0;
        mx1[i]  = 0;
        my1[i]  = 0;
        mdx0[i] = 0;
        mdy0[i] = 0;
        mdx1[i] = 0;
        mdy1[i] = 0; 
    }

    mx0[0]  = 50;
    my0[0]  = 50;
    mx1[0]  = 100;
    my1[0]  = 100;
    mdx0[0] = 2;
    mdy0[0] = -3;
    mdx1[0] = -1;
    mdy1[0] = 3;    

    for(UINT32 i = 0; i < maxLineCount; i++)
    {
        //mcol[i] = GetColorFromComponents(255 / (maxLineCount - i + 1), 0 / (maxLineCount - i + 1), 255 / (maxLineCount - i + 1));
        //mcol[i] = GetColorFromComponents(255, 0, 255);

        UINT32 d = 255 * i / maxLineCount;
        mcol[i] = GetColorFromComponents(0, 0, 255 - d);

        if (i == maxLineCount - 1)
        {
            mcol[i] = GetColorFromComponents(0, 0, 0);
        }
    }    

    lineCount = 1;

    IsMystifyWindowCreated = 0xBAADB00B;
}

UINT32 MystifyWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    if (message == WINDOW_MESSAGE_PAINT)
    {
        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);

        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);

        SetTextColor(hDeviceContext, 0xFF00808F);
        SetBackgroundColor(hDeviceContext, 0xFF000000);
        GetClientRectangle(hWindow, &clientRectangle);

        DrawBegin(hDeviceContext);

        DrawFilledRectangleC(hDeviceContext, &clientRectangle);

        DrawEnd(hDeviceContext);
        
        ReleaseDeviceContext(hWindow);

        return;
    }

    if (message == WINDOW_MESSAGE_ON_TIMER)
    {
        // if (IsMystifyWindowCreated == 0xBAADB00B)
        // {
        //     SendMessage(hMystifyWindow, WINDOW_MESSAGE_PAINT, 0, 0);
        // }
        
        // return;

        if (GetCapture() != (HWINDOW) NULL)
        {
            return;
        }

        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);

        // Update

        if (lineCount > 1)
        {
            for(UINT32 i = lineCount - 1; i >= 1; i--)
            {
                mx0[i] = mx0[i - 1];
                my0[i] = my0[i - 1];
                mx1[i] = mx1[i - 1];
                my1[i] = my1[i - 1];

                mdx0[i] = mdx0[i - 1];
                mdy0[i] = mdy0[i - 1];
                mdx1[i] = mdx1[i - 1];
                mdy1[i] = mdy1[i - 1];
            }

        }

        // Old line becomes new line
            if (lineCount < maxLineCount)
            {
                lineCount++;
            }     

        {
            mx0[0] += mdx0[0];
            my0[0] += mdy0[0];
            mx1[0] += mdx1[0];
            my1[0] += mdy1[0];

            if (mx0[0] < 1 || mx0[0] > clientRectangle.Right - 1)
            {
                mdx0[0] = 0 - mdx0[0];
                mx0[0] += mdx0[0];
            }

            if (my0[0] < 1 || my0[0] > clientRectangle.Bottom - 1)
            {
                mdy0[0] = 0 - mdy0[0];
                my0[0] += mdy0[0];
            }

            if (mx1[0] < 1 || mx1[0] > clientRectangle.Right - 1)
            {
                mdx1[0] = 0 - mdx1[0];
                mx1[0] += mdx1[0];
            }

            if (my1[0] < 1 || my1[0] > clientRectangle.Bottom - 1)
            {
                mdy1[0] = 0 - mdy1[0];
                my1[0] += mdy1[0];
            }
        }

        // Redraw


        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);

        SetTextColor(hDeviceContext, 0xFF00808F);
        SetBackgroundColor(hDeviceContext, 0xFF000000);
        SetForegroundColor(hDeviceContext, 0xFFFF00FF);
        GetClientRectangle(hWindow, &clientRectangle);

        DrawBegin(hDeviceContext);

        //DrawFilledRectangleC(hDeviceContext, &clientRectangle);

        for(UINT32 i = 0; i < lineCount; i++)
        {
            SetForegroundColor(hDeviceContext, mcol[i]);
            DrawLineA(hDeviceContext, mx0[i], my0[i], mx1[i], my1[i]);
        }

        //SetForegroundColor(hDeviceContext, 0xFF000000);
        //DrawLineA(hDeviceContext, mx0[lineCount - 1], my0[lineCount - 1], mx1[lineCount - 1], my1[lineCount - 1]);

        DrawEnd(hDeviceContext);
        
        ReleaseDeviceContext(hWindow);

        return;
    }

    DefaultWindowProcedure(hWindow, message, param1, param2);
}