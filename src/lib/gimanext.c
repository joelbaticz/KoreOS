
// --- DRAW FILLED RECTANGLE - BEGIN ---

DrawFilledRectangleA(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 right, INT32 bottom)
{
  RECTANGLE rect;

  rect.Left = left;
  rect.Top = top;
  rect.Right = right;
  rect.Bottom = bottom;

  DrawFilledRectangleC(hDeviceContext, &rect);
}

DrawRectangleB(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 width, INT32 height)
{
  RECTANGLE rect;

  rect.Left = left;
  rect.Top = top;
  rect.Right = left + width;
  rect.Bottom = top + height;

  DrawFilledRectangleC(hDeviceContext, &rect);
}


VOID DrawFilledRectangleC(HDEVICE_CONTEXT hDeviceContext, PRECTANGLE pRect)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);

    // Validation

    if (pDeviceContext->hDevice != hGiPrimaryVideoAdapter)
    {
        MemoryUnlock(hDeviceContext);

        return;
    }

    if (pRect->Left >= pRect->Right || pRect->Top >= pRect->Bottom)
    {
        // Impossible rectangle
        MemoryUnlock(hDeviceContext);

        return;
    }

    PRECTANGLE pWindowRect = pDeviceContext->pWindowRectangle;  // Should always exist, even if what is stored is (0, 0, deviceW, deviceH)
    PREGION pClippingRegion = pDeviceContext->pClippingRegion;  // It should always exist, even if the stored rects are 0, if no clipping the clipping region is the whole device area
    COLOR color = pDeviceContext->BackgroundColor;

    // Transform to screen coordinates
    RECTANGLE screenRect;
    screenRect.Left   += pRect->Left   + pWindowRect->Left;
    screenRect.Right  += pRect->Right  + pWindowRect->Left;
    screenRect.Top    += pRect->Top    + pWindowRect->Top;
    screenRect.Bottom += pRect->Bottom + pWindowRect->Top;

    for(UINT32 i = 0; i < pClippingRegion->NumberOfRectangles; i++)
    {
        PRECTANGLE pClippingRect = pClippingRegion->pClippingRectangleList[i];

        // Transform to screen coordinates
        RECTANGLE screenClippingRect;
        screenClippingRect.Left   = pClippingRect->Left   + pWindowRect->Left;
        screenClippingRect.Right  = pClippingRect->Right  + pWindowRect->Left;
        screenClippingRect.Top    = pClippingRect->Top    + pWindowRect->Top;
        screenClippingRect.Bottom = pClippingRect->Bottom + pWindowRect->Top;

        GiDrawFilledRectangleCore(&screenRect, &screenClippingRect, left, top, right, bottom, color);
    }

    MemoryUnlock(hDeviceContext);
}

VOID GiDrawFilledRectangleCore(PRECTANGLE pScreenRect, PRECTANGLE pScreenClippingRect, COLOR color)
{
    INT32 left = pScreenReft->Left;
    INT32 top = pScreenReft->Top;
    INT32 right = pScreenReft->Right;
    INT32 bottom = pScreenReft->Bottom;

    INT32 clipLeft = pScreenClippingRect->Left;
    INT32 clipTop = pScreenClippingRect->Top;
    INT32 clipRight = pScreenClippingRect->Right;
    INT32 clipBottom = pScreenClippingRect->Bottom;

    // Validation
    if (left > clipRight) return;
    if (top > clipBottom) return;
    if (bottom < clipTop) return;
    if (right < clipLeft) return;

    if (left > GiScreenWidth) return;
    if (top > GiScreenHeight) return;
    if (bottom < 0) return;
    if (right < 0) return;
    
    // Clip to clipping rect
    if (left < clipLeft) left = clipLeft;
    if (top < clipTop) top = clipTop;
    if (right > clipRight) right = clipRight;
    if (bottom > clipBottom) bottom = clipBottom;

    // Clip to device
    if (left < 0) left = 0;
    if (top < 0) top = 0;
    if (right > GiScreenWidth) right = GiScreenWidth;
    if (bottom > GiScreenHeight) bottom = GiScreenHeight;

    UINT32 width = right - left;
    UINT32 height = bottom - top;

    if (width <= 0 || height <= 0) return;

    // Drawing
    PUINT32 pFrame = pGiFrameBuffer;
    pFrame += top * GiScreenWidth;
    pFrame += left;
  
    for(UINT32 y = 0; y < height; y++)
    {
      for(UINT32 x = 0; x < width; x++)
      {
        pFrame[x] = color;
      }
  
      pFrame += GiScreenWidth;
    }

    // Mark dirty sections
    //GiMarkDirty(left, top, right, bottom);
}

