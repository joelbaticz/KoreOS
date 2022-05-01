#include <kldr.h>

UINT32 GiTestNumberOfDevices;
DISPLAY_DEVICE displayDevice;
DEVICE_CAPABILITIES deviceCapabilities;
VIDEO_MODE_INFORMATION videoModeInformation;
HDEVICE hDevice;
HDEVICE_CONTEXT hDeviceContext;
UINT32 result;

VOID GiTestShowVariableNameAndValue(PCHAR pVariableName, UINT32 variableValue, UINT32 base);
VOID GiTestShowVariableNameAndValue2(HDEVICE_CONTEXT hDeviceContext, PCHAR pVariableName, UINT32 variableValue, UINT32 base);

COLOR GetMidColor(COLOR startColor, COLOR endColor, UINT32 percentage);

VOID DrawBackground(HDEVICE_CONTEXT hDeviceContext);
VOID DrawWindow(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y, INT32 width, INT32 height);

VOID GiTest()
{
    UiDrawText("----------");
    UiDrawText("GetNumberOfDevices();");
    GiTestNumberOfDevices = GetNumberOfDevices();
    GiTestShowVariableNameAndValue("NumberOfDevices", GiTestNumberOfDevices, 10);

    UiDrawText("----------");
    UiDrawText("EnumerateDevices();");
    EnumerateDevices(&displayDevice, sizeof(displayDevice));
    UiDrawText(displayDevice.DeviceName);
    UiDrawText(displayDevice.DeviceDescription);
    GiTestShowVariableNameAndValue("DeviceHandle", displayDevice.hDevice, 16);
    hDevice = displayDevice.hDevice;

    UiDrawText("----------");
    UiDrawText("GetDeviceCapabilities();");
    GetDeviceCapabilities(hDevice, &deviceCapabilities);
    GiTestShowVariableNameAndValue("DeviceTechnology", deviceCapabilities.DeviceTechnology, 16);
    GiTestShowVariableNameAndValue("ResolutionX", deviceCapabilities.ResolutionX, 10);
    GiTestShowVariableNameAndValue("ResolutionY", deviceCapabilities.ResolutionY, 10);
    GiTestShowVariableNameAndValue("NumberOfColors", deviceCapabilities.NumberOfColors, 10);
    GiTestShowVariableNameAndValue("NumberOfVideoModes", deviceCapabilities.NumberOfVideoModes, 10);
    GiTestShowVariableNameAndValue("CurrentVideoModeIndex", deviceCapabilities.CurrentVideoModeIndex, 16);
    GiTestShowVariableNameAndValue("RasterCapabilities", deviceCapabilities.RasterCapabilities, 16);

    UiDrawText("----------");
    UiDrawText("GetVideoModeInformation();");
    GetVideoModeInformation(hDevice, 0, &videoModeInformation);
    GiTestShowVariableNameAndValue("ModeNumber", videoModeInformation.ModeNumber, 10);
    GiTestShowVariableNameAndValue("Width", videoModeInformation.Width, 10);
    GiTestShowVariableNameAndValue("Height", videoModeInformation.Height, 10);
    GiTestShowVariableNameAndValue("WidthInBytes", videoModeInformation.WidthInBytes, 10);
    GiTestShowVariableNameAndValue("NumberOfBitsPerPixel", videoModeInformation.NumberOfBitsPerPixel, 10);
    GiTestShowVariableNameAndValue("NumberOfColors", videoModeInformation.NumberOfColors, 10);
    GiTestShowVariableNameAndValue("NumberOfPlanes", videoModeInformation.NumberOfPlanes, 10);
    GiTestShowVariableNameAndValue("NumberOfRedBits", videoModeInformation.NumberOfRedBits, 10);
    GiTestShowVariableNameAndValue("NumberOfGreenBits", videoModeInformation.NumberOfGreenBits, 10);
    GiTestShowVariableNameAndValue("NumberOfBlueBits", videoModeInformation.NumberOfBlueBits, 10);
    GiTestShowVariableNameAndValue("RedMask", videoModeInformation.RedMask, 16);
    GiTestShowVariableNameAndValue("GreenMask", videoModeInformation.GreenMask, 16);
    GiTestShowVariableNameAndValue("BlueMask", videoModeInformation.BlueMask, 16);
    GiTestShowVariableNameAndValue("BitsPerPlane", videoModeInformation.BitsPerPlane, 10);
    GiTestShowVariableNameAndValue("Attributes", videoModeInformation.Attributes, 16);
    GiTestShowVariableNameAndValue("Frequency", videoModeInformation.Frequency, 10);

    UiDrawText("----------");
    UiDrawText("SetVideoMode();");
    SetVideoMode(hDevice, 0);

    UiDrawText("----------");
    UiDrawText("CreateDeviceContext();");
    hDeviceContext = CreateDeviceContext(hDevice);
    GiTestShowVariableNameAndValue("hDeviceContext", hDeviceContext, 16);


    // UINT32 textWidth = GetTextWidth(hDeviceContext, (PCHAR) &"W", 0);
    // GiTestShowVariableNameAndValue("textWidth", textWidth, 10);

    // INT32 textHeight = GetTextHeight(hDeviceContext, (PCHAR) &"W");
    // GiTestShowVariableNameAndValue("textHeight", textHeight, 10);

    // HALT;



    // Test drawing
    DrawBegin(hDeviceContext);
    DrawFilledRectangleA(hDeviceContext, 24, 24, 28, 28);
    DrawFilledRectangleA(hDeviceContext, 224, 74, 228, 78);
    DrawFilledRectangleA(hDeviceContext, 126, 126, 128, 128);

    DrawEnd(hDeviceContext);

    DrawBegin(hDeviceContext);

    // for(UINT32 i = 0; i < 200; i++)
    // {
    //     DrawFilledRectangle(hDeviceContext, i, i, i + 1, i + 1);
    // }

    // PDEVICE_CONTEXT pDC = MemoryLock(hDeviceContext);

    // COLOR startColor = 0x1F66DA;
    // COLOR endColor = 0x28A7FF;

    // UINT32 length = 200;
    // for(UINT32 i = 0; i < length; i++)
    // {
    //     pDC->BackgroundColor = GetMidColor(startColor, endColor, i * 100 / length);
    //     DrawFilledRectangle(hDeviceContext, 10 + i, 50, 10 + i + 1, 70);
    // }

    DrawBackground(hDeviceContext);
    DrawWindow(hDeviceContext, 20, 30, 100, 100);

    DrawWindow(hDeviceContext, 35, 50, 250, 110);

    GiTestShowVariableNameAndValue2(hDeviceContext, "Test", 10, 10);

    DrawEnd(hDeviceContext);

    //UiDrawText("----------");
    //UiDrawText("DeleteDeviceContext();");
    //result = DeleteDeviceContext(hDeviceContext);
    //GiTestShowVariableNameAndValue("Status", result, 16);
}