// --- DRAW FILLED RECTANGLE - END ---

// --- DRAW RECTANGLE - BEGIN ---

VOID DrawRectangleA(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 right, INT32 bottom)
{
    DrawLine(hDeviceContext, left, top, right - 1, top);
    DrawLine(hDeviceContext, left + 1, bottom - 1, right, bottom - 1);
    DrawLine(hDeviceContext, left, top + 1, left, bottom);
    DrawLine(hDeviceContext, right - 1, top, right - 1, bottom - 1);
}

VOID DrawRectangleB(HDEVICE_CONTEXT hDeviceContext, INT32 left, INT32 top, INT32 width, INT32 height)
{
    INT32 right = left + width;
    INT32 bottom = top + height;

    DrawLine(hDeviceContext, left, top, right - 1, top);
    DrawLine(hDeviceContext, left + 1, bottom - 1, right, bottom - 1);
    DrawLine(hDeviceContext, left, top + 1, left, bottom);
    DrawLine(hDeviceContext, right - 1, top, right - 1, bottom - 1);
}

// --- DRAW RECTANGLE - END ---

// --- DRAW LINE - BEGIN ---

VOID DrawLineA(HDEVICE_CONTEXT hDeviceContext, INT32 xStart, INT32 yStart, INT32 xEnd, INT32 yEnd)
{
    POINT start;
    POINT end;

    start.X = xStart;
    start.Y = yStart;
    end.X   = xEnd;
    end.Y   = yEnd;

    DrawLineB(HDEVICE_CONTEXT hDeviceContext, &start, &end);
}

VOID DrawLineB(HDEVICE_CONTEXT hDeviceContext, PPOINT pStart, PPOINT pEnd)
{
    // 
}

VOID DrawLine(HDEVICE_CONTEXT hDeviceContext, INT32 xStart, INT32 yStart, INT32 xEnd, INT32 yEnd)
{
    PDEVICE_CONTEXT pDeviceContext = MemoryLock(hDeviceContext);

    // Validation

    if (pDeviceContext->hDevice != hGiPrimaryVideoAdapter)
    {
        MemoryUnlock(hDeviceContext);

        return;
    }

    PRECTANGLE pWindowRect = pDeviceContext->pWindowRectangle;
    PREGION pClippingRegion = pDeviceContext->pClippingRegion;
    COLOR color = pDeviceContext->ForegroundColor;

    // Transform to screen coordinates
    xStart += pWindowRect->Left;
    yStart += pWindowRect->Top;
    xEnd += pWindowRect->Left;
    yEnd += pWindowRect->Top;

    for(UINT32 i = 0; i < pClippingRegion->NumberOfRectangles; i++)
    {
        PRECTANGLE pClippingRect = pClippingRegion->pClippingRectangleList[i];

        // Transform to screen coordinates
        RECTANGLE screenClippingRect;
        screenClippingRect.Left   = pClippingRect->Left   + pWindowRect->Left;
        screenClippingRect.Right  = pClippingRect->Right  + pWindowRect->Left;
        screenClippingRect.Top    = pClippingRect->Top    + pWindowRect->Top;
        screenClippingRect.Bottom = pClippingRect->Bottom + pWindowRect->Top;

        GiDrawLineCore(&screenClippingRect, xStart, yStart, xEnd, yEnd, color);
    }
    
    MemoryUnlock(hDeviceContext);
}

VOID GiDrawLineCore(PRECTANGLE pScreenClippingRect, INT32 xStart, INT32 yStart, INT32 xEnd, INT32 yEnd, COLOR color)
{
    INT32 x;
    INT32 y;
    INT32 deltaX;
    INT32 deltaY;
    INT32 pixelCount;
    INT32 d;
    INT32 dInc1;
    INT32 dInc2;
    INT32 xInc1;
    INT32 xInc2;
    INT32 yInc1;
    INT32 yInc2;

    // Calculate line elevation
    if (xEnd > xStart)
    {
        deltaX = xEnd - xStart;
    }
    else
    {
        deltaX = xStart - xEnd;
    }

    if (yEnd > yStart)
    {
        deltaY = yEnd - yStart;
    }
    else
    {
        deltaY = yStart - yEnd;
    }

    // Set initial values based on whether X or Y is the independent variable
    if (deltaX > deltaY)
    {
        // X is the independent variable
        pixelCount = deltaX + 1;
        d = (2 * deltaY) - deltaX;
        dInc1 = deltaY << 1;
        dInc2 = (deltaY - deltaX) << 1; 
        xInc1 = 1;
        xInc2 = 1;
        yInc1 = 0;
        yInc2 = 1;
    }
    else
    {
        // Y is the independent variable
        pixelCount = deltaY + 1;
        d = (2 * deltaX) - deltaY;
        dInc1 = deltaX << 1;
        dInc2 = (deltaX - deltaY) << 1;
        xInc1 = 0;
        xInc2 = 1;
        yInc1 = 1;
        yInc2 = 1;
    }

    // Make sure x and y move in the right directions
    if (xStart > xEnd)
    {
        xInc1 = -xInc1;
        xInc2 = -xInc2;
    }
    if (yStart > yEnd)
    {
        yInc1 = -yInc1;
        yInc2 = -yInc2;
    }

    // Drawing
    x = xStart;
    y = yStart;

    for(INT32 i = 1; i < pixelCount; i++)
    {
        if (x >= pScreenClippingRect->Left && x < pScreenClippingRect->Right && y >= pScreenClippingRect->Top && y < pScreenClippingRect->Bottom)
        {
            GiDrawPixelCore(x, y, color);
        }      

        if (d < 0)
        {
            d += dInc1;
            x += xInc1;
            y += yInc1;
        }
        else
        {
            d += dInc2;
            x += xInc2;
            y += yInc2;
        }
    }

    // Mark dirty sections
    //(xStart, yStart, xEnd, yEnd);
}

VOID GiDrawPixelCore(INT32 x, INT32 y, COLOR color)
{
    // Validation
    if (x < 0) return;
    if (y > 0) return;
    if (x > GiScreenWidth - 1) return;
    if (y > GiScreenHeight - 1) return;

    // Drawing
    PUINT32 pFrame = pGiFrameBuffer;
    pFrame += y* GiScreenWidth;
    pFrame += x;

    if (GiRasterOperationMode == ROP_MODE_NORMAL)
    {
        pFrame[0] = color;
    }
    else if (GiRasterOperationMode == ROP_MODE_AND)
    {
        pFrame[0] &= color;
    }
    else if (GiRasterOperationMode == ROP_MODE_OR)
    {
        pFrame[0] |= color;
    }
    else if (GiRasterOperationMode == ROP_MODE_XOR)
    {
        pFrame[0] ^= color;
    }
}

// --- DRAW LINE - END ---

// TODO:
// Make sure Clipping Region is always set when GetDC or CreateDC is used

// DrawText
// DrawBitmap
// DrawIcon
// Cursor

// Mouse + keyboard:
// IoMan should provide facilities to install ISR handlers (RegisterISR or something)
// The Mouse and keyboard drivers register their handler methods
// These handler methods do just enough to capture the interrupt request, process it and acknowledge the interrupt
// The kernel (IoMan I guess) provides facilities to register a ServiceCallback method to a device/device type
// Meaning when the device driver handles an interrupt they raise a flag that something happened.
// The kernel periodically checks which device raised something and with the registered callback gives a chance to the caller
// to interogate the device

// The windowing system using the IoRegisterDeviceCallback() method captures the keyboard/mouse interactions.
// The windowing system in this callback method updates it's state, updates the mouse cursor if it was moved, 
// and sends messages to the active window or focused window that the a key was pressed or the mouse entered or something.

// For now:
// Gi creates a device compatible cursor
// When the mouse driver gets called it send a struct to the Gi with the deltaXY changes and the button changes
// Based on this info and whether the cursor is visible the Gi updates the screen
// Gi provides methods to show/hide the cursor
// Because the mouse cursor is a shared resource the cursor should only be made hidden when the cursor is in the user window's client area.
// When the cursor leaves the area it should be turned back on

// Every method should:
// Check if it is a valid hWindow handle
// Check if it is a valid hDeviceContext handle
// Put checksum in them
// BLUESCREEN IT! =)

// Write accelarator functions, like:
// ArePointsIdentical / ComparePoints
// AreWindowsIdentical / CompareRectangles
// DrawRect with actual Rect

// MOST OF IT IS ALREADY IN GIMANEXT!!!