COLOR GetMidColor(COLOR startColor, COLOR endColor, UINT32 percentage)
{
    INT32 red = startColor >> 16 & 0xFF;
    INT32 green = startColor  >> 8 & 0xFF;
    INT32 blue = startColor & 0xFF;

    INT32 red2 = endColor >> 16 & 0xFF;
    INT32 green2 = endColor >> 8 & 0xFF;
    INT32 blue2 = endColor & 0xFF;

    INT32 dRed = (red2 - red) * percentage / 100;
    INT32 dGreen = (green2 - green) * percentage / 100;
    INT32 dBlue = (blue2 - blue) * percentage / 100;

    COLOR newColor = startColor + (dRed << 16) + (dGreen << 8) + dBlue;

    return newColor;
}

VOID GiTestShowVariableNameAndValue(PCHAR pVariableName, UINT32 variableValue, UINT32 base)
{
    CHAR textBuffer[40] = { 0 };
    size_t textLength;

    textLength = strlen((const char*)pVariableName);
    strcpy(&textBuffer[0], (const char*)pVariableName);
    textBuffer[textLength] = '=';
    itoa(variableValue, &textBuffer[textLength + 1], base);

    UiDrawText(&textBuffer[0]);
}

VOID GiTestShowVariableNameAndValue2(HDEVICE_CONTEXT hDeviceContext, PCHAR pVariableName, UINT32 variableValue, UINT32 base)
{
    CHAR textBuffer[40] = { 0 };
    size_t textLength;

    textLength = strlen((const char*)pVariableName);
    strcpy(&textBuffer[0], (const char*)pVariableName);
    textBuffer[textLength] = '=';
    itoa(variableValue, &textBuffer[textLength + 1], base);

    DrawText(hDeviceContext, &textBuffer[0], 0, 0);
}

VOID DrawBackground(HDEVICE_CONTEXT hDeviceContext)
{
    PDEVICE_CONTEXT pDC = MemoryLock(hDeviceContext);

    pDC->BackgroundColor = GetColorFromComponents(20, 35, 80);
    DrawFilledRectangleA(hDeviceContext, 0, 0, 320, 200);

    RECTANGLE rectangle;

    PCHAR buildText = (PCHAR) &"KoreOS Build 1204";

    rectangle.Left = 320 - GetTextWidth(hDeviceContext, buildText, 0);
    rectangle.Top = 200 - GetTextHeight(hDeviceContext, buildText);
    rectangle.Right = 320;
    rectangle.Bottom = 200;

    DrawText(hDeviceContext, buildText, &rectangle, 0);

    MemoryUnlock(hDeviceContext);
}

VOID DrawWindow(HDEVICE_CONTEXT hDeviceContext, INT32 x, INT32 y, INT32 width, INT32 height)
{
    PDEVICE_CONTEXT pDC = MemoryLock(hDeviceContext);

    pDC->pWindowRectangle->Left = x;
    pDC->pWindowRectangle->Top = y;

    pDC->BackgroundColor = GetColorFromComponents(66, 170, 231);
    DrawFilledRectangleA(hDeviceContext, 0, 0, width, height);

    pDC->BackgroundColor = GetColorFromComponents(255, 255, 255);
    DrawFilledRectangleA(hDeviceContext, 5, 22, width - 5, height - 5);

    pDC->ForegroundColor =  GetColorFromComponents(28, 139, 215);
    DrawRectangleA(hDeviceContext, 0, 0, width, height);
    DrawRectangleA(hDeviceContext, 4, 21, width - 4, height - 4);

    MemoryUnlock(hDeviceContext);
}

