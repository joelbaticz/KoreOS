/*
  - Date: 2021-08-28
  - Author: Joel
  - Purpose: VGA video driver
*/

#include <kldr.h>

#define VGA_NUMBER_OF_MODES             4
#define VGA_VIDEO_MODE_320x200x256      0
#define VGA_VIDEO_MODE_640x480x2        1
#define VGA_VIDEO_MODE_640x480x16       2
#define VGA_VIDEO_MODE_320x240x256      3
#define VGA_FRAME_BUFFER_ADDRESS        0xA0000

PDRIVER_OBJECT pVgaDriver;
PDEVICE_OBJECT pVgaDevice;
PDRIVER_DISPATCH_FUNCTION pVgaDriverDispatchFunction;
UINT32 currentVideoMode;

VIDEO_MODE_INFORMATION VgaVideoModeInfo[VGA_NUMBER_OF_MODES];

BOOL    VgaMouseShown;
POINT   VgaMousePosition;
POINT   VgaMousePositionOld;
UINT32  VgaMouseBackground[32 * 32];
HCURSOR hVgaMouseCursor;

VOID VgaSetVideoModeInformation();
VOID VgaSetDriverFunction();
VOID VgaDispatchPassThru(PDEVICE_OBJECT pDeviceObject, PIRP pIRP);
VOID VgaDispatchDeviceControl(PDEVICE_OBJECT pDeviceObject, PIRP pIRP);

VOID VgaInitialize(PIRP pIRP);
VOID VgaResetDevice(PIRP pIRP);
VOID VgaGetNumberOfVideoModes(PIRP pIRP);
VOID VgaGetVideoModeInformation(PIRP pIRP);
VOID VgaGetFrameBufferAddress(PIRP pIRP);
VOID VgaSetCurrentMode(PIRP pIRP);
VOID VgaSetPalette(PIRP pIRP);
VOID VgaGetDeviceFunctions(PIRP pIRP);

VOID VgaShowVariableNameAndValue(PCHAR pVariableName, UINT32 variableValue, UINT32 base);

// DRAWING FUNCTIONS

// 2 COLORS
VOID VgaDrawDevicePixelOverwrite1Bit(UINT32 screenX, UINT32 screenY, COLOR color);
VOID VgaDrawDevicePixelXor1Bit(UINT32 screenX, UINT32 screenY, COLOR color);
VOID VgaDrawDeviceFilledRectangle1Bit(UINT32 left, UINT32 top, UINT32 right, UINT32  bottom, COLOR color);
VOID VgaShowMouse1Bit();
VOID VgaHideMouse1Bit();
VOID VgaMoveMouse1Bit(PPOINT pMousePosition);
VOID VgaSaveMouseArea1Bit(POINT mousePosition);
VOID VgaRestoreMouseArea1Bit(POINT mousePosition);

// 16 COLORS
VOID VgaDrawDevicePixelOverwrite4Bit(UINT32 screenX, UINT32 screenY, COLOR color);
VOID VgaDrawDevicePixelXor4Bit(UINT32 screenX, UINT32 screenY, COLOR color);
VOID VgaDrawDeviceFilledRectangle4Bit(UINT32 left, UINT32 top, UINT32 right, UINT32  bottom, COLOR color);
VOID VgaShowMouse4Bit();
VOID VgaHideMouse4Bit();
VOID VgaMoveMouse4Bit(PPOINT pMousePosition);
VOID VgaSaveMouseArea4Bit(POINT mousePosition);
VOID VgaRestoreMouseArea4Bit(POINT mousePosition);

// 256 COLORS
VOID VgaDrawDevicePixelOverwrite8Bit(UINT32 screenX, UINT32 screenY, COLOR color);
VOID VgaDrawDevicePixelXor8Bit(UINT32 screenX, UINT32 screenY, COLOR color);
VOID VgaDrawDeviceFilledRectangle8Bit(UINT32 left, UINT32 top, UINT32 right, UINT32  bottom, COLOR color);
VOID VgaShowMouse8Bit();
VOID VgaHideMouse8Bit();
VOID VgaMoveMouse8Bit(PPOINT pMousePosition);
VOID VgaSaveMouseArea8Bit(POINT mousePosition);
VOID VgaRestoreMouseArea8Bit(POINT mousePosition);

// 256 COLORS - MODEX
VOID VgaDrawDevicePixelOverwrite8BitModeX(UINT32 screenX, UINT32 screenY, COLOR color);
VOID VgaDrawDevicePixelXor8BitModeX(UINT32 screenX, UINT32 screenY, COLOR color);
VOID VgaDrawDeviceFilledRectangle8BitModeX(UINT32 left, UINT32 top, UINT32 right, UINT32  bottom, COLOR color);
VOID VgaShowMouse8BitModeX();
VOID VgaHideMouse8BitModeX();
VOID VgaMoveMouse8BitModeX(PPOINT pMousePosition);
VOID VgaSaveMouseArea8BitModeX(POINT mousePosition);
VOID VgaRestoreMouseArea8BitModeX(POINT mousePosition);

// Common functions
VOID VgaSetMousePosition(PPOINT pMousePosition);
VOID VgaGetMousePosition(OUT PPOINT pMousePosition);
VOID VgaSetMouseCursor(HCURSOR hCursor);
HCURSOR VgaGetMouseCursor();

// Other
VOID VgaDrawCrap();
VOID VgaSetCurrentModeWithoutBios(PIRP pIRP);
VOID VgaSetPaletteWithoutBios(PIRP pIRP);

void write_regs(unsigned char *regs);

VOID VgaDriverInit()
{
    pVgaDriver = NULL;
    pVgaDevice = NULL;

    currentVideoMode = 0xFFFFFFFF;

    VgaSetVideoModeInformation();

    KSTATUS result = DriverCreate(
                        (PCHAR) &"",                                // Fully qualified driver object name
                        (PCHAR) &"Standard VGA Video Driver",       // Driver name/description
                        DRIVER_BOOT | DRIVER_FALL_BACK,             // Flags
                        NULL,                                       // Driver Init function
                        NULL,                                       // Driver Unload function
                        &VgaDispatchDeviceControl,                  // Driver Dispatch funcion
                        OUT &pVgaDriver);

    if (result != STATUS_SUCCESS | pVgaDriver == NULL)
    {
        ERROR("Could not initialize VGA driver  (DriverCreate failed)!");
        return;
        HALT;
    }

    result = DeviceCreate(
                (PCHAR) &"\\VideoAdapter0",                         // Fully qualified device object name
                (PCHAR) &"Standard VGA Display Adapter",            // Device name/description
                pVgaDriver,                                         // Pointer to driver object
                DEVICE_VIDEO,                                       // Device type
                DEVICE_CLASS_OUTPUT,                                // Device class
                OUT &pVgaDevice);

    if (result != STATUS_SUCCESS | pVgaDevice == NULL)
    {
        DriverDelete(pVgaDriver);
        ERROR("Could not initialize VGA driver (DeviceCreate failed)!");
        HALT;
    }

    VgaMouseShown         = FALSE;
    VgaMousePosition.X    = 0;
    VgaMousePosition.Y    = 0;
    VgaMousePositionOld.X = 0;
    VgaMousePositionOld.Y = 0;

    hVgaMouseCursor  = (HCURSOR) NULL;

    UiDrawText("VGA Driver...................DONE");
}

VOID VgaSetVideoModeInformation()
{
    VgaVideoModeInfo[0].ModeNumber = 0;
    VgaVideoModeInfo[0].Width = 320;
    VgaVideoModeInfo[0].Height = 200;
    VgaVideoModeInfo[0].WidthInBytes = 320;
    VgaVideoModeInfo[0].NumberOfBitsPerPixel = 8;
    VgaVideoModeInfo[0].NumberOfColors = 256;
    VgaVideoModeInfo[0].NumberOfPlanes = 0;
    VgaVideoModeInfo[0].NumberOfRedBits = 0;
    VgaVideoModeInfo[0].NumberOfGreenBits = 0;
    VgaVideoModeInfo[0].NumberOfBlueBits = 0;
    VgaVideoModeInfo[0].RedMask = 0;
    VgaVideoModeInfo[0].GreenMask = 0;
    VgaVideoModeInfo[0].BlueMask = 0;
    VgaVideoModeInfo[0].BitsPerPlane = 0;
    VgaVideoModeInfo[0].Attributes = VIDEO_MODE_ATTRIBUTE_COLOR | VIDEO_MODE_ATTRIBUTE_GRAPHICS | VIDEO_MODE_ATTRIBUTE_PALETTE_DRIVEN | VIDEO_MODE_ATTRIBUTE_MANAGED_PALETTE;
    VgaVideoModeInfo[0].Frequency = 60;

    VgaVideoModeInfo[1].ModeNumber = 1;
    VgaVideoModeInfo[1].Width = 640;
    VgaVideoModeInfo[1].Height = 480;
    VgaVideoModeInfo[1].WidthInBytes = 80;
    VgaVideoModeInfo[1].NumberOfBitsPerPixel = 1;
    VgaVideoModeInfo[1].NumberOfColors = 2;
    VgaVideoModeInfo[1].NumberOfPlanes = 1;
    VgaVideoModeInfo[1].NumberOfRedBits = 0;
    VgaVideoModeInfo[1].NumberOfGreenBits = 0;
    VgaVideoModeInfo[1].NumberOfBlueBits = 0;
    VgaVideoModeInfo[1].RedMask = 0;
    VgaVideoModeInfo[1].GreenMask = 0;
    VgaVideoModeInfo[1].BlueMask = 0;
    VgaVideoModeInfo[1].BitsPerPlane = 8;
    VgaVideoModeInfo[1].Attributes = VIDEO_MODE_ATTRIBUTE_COLOR | VIDEO_MODE_ATTRIBUTE_GRAPHICS | VIDEO_MODE_ATTRIBUTE_PALETTE_DRIVEN | VIDEO_MODE_ATTRIBUTE_MANAGED_PALETTE;
    VgaVideoModeInfo[1].Frequency = 60;

    VgaVideoModeInfo[2].ModeNumber = 2;
    VgaVideoModeInfo[2].Width = 640;
    VgaVideoModeInfo[2].Height = 480;
    VgaVideoModeInfo[2].WidthInBytes = 80;
    VgaVideoModeInfo[2].NumberOfBitsPerPixel = 1;
    VgaVideoModeInfo[2].NumberOfColors = 16;
    VgaVideoModeInfo[2].NumberOfPlanes = 4;
    VgaVideoModeInfo[2].NumberOfRedBits = 0;
    VgaVideoModeInfo[2].NumberOfGreenBits = 0;
    VgaVideoModeInfo[2].NumberOfBlueBits = 0;
    VgaVideoModeInfo[2].RedMask = 0;
    VgaVideoModeInfo[2].GreenMask = 0;
    VgaVideoModeInfo[2].BlueMask = 0;
    VgaVideoModeInfo[2].BitsPerPlane = 8;
    VgaVideoModeInfo[2].Attributes = VIDEO_MODE_ATTRIBUTE_COLOR | VIDEO_MODE_ATTRIBUTE_GRAPHICS | VIDEO_MODE_ATTRIBUTE_PALETTE_DRIVEN | VIDEO_MODE_ATTRIBUTE_MANAGED_PALETTE;
    VgaVideoModeInfo[2].Frequency = 60;

    VgaVideoModeInfo[3].ModeNumber = 0;
    VgaVideoModeInfo[3].Width = 320;
    VgaVideoModeInfo[3].Height = 240;
    VgaVideoModeInfo[3].WidthInBytes = 320;
    VgaVideoModeInfo[3].NumberOfBitsPerPixel = 8;
    VgaVideoModeInfo[3].NumberOfColors = 256;
    VgaVideoModeInfo[3].NumberOfPlanes = 0;
    VgaVideoModeInfo[3].NumberOfRedBits = 0;
    VgaVideoModeInfo[3].NumberOfGreenBits = 0;
    VgaVideoModeInfo[3].NumberOfBlueBits = 0;
    VgaVideoModeInfo[3].RedMask = 0;
    VgaVideoModeInfo[3].GreenMask = 0;
    VgaVideoModeInfo[3].BlueMask = 0;
    VgaVideoModeInfo[3].BitsPerPlane = 0;
    VgaVideoModeInfo[3].Attributes = VIDEO_MODE_ATTRIBUTE_COLOR | VIDEO_MODE_ATTRIBUTE_GRAPHICS | VIDEO_MODE_ATTRIBUTE_PALETTE_DRIVEN | VIDEO_MODE_ATTRIBUTE_MANAGED_PALETTE;
    VgaVideoModeInfo[3].Frequency = 60;
}

VOID VgaDispatchDeviceControl(PDEVICE_OBJECT pDeviceObject, PIRP pIRP)
{
    //UiDrawText(pVGADevice->pDeviceName);
    //UiDrawText("VGA DeviceControl function called.");

    switch(pIRP->ioControlCode)
    {
        case IOCTL_VIDEO_INITIALIZE:
            VgaInitialize(pIRP);
            return;
           
        case IOCTL_VIDEO_RESET_DEVICE:
            VgaResetDevice(pIRP);
            return;

        case IOCTL_VIDEO_GET_NUMBER_OF_MODES:
            VgaGetNumberOfVideoModes(pIRP);
            return;

        case IOCTL_VIDEO_GET_MODE_INFORMATION:
            VgaGetVideoModeInformation(pIRP);
            return;

        case IOCTL_VIDEO_GET_FRAMEBUFFER_ADDRESS:
            VgaGetFrameBufferAddress(pIRP);
            return;

        case IOCTL_VIDEO_SET_CURRENT_MODE:
            //VgaSetCurrentMode(pIRP);
            VgaSetCurrentModeWithoutBios(pIRP);

            return;

        case IOCTL_VIDEO_SET_PALETTE:
            //VgaSetPalette(pIRP);
            VgaSetPaletteWithoutBios(pIRP);

            pIRP->ioStatus = IO_STATUS_SUCCESS;

            return;

        case IOCTL_VIDEO_GET_DEVICE_FUNCTIONS:
            VgaGetDeviceFunctions(pIRP);

            return;

        default:
            pIRP->ioStatus = IO_STATUS_ERROR_INVALID_REQUEST;
            return;
    }
}

VOID VgaInitialize(PIRP pIRP)
{
    pIRP->ioStatus = IO_STATUS_SUCCESS;
}

VOID VgaResetDevice(PIRP pIRP)
{
    pIRP->ioStatus = IO_STATUS_SUCCESS;
}

VOID VgaGetNumberOfVideoModes(PIRP pIRP)
{
    // Validation
    if (pIRP->inBufferSize != 0 ||
        pIRP->outBufferSize != sizeof(UINT32))
    {
        pIRP->ioStatus = IO_STATUS_ERROR_INVALID_BUFFER_SIZE;

        return;
    }

    // Operation
    ((PUINT32)(pIRP->pOutBuffer))[0] = VGA_NUMBER_OF_MODES;

    // Signal success
    pIRP->ioStatus = IO_STATUS_SUCCESS;
}

VOID VgaGetVideoModeInformation(PIRP pIRP)
{
    // Validation
    if (pIRP->inBufferSize != sizeof(UINT32) ||
        pIRP->outBufferSize != sizeof(VIDEO_MODE_INFORMATION))
    {
        pIRP->ioStatus = IO_STATUS_ERROR_INVALID_BUFFER_SIZE;

        return;
    }

    UINT32 requestedMode = ((PUINT32)pIRP->pInBuffer)[0];

    if (requestedMode < 0 || requestedMode > VGA_NUMBER_OF_MODES - 1)
    {
        pIRP->ioStatus = IO_STATUS_ERROR_INVALID_PARAMETER;

        return;
    }

    // Operation
    PVIDEO_MODE_INFORMATION pVideoModeInfo = (PVIDEO_MODE_INFORMATION) pIRP->pOutBuffer;

    pVideoModeInfo[0] = VgaVideoModeInfo[requestedMode];

    // Signal success
    pIRP->ioStatus = IO_STATUS_SUCCESS;
}

VOID VgaGetFrameBufferAddress(PIRP pIRP)
{
    // Validation
    if (pIRP->inBufferSize != 0 ||
        pIRP->outBufferSize != sizeof(UINT32))
    {
        UiDrawText("INVALID BUFFER SIZE!");
        pIRP->ioStatus = IO_STATUS_ERROR_INVALID_BUFFER_SIZE;

        return;
    }

    // Operation
    ((PUINT32)(pIRP->pOutBuffer))[0] = VGA_FRAME_BUFFER_ADDRESS;

    // Signal success
    pIRP->ioStatus = IO_STATUS_SUCCESS;
}

VOID VgaSetCurrentMode(PIRP pIRP)
{
    UiDrawText("VgaSetCurrentMode()");    

    // Validation
    if (pIRP->inBufferSize != sizeof(UINT32) ||
        pIRP->outBufferSize != 0)
    {
        UiDrawText("INVALID BUFFER SIZE!");
        pIRP->ioStatus = IO_STATUS_ERROR_INVALID_BUFFER_SIZE;

        return;
    }

    UINT32 requestedMode = ((PUINT32) pIRP->pInBuffer)[0];
    //UINT32 requestedMode = ((PUINT32)pIRP->pInBuffer)[0];
    //UINT32 requestedMode = *pRequestedMode;

    if (requestedMode > VGA_NUMBER_OF_MODES - 1)
    {
        UiDrawText("INVALID REQUEST!");
        pIRP->ioStatus = IO_STATUS_ERROR_INVALID_REQUEST;

        return;
    }

    // Operation
    UINT8 biosModeNumber = 0;

    switch(requestedMode)
    {
        case 0:
            biosModeNumber = 0x13;
            break;

        case 1:
            biosModeNumber = 0x11;
            break;

        case 2:
            biosModeNumber = 0x12;
            break;
    }

    VgaShowVariableNameAndValue("requestedMode", requestedMode, 16);
    VgaShowVariableNameAndValue("biosModeNumber", biosModeNumber, 16);

    PcConsoleSetBiosVideoMode(biosModeNumber);

    UiDrawText("BIOS CALL RETURNED SUCCESSFULLY.");

    VgaDrawCrap();
    //HALT;

    // Signal success
    pIRP->ioStatus = IO_STATUS_SUCCESS;
}

VOID VgaDrawCrap()
{
    PCHAR pFrameBuffer = (PCHAR) 0xA0000;
    UINT32 locationX = 0;
    UINT32 i = locationX;
    COLOR crapColor = 125;

    for(UINT32 y = 0; y < 40; y++)
    {
        for(UINT32 x = i; x < i + 40; x++)
        {
            pFrameBuffer[x] = crapColor;
        }

        pFrameBuffer += 320;
    }

locationX++;

    if (crapColor == 50)
    {
        crapColor = 100;
    }
    else
    {
        crapColor = 50;
    }
}

VOID VgaSetPalette(PIRP pIRP)
{
    // Validation
    if (pIRP->inBufferSize != sizeof(UINT32) ||
        pIRP->outBufferSize != 0)
    {
        UiDrawText("INVALID BUFFER SIZE!");
        pIRP->ioStatus = IO_STATUS_ERROR_INVALID_BUFFER_SIZE;

        return;
    }

    PPALETTE pPalette = (PPALETTE) ((PUINT32) pIRP->pInBuffer)[0];

    VgaShowVariableNameAndValue("VGA:numberOfPaletteEntries", pPalette->NumberOfEntries, 10);

    // Operation

        // With R Do
        //     Begin
        //         AX := $1012  ;          (* Command to put Palette *)
        //         BX := 0      ;          (* Starting Palette Entry *)
        //         CX := 256    ;          (* Number of Entries      *)
        //         ES := Seg(P) ;          (* Address of Palette     *)
        //         DX := Ofs(P) ;          (* Address of Palette     *)
        //     End ;

        // Intr ($10, R) ;                 (* Do the command         *)

    UINT32 numberOfEntries = pPalette->NumberOfEntries;
    UINT32 addressOfPaletteEntries = (UINT32) pPalette->pPaletteEntries;

	REGISTERS regs;
	PREGISTERS pRegs = &regs;
		
	regs.EAX = 0x1012;                                  // Command: set palette
	regs.EBX = 0;                                       // Index of starting palette entry
	regs.ECX = numberOfEntries;                         // Number of entries
	regs.EDX = addressOfPaletteEntries & 0xFFFF;        // Offset of first palette entry (lower 16bits)
	regs.ESI = 0;
	regs.EDI = 0;
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = addressOfPaletteEntries >> 16 & 0xF;      // Segment of first palette entry (higher 4bits)
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
		
	PcBiosCallInterrupt(0x10, pRegs, pRegs);

    // Signal success
    pIRP->ioStatus = IO_STATUS_SUCCESS;
}

VOID VgaShowVariableNameAndValue(PCHAR pVariableName, UINT32 variableValue, UINT32 base)
{
    CHAR textBuffer[40] = { 0 };
    size_t textLength;

    textLength = strlen((const char*)pVariableName);
    strcpy(&textBuffer[0], (const char*)pVariableName);
    textBuffer[textLength] = '=';
    itoa(variableValue, &textBuffer[textLength + 1], base);

    UiDrawText(&textBuffer[0]);
}





// ------------- SET VIDEO WITHOUT BIOS ----------------------

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
				VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

unsigned char g_640x480x16[] =
{
/* MISC */
	0xE3,
/* SEQ */
	0x03, 0x01, 0x08, 0x00, 0x06,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
	0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x01, 0x00, 0x0F, 0x00, 0x00
};

unsigned char g_320x200x256[] =
{
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};

unsigned char g_320x200x256_modex[] =
{
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x06,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF , 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28, 0x00, 0x96, 0xB9, 0xE3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00, 0x00
};

unsigned char g_320x240x256_modex[] =
{
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x06,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0D , 0x3E,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xEA, 0xAC, 0xDF, 0x28, 0x00, 0xE7, 0x06, 0xE3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00, 0x00
};

VOID VgaSetCurrentModeWithoutBios(PIRP pIRP)
{
    UINT32 requestedMode = ((PUINT32) pIRP->pInBuffer)[0];

    if (requestedMode > VGA_NUMBER_OF_MODES - 1)
    {
        UiDrawText("INVALID REQUEST!");
        pIRP->ioStatus = IO_STATUS_ERROR_INVALID_REQUEST;

       return;
    }

    if (requestedMode == VGA_VIDEO_MODE_320x200x256)
    {
        write_regs(g_320x200x256);

        currentVideoMode = VGA_VIDEO_MODE_320x200x256;

        // Signal success
        pIRP->ioStatus = IO_STATUS_SUCCESS;

        return;
    }

    if (requestedMode == VGA_VIDEO_MODE_640x480x2)
    {
        write_regs(g_640x480x16);

        outportb(0x3C4, 0x2);  // Enable Set/Reset Register
        outportb(0x3C5, 15);   // Plane to write

        currentVideoMode = VGA_VIDEO_MODE_640x480x2;

        // Signal success
        pIRP->ioStatus = IO_STATUS_SUCCESS;

        return;
    }

    if (requestedMode == VGA_VIDEO_MODE_640x480x16)
    {
        write_regs(g_640x480x16);

        outportb(0x3CE, 0x5);  // Set Read-Write mode
	    outportb(0x3CF, 0x0);  // Write Mode 0, Read Mode 0

        outportb(0x3C4, 0x2);  // Enable Set/Reset Register
        outportb(0x3C5, 15);   // Plane to write

        currentVideoMode = VGA_VIDEO_MODE_640x480x16;

        // Signal success
        pIRP->ioStatus = IO_STATUS_SUCCESS;

        return;
    }

    if (requestedMode == VGA_VIDEO_MODE_320x240x256)
    {
        write_regs(g_320x240x256_modex);

        currentVideoMode = VGA_VIDEO_MODE_320x240x256;

        // Signal success
        pIRP->ioStatus = IO_STATUS_SUCCESS;

        return;
    }
}

void write_regs(unsigned char *regs)
{
	unsigned i;

/* write MISCELLANEOUS reg */
	outportb(VGA_MISC_WRITE, *regs);
	regs++;
/* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outportb(VGA_SEQ_INDEX, i);
		outportb(VGA_SEQ_DATA, *regs);
		regs++;
	}
/* unlock CRTC registers */
	outportb(VGA_CRTC_INDEX, 0x03);
	outportb(VGA_CRTC_DATA, inportb(VGA_CRTC_DATA) | 0x80);
	outportb(VGA_CRTC_INDEX, 0x11);
	outportb(VGA_CRTC_DATA, inportb(VGA_CRTC_DATA) & ~0x80);
/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
/* write CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outportb(VGA_CRTC_INDEX, i);
		outportb(VGA_CRTC_DATA, *regs);
		regs++;
	}
/* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outportb(VGA_GC_INDEX, i);
		outportb(VGA_GC_DATA, *regs);
		regs++;
	}
/* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inportb(VGA_INSTAT_READ);
		outportb(VGA_AC_INDEX, i);
		outportb(VGA_AC_WRITE, *regs);
		regs++;
	}
/* lock 16-color palette and unblank display */
	(void)inportb(VGA_INSTAT_READ);
	outportb(VGA_AC_INDEX, 0x20);
}

VOID VgaSetPaletteWithoutBios(PIRP pIRP)
{
    PPALETTE pPalette = (PPALETTE) ((PUINT32) pIRP->pInBuffer)[0];

    UINT32 noOfEntries = pPalette->NumberOfEntries;

    PPALETTE_ENTRY pPaletteEntry = pPalette->pPaletteEntries;

    outportb(0x3c8, 0);

    for(UINT32 i = 0; i < noOfEntries; i++)
    {
      UINT8 red = (pPaletteEntry->Red) / 4;
      outportb(0x3c9, red);

      UINT8 green = (pPaletteEntry->Green) / 4;
      outportb(0x3c9, green);

      UINT8 blue = (pPaletteEntry->Blue) / 4;
      outportb(0x3c9, blue);

      pPaletteEntry++;
    }

    // Signal success
    pIRP->ioStatus = IO_STATUS_SUCCESS;
}

VOID VgaGetDeviceFunctions(PIRP pIRP)
{
    if (pIRP->outBufferSize != sizeof(VIDEODEVICEFUNCTIONS_VTBL))
    {
        return;
    }

    PVIDEODEVICEFUNCTIONS_VTBL pDeviceFunctions = (PVIDEODEVICEFUNCTIONS_VTBL) pIRP->pOutBuffer;

    if (currentVideoMode == VGA_VIDEO_MODE_320x200x256)
    {
        pDeviceFunctions->DeviceDrawPixelOverwrite  = (PVOID) VgaDrawDevicePixelOverwrite8Bit;
        pDeviceFunctions->DeviceDrawPixelXor        = (PVOID) VgaDrawDevicePixelXor8Bit;

        pDeviceFunctions->DeviceDrawFilledRectangle = (PVOID) VgaDrawDeviceFilledRectangle8Bit;

        pDeviceFunctions->DeviceHideMouse           = (PVOID) VgaHideMouse8Bit;
        pDeviceFunctions->DeviceShowMouse           = (PVOID) VgaShowMouse8Bit;
        pDeviceFunctions->DeviceMoveMouse           = (PVOID) VgaMoveMouse8Bit;
    }

    if (currentVideoMode == VGA_VIDEO_MODE_640x480x2)
    {
        pDeviceFunctions->DeviceDrawPixelOverwrite  = (PVOID) VgaDrawDevicePixelOverwrite1Bit;
        pDeviceFunctions->DeviceDrawPixelXor        = (PVOID) VgaDrawDevicePixelXor1Bit;

        pDeviceFunctions->DeviceDrawFilledRectangle = (PVOID) VgaDrawDeviceFilledRectangle1Bit;

        pDeviceFunctions->DeviceHideMouse           = (PVOID) VgaHideMouse1Bit;
        pDeviceFunctions->DeviceShowMouse           = (PVOID) VgaShowMouse1Bit;
        pDeviceFunctions->DeviceMoveMouse           = (PVOID) VgaMoveMouse1Bit;
    }

    if (currentVideoMode == VGA_VIDEO_MODE_640x480x16)
    {
        pDeviceFunctions->DeviceDrawPixelOverwrite  = (PVOID) VgaDrawDevicePixelOverwrite4Bit;
        pDeviceFunctions->DeviceDrawPixelXor        = (PVOID) VgaDrawDevicePixelXor4Bit;

        pDeviceFunctions->DeviceDrawFilledRectangle = (PVOID) VgaDrawDeviceFilledRectangle4Bit;

        pDeviceFunctions->DeviceHideMouse           = (PVOID) VgaHideMouse4Bit;
        pDeviceFunctions->DeviceShowMouse           = (PVOID) VgaShowMouse4Bit;
        pDeviceFunctions->DeviceMoveMouse           = (PVOID) VgaMoveMouse4Bit;
    }

    if (currentVideoMode == VGA_VIDEO_MODE_320x240x256)
    {
        pDeviceFunctions->DeviceDrawPixelOverwrite  = (PVOID) VgaDrawDevicePixelOverwrite8BitModeX;
        pDeviceFunctions->DeviceDrawPixelXor        = (PVOID) VgaDrawDevicePixelXor8BitModeX;

        pDeviceFunctions->DeviceDrawFilledRectangle = (PVOID) VgaDrawDeviceFilledRectangle8BitModeX;

        pDeviceFunctions->DeviceHideMouse           = (PVOID) VgaHideMouse8BitModeX;
        pDeviceFunctions->DeviceShowMouse           = (PVOID) VgaShowMouse8BitModeX;
        pDeviceFunctions->DeviceMoveMouse           = (PVOID) VgaMoveMouse8BitModeX;
    }

    pDeviceFunctions->DeviceSetMousePosition    = (PVOID) VgaSetMousePosition;
    pDeviceFunctions->DeviceGetMousePosition    = (PVOID) VgaGetMousePosition;
    pDeviceFunctions->DeviceSetMouseCursor      = (PVOID) VgaSetMouseCursor;
    pDeviceFunctions->DeviceGetMouseCursor      = (PVOID) VgaGetMouseCursor;

    // Signal success
    pIRP->ioStatus = IO_STATUS_SUCCESS;
}

// -- DRAWING FUNCTIONS --

// -- 2 COLORS --

VOID VgaDrawDevicePixelOverwrite1Bit(UINT32 screenX, UINT32 screenY, COLOR color)
{
    // Validation - Already done in caller functions

    // Drawing
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += screenY * 80;

    UINT8 d = 7 - screenX % 8;
    UINT8 byteMask = 0x1 << d;
    UINT8 screenByte = pFrame[screenX / 8];
    
    color = color & 0x1;
    color = color << d;

    pFrame[screenX / 8] &= ~byteMask;
    pFrame[screenX / 8] |= color;
}

VOID VgaDrawDevicePixelXor1Bit(UINT32 screenX, UINT32 screenY, COLOR color)
{
    // Validation - Already done in caller functions

    // Drawing
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += screenY * 80;

    UINT8 d = 7 - screenX % 8;
    UINT8 byteMask = 0x1 << d;
    UINT8 screenByte = pFrame[screenX / 8];
    
    color = color & 0x1;
    color = color << d;

    pFrame[screenX / 8] ^= color;
}


VOID VgaDrawDeviceFilledRectangle1Bit(UINT32 left, UINT32 top, UINT32 right, UINT32  bottom, COLOR color)
{
    // INT32 width  = right  - left;
    // INT32 height = bottom - top;

    // for(UINT32 y = top; y < bottom; y++)
    // {
    //     for(UINT32 x = left; x < right; x++)
    //     {
    //         VgaDrawDevicePixelOverwrite1Bit(x, y, color);
    //     }
    // }

    UINT8 byteMask;
    UINT8 temp;

    // Validation - Already done in caller functions
    
    // Drawing
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += top * 80;
    pFrame += left / 8;

    INT32 width  = right  - left;
    INT32 height = bottom - top;

    if (width <= 0 || height <= 0) return;

    UINT32 leftByteOffset = left / 8;
    UINT32 rightByteOffset = right / 8;
    UINT32 byteWidth = rightByteOffset - leftByteOffset;

    //UINT32 byteMask;
    UINT32 leftByteMask  = 255 >> (left % 8);
    UINT32 rightByteMask = 255 << (8 - (right % 8));  // 7??

    if (byteWidth == 0)
    {
        // Left and right is in the same byte (combine masks)
        byteMask = leftByteMask & rightByteMask;

        for(UINT32 y = 0; y < height; y++)
        {
            pFrame[y * 80] &= ~byteMask;

            if (color == 1)
            {
                pFrame[y * 80] |= byteMask;
            }
        }
    }
    else if (byteWidth == 1)
    {
        // Left and right part is one byte appart (no middle piece)
        byteMask = leftByteMask;

        for(UINT32 y = 0; y < height; y++)
        {
            pFrame[y * 80] &= ~byteMask;

            if (color == 1)
            {
                pFrame[y * 80] |= byteMask;
            }
        }

        pFrame++;
        byteMask = rightByteMask;

        for(UINT32 y = 0; y < height; y++)
        {
            pFrame[y * 80] &= ~byteMask;

            if (color == 1)
            {
                pFrame[y * 80] |= byteMask;
            }
        }
    }
    else
    {
        // There is left/right and middle part
        byteMask = leftByteMask;

        for(UINT32 y = 0; y < height; y++)
        {
            pFrame[y * 80] &= ~byteMask;

            if (color == 1)
            {
                pFrame[y * 80] |= byteMask;
            }
        }

        pFrame++;
        byteMask = 255;
        
        for(UINT32 y = 0; y < height; y++)
        {
            for(UINT32 x = 0; x < byteWidth - 1; x++)
            {
                pFrame[x] &= ~byteMask;

            if (color == 1)
            {
                pFrame[x] |= byteMask;
            }
            }

            pFrame += 80;
        }

        // pFrame -= height * GiDeviceWidthInBytes;
        // pFrame += rightByteOffset;

        pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
        pFrame += top * 80;
        pFrame += right / 8;

        byteMask = rightByteMask;

        for(UINT32 y = 0; y < height; y++)
        {
            pFrame[y * 80] &= ~byteMask;

            if (color == 1)
            {
                pFrame[y * 80] |= byteMask;
            }
        }
    }
}

VOID VgaShowMouse1Bit()
{
    if (VgaMouseShown)
    {
        return;
    }

    VgaSaveMouseArea1Bit(VgaMousePosition);
    GiDrawMouseCursor(VgaMousePosition.X, VgaMousePosition.Y, hVgaMouseCursor);

    VgaMousePositionOld = VgaMousePosition;
    VgaMouseShown = TRUE;
}

VOID VgaHideMouse1Bit()
{
    if (!VgaMouseShown)
    {
        return;
    }

    VgaRestoreMouseArea1Bit(VgaMousePositionOld);
    VgaMousePositionOld = VgaMousePosition;

    VgaMouseShown = FALSE;
}

VOID VgaMoveMouse1Bit(PPOINT pMousePosition)
{
    if (VgaMouseShown)
    {
        VgaMousePosition = pMousePosition[0];

        VgaRestoreMouseArea1Bit(VgaMousePositionOld);
        VgaSaveMouseArea1Bit(VgaMousePosition);

        GiDrawMouseCursor(VgaMousePosition.X, VgaMousePosition.Y, hVgaMouseCursor);

        VgaMousePositionOld = VgaMousePosition;    
    }       
}

VOID VgaSaveMouseArea1Bit(POINT mousePosition)
{
    PUINT8 pMouseBackground = (PUINT8) &VgaMouseBackground[0];
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += mousePosition.Y * 80;
    pFrame += mousePosition.X / 8;

    // Maybe
   	outportb(0x3CE, 0x5);	    // Graphics Mode Register
	outportb(0x3CF, 0);         // Write Mode 0

    outportb(0x3CE, 0);         // Load Set/Reset Data register
    outportb(0x3CF, 0);     // Color

    outportb(0x3CE, 1);         // Enable Set/Reset
    outportb(0x3CF, 0);        // Enable all planes

    outportb(0x3CE, 0x5);	     // Set Read Mode
    outportb(0x3CF, 0x0);         // Read Mode 0 (bit 0b10XX) - XX is the write mode

    outportb(0x3CE, 0x8);       // Bit Mask Register
    outportb(0x3CF, 255);       // Select bits

    for(UINT32 y = 0; y < 32; y++)
    {
        for(UINT32 plane = 0; plane < 4; plane++)
        {
    	    outportb(0x3CE, 0x4);    // Enable read
	        outportb(0x3CF, plane);

            for(UINT32 x = 0; x < 5; x++)
            {
                pMouseBackground[x] = pFrame[x];
            }

            pMouseBackground += 5;
        }

        pFrame += 80;
    }
}

VOID VgaRestoreMouseArea1Bit(POINT mousePosition)
{
  	outportb(0x3CE, 0x5);	    // Graphics Mode Register
	outportb(0x3CF, 0);         // Write Mode 0

    outportb(0x3CE, 0);         // Set/Reset Register
    outportb(0x3CF, 0);         // Set/Reset Value

    outportb(0x3CE, 1);         // Enable Set/Reset Register (Only Write Mode 0)
    outportb(0x3CF, 0);        // Enable all planes

    outportb(0x3CE, 2);         // Map Mask Register
    outportb(0x3CF, 15);         // Enable planes to write on

    outportb(0x3CE, 3);         // Mode
    outportb(0x3CF, 0x0);      // 0x00 - Byte from set-reset is forwarded, 0x08 - Latches and data is ANDed together, 0x10 - Latches and data is ORed together, 0x10, 0x18 - Latches and data is XORed

    outportb(0x3CE, 0x8);       // Bit Mask Register
    outportb(0x3CF, 255);       // Select bits

    // ---

    //outportb(0x3C4, 0x5);       // Enable Set/Reset Register
    //outportb(0x3C5, 15);        // All planes


    PUINT8 pMouseBackground = (PUINT8) &VgaMouseBackground[0];
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += mousePosition.Y * 80;
    pFrame += mousePosition.X / 8;
    UINT8 temp;
    UINT8 value;

    for(UINT32 y = 0; y < 32; y++)
    {
        for(UINT32 plane = 0; plane < 4; plane++)
        {
            outportb(0x3C4, 0x2);       // Enable Set/Reset Register   (Is it 2 or 5???)
            outportb(0x3C5, 1 << plane);        // All planes

            for(UINT32 x = 0; x < 5; x++)    
            {
                pFrame[x] = 0;

                value = pMouseBackground[x];

                temp = pFrame[x];
                pFrame[x] = value;
            }

            pMouseBackground += 5;
        }

        pFrame += 80;
    }

    outportb(0x3C4, 0x2);       // Enable Set/Reset Register   (Is it 2 or 5???)
    outportb(0x3C5, 15);        // All planes
}

// -- 16 COLORS --

VOID VgaDrawDevicePixelOverwrite4Bit(UINT32 screenX, UINT32 screenY, COLOR color)
{
    // Validation - Already done in caller functions

    // Drawing
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += screenY * 80;

    UINT8 byteMask = 1 << (7 - screenX % 8);

    outportb(0x3CE, 0);         // Load Set/Reset Data register
    outportb(0x3CF, color);     // Color

    outportb(0x3CE, 1);         // Enable Set/Reset
    outportb(0x3CF, 15);        // Enable all planes

    outportb(0x3CE, 0x8);       // Enable data change
    outportb(0x3CF, byteMask);  // All bits

	// // outportb(0x3CE, 0x5);	     // Set Write Mode
	// // outportb(0x3CF, 0x3);         // Write Mode 3
	// // outportb(0x3CE, 0x0);	     // Plane
	// // outportb(0x3CF, color);  // Write only to selected planes

    // Not needed:
    // outportb(0x3CE, 3);         // Mode
    // outportb(0x3CF, 3);         // 0 - Overwrite

    // outportb(0x3CE, 0x5);       // Write Mode
    // outportb(0x3CF, 0);         // Mode 0
	
    // outportb(0x3C4, 0x5);       // Enable Set/Reset Register
    // outportb(0x3C5, 15);        // All planes


    UINT8 temp = pFrame[screenX / 8]; // Read to fill latches (cache bytes) with accurate data

    pFrame[screenX / 8] = byteMask;
}

VOID VgaDrawDevicePixelXor4Bit(UINT32 screenX, UINT32 screenY, COLOR color)
{
    // Validation - Already done in caller functions

    // Drawing
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += screenY * 80;

    UINT8 byteMask = 1 << (7 - screenX % 8);

    outportb(0x3CE, 3);         // Mode
    outportb(0x3CF, 24);         // 0 - Overwrite, 24 - Xor, 4 - And ?, 2 - Or ?

    outportb(0x3CE, 0);         // Load Set/Reset Data register
    outportb(0x3CF, color);     // Color

    outportb(0x3CE, 1);         // Enable Set/Reset
    outportb(0x3CF, 15);        // Enable all planes

    outportb(0x3CE, 0x8);       // Enable data change
    outportb(0x3CF, byteMask);  // All bits

    UINT8 temp = pFrame[screenX / 8]; // Read to fill latches (cache bytes) with accurate data

    pFrame[screenX / 8] |= byteMask;

    // Reset
    outportb(0x3CE, 3);         // Mode
    outportb(0x3CF, 0);         // 0 - Overwrite
}

VOID VgaDrawDeviceFilledRectangle4Bit(UINT32 left, UINT32 top, UINT32 right, UINT32  bottom, COLOR color)
{
    UINT8 byteMask;
    UINT8 temp;

    // Validation - Already done in caller functions
    
    // Drawing
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += top * 80;
    pFrame += left / 8;

    INT32 width  = right  - left;
    INT32 height = bottom - top;

    UINT32 leftByteOffset = left / 8;
    UINT32 rightByteOffset = right / 8;
    UINT32 byteWidth = rightByteOffset - leftByteOffset;

    UINT32 leftByteMask  = 255 >> (left % 8);
    UINT32 rightByteMask = 255 << (8 - (right % 8));  // 7??

    outportb(0x3CE, 0);         // Load Set/Reset Data register
    outportb(0x3CF, color);     // Color

    outportb(0x3CE, 1);         // Enable Set/Reset
    outportb(0x3CF, 15);        // Enable all planes

    if (byteWidth == 0)
    {
        // Left and right is in the same byte (combine masks)
        outportb(0x3CE, 0x8);                           // Enable data change
        outportb(0x3CF, leftByteMask & rightByteMask);  // Select bits

        for(UINT32 y = 0; y < height; y++)
        {
            temp = pFrame[y * 80]; // Read to fill latches (cache bytes) with accurate data
            pFrame[y * 80] = 255;
        }
    }
    else if (byteWidth == 1)
    {
        // Left and right part is one byte appart (no middle piece)
        outportb(0x3CE, 0x8);           // Enable data change
        outportb(0x3CF, leftByteMask);  // Select bits

        for(UINT32 y = 0; y < height; y++)
        {
            temp = pFrame[y * 80]; // Read to fill latches (cache bytes) with accurate data
            pFrame[y * 80] = 255;
        }

        outportb(0x3CE, 0x8);            // Enable data change
        outportb(0x3CF, rightByteMask);  // Select bits

        pFrame++;

        for(UINT32 y = 0; y < height; y++)
        {
            temp = pFrame[y * 80]; // Read to fill latches (cache bytes) with accurate data
            pFrame[y * 80] = 255;
        }
    }
    else
    {
        // There is left/right and middle part
        outportb(0x3CE, 0x8);           // Enable data change
        outportb(0x3CF, leftByteMask);  // Select bits

        for(UINT32 y = 0; y < height; y++)
        {
            temp = pFrame[y * 80]; // Read to fill latches (cache bytes) with accurate data
            pFrame[y * 80] = 255;
        }

        outportb(0x3CE, 0x8);           // Enable data change
        outportb(0x3CF, 255);           // Select bits

        pFrame++;
        
        for(UINT32 y = 0; y < height; y++)
        {
            for(UINT32 x = 0; x < byteWidth - 1; x++)
            {
                temp = pFrame[x]; // Read to fill latches (cache bytes) with accurate data
                pFrame[x] = 255;
            }

            pFrame += 80;
        }

        // pFrame -= height * GiDeviceWidthInBytes;
        // pFrame += rightByteOffset;

        pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
        pFrame += top * 80;
        pFrame += right / 8;

        outportb(0x3CE, 0x8);            // Enable data change
        outportb(0x3CF, rightByteMask);  // Select bits

        for(UINT32 y = 0; y < height; y++)
        {
            temp = pFrame[y * 80]; // Read to fill latches (cache bytes) with accurate data
            pFrame[y * 80] = 255;
        }
    }
}

// -- Mouse Pointer --

VOID VgaShowMouse4Bit()
{
    if (VgaMouseShown)
    {
        return;
    }

    VgaSaveMouseArea4Bit(VgaMousePosition);
    GiDrawMouseCursor(VgaMousePosition.X, VgaMousePosition.Y, hVgaMouseCursor);

    VgaMousePositionOld = VgaMousePosition;
    VgaMouseShown = TRUE;
}

VOID VgaHideMouse4Bit()
{
    if (!VgaMouseShown)
    {
        return;
    }

    VgaRestoreMouseArea4Bit(VgaMousePositionOld);
    VgaMousePositionOld = VgaMousePosition;

    VgaMouseShown = FALSE;
}

VOID VgaMoveMouse4Bit(PPOINT pMousePosition)
{
    if (VgaMouseShown)
    {
        VgaMousePosition = pMousePosition[0];

        VgaRestoreMouseArea4Bit(VgaMousePositionOld);
        VgaSaveMouseArea4Bit(VgaMousePosition);

        GiDrawMouseCursor(VgaMousePosition.X, VgaMousePosition.Y, hVgaMouseCursor);

        VgaMousePositionOld = VgaMousePosition;    
    }       
}

VOID VgaSaveMouseArea4Bit(POINT mousePosition)
{
    PUINT8 pMouseBackground = (PUINT8) &VgaMouseBackground[0];
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += mousePosition.Y * 80;
    pFrame += mousePosition.X / 8;

    // Maybe
   	outportb(0x3CE, 0x5);	    // Graphics Mode Register
	outportb(0x3CF, 0);         // Write Mode 0

    outportb(0x3CE, 0);         // Load Set/Reset Data register
    outportb(0x3CF, 0);     // Color

    outportb(0x3CE, 1);         // Enable Set/Reset
    outportb(0x3CF, 0);        // Enable all planes

    outportb(0x3CE, 0x5);	     // Set Read Mode
    outportb(0x3CF, 0x0);         // Read Mode 0 (bit 0b10XX) - XX is the write mode

    outportb(0x3CE, 0x8);       // Bit Mask Register
    outportb(0x3CF, 255);       // Select bits

    for(UINT32 y = 0; y < 32; y++)
    {
        for(UINT32 plane = 0; plane < 4; plane++)
        {
    	    outportb(0x3CE, 0x4);    // Enable read
	        outportb(0x3CF, plane);

            for(UINT32 x = 0; x < 5; x++)
            {
                pMouseBackground[x] = pFrame[x];
            }

            pMouseBackground += 5;
        }

        pFrame += 80;
    }
}

VOID VgaRestoreMouseArea4Bit(POINT mousePosition)
{
  	outportb(0x3CE, 0x5);	    // Graphics Mode Register
	outportb(0x3CF, 0);         // Write Mode 0

    outportb(0x3CE, 0);         // Set/Reset Register
    outportb(0x3CF, 0);         // Set/Reset Value

    outportb(0x3CE, 1);         // Enable Set/Reset Register (Only Write Mode 0)
    outportb(0x3CF, 0);        // Enable all planes

    outportb(0x3CE, 2);         // Map Mask Register
    outportb(0x3CF, 15);         // Enable planes to write on

    outportb(0x3CE, 3);         // Mode
    outportb(0x3CF, 0x0);      // 0x00 - Byte from set-reset is forwarded, 0x08 - Latches and data is ANDed together, 0x10 - Latches and data is ORed together, 0x10, 0x18 - Latches and data is XORed

    outportb(0x3CE, 0x8);       // Bit Mask Register
    outportb(0x3CF, 255);       // Select bits

    // ---

    //outportb(0x3C4, 0x5);       // Enable Set/Reset Register
    //outportb(0x3C5, 15);        // All planes


    PUINT8 pMouseBackground = (PUINT8) &VgaMouseBackground[0];
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += mousePosition.Y * 80;
    pFrame += mousePosition.X / 8;
    UINT8 temp;
    UINT8 value;

    for(UINT32 y = 0; y < 32; y++)
    {
        for(UINT32 plane = 0; plane < 4; plane++)
        {
            outportb(0x3C4, 0x2);       // Enable Set/Reset Register   (Is it 2 or 5???)
            outportb(0x3C5, 1 << plane);        // All planes

            for(UINT32 x = 0; x < 5; x++)    
            {
                pFrame[x] = 0;

                value = pMouseBackground[x];

                temp = pFrame[x];
                pFrame[x] = value;
            }

            pMouseBackground += 5;
        }

        pFrame += 80;
    }

    outportb(0x3C4, 0x2);       // Enable Set/Reset Register   (Is it 2 or 5???)
    outportb(0x3C5, 15);        // All planes
}

// -- 256 COLORS --
VOID VgaDrawDevicePixelOverwrite8Bit(UINT32 screenX, UINT32 screenY, COLOR color)
{
    // Validation - Already done in caller functions

    // Drawing
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += screenY * 320;

    pFrame[screenX] = color;
}

// static void write_pixel8x(unsigned x, unsigned y, unsigned c)
// {
// 	unsigned wd_in_bytes;
// 	unsigned off;

// 	wd_in_bytes = g_wd / 4;
// 	off = wd_in_bytes * y + x / 4;
// 	set_plane(x & 3);
// 	vpokeb(off, c);
// }

// static void set_plane(unsigned p)
// {
// 	unsigned char pmask;

// 	p &= 3;
// 	pmask = 1 << p;
// /* set read plane */
// 	outportb(VGA_GC_INDEX, 4);
// 	outportb(VGA_GC_DATA, p);
// /* set write plane */
// 	outportb(VGA_SEQ_INDEX, 2);
// 	outportb(VGA_SEQ_DATA, pmask);
// }

VOID VgaDrawDevicePixelXor8Bit(UINT32 screenX, UINT32 screenY, COLOR color)
{
    // Validation - Already done in caller functions

    // Drawing
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += screenY * 320;

    pFrame[screenX] ^= color;
}

VOID VgaDrawDeviceFilledRectangle8Bit(UINT32 left, UINT32 top, UINT32 right, UINT32  bottom, COLOR color)
{
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += top * 320;
    pFrame += left;

    UINT32 width = right - left;
    UINT32 height = bottom - top;

    if (width <= 0 || height <= 0) return;

    for(UINT32 y = 0; y < height; y++)
    {
        for(UINT32 x = 0; x < width; x++)
        {
            pFrame[x] = color;        
        }

        pFrame += 320;
    }
}

VOID VgaShowMouse8Bit()
{
    if (VgaMouseShown)
    {
        return;
    }

    VgaSaveMouseArea8Bit(VgaMousePosition);
    GiDrawMouseCursor(VgaMousePosition.X, VgaMousePosition.Y, hVgaMouseCursor);

    VgaMousePositionOld = VgaMousePosition;
    VgaMouseShown = TRUE;
}

VOID VgaHideMouse8Bit()
{
    if (!VgaMouseShown)
    {
        return;
    }

    VgaRestoreMouseArea8Bit(VgaMousePositionOld);
    VgaMousePositionOld = VgaMousePosition;

    VgaMouseShown = FALSE;
}

VOID VgaMoveMouse8Bit(PPOINT pMousePosition)
{
    if (VgaMouseShown)
    {
        VgaMousePosition = pMousePosition[0];

        VgaRestoreMouseArea8Bit(VgaMousePositionOld);
        VgaSaveMouseArea8Bit(VgaMousePosition);

        GiDrawMouseCursor(VgaMousePosition.X, VgaMousePosition.Y, hVgaMouseCursor);

        VgaMousePositionOld = VgaMousePosition;    
    }       
}

VOID VgaSaveMouseArea8Bit(POINT mousePosition)
{
    PUINT8 pMouseBackground = (PUINT8) &VgaMouseBackground[0];
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += mousePosition.Y * 320;
    pFrame += mousePosition.X;

    for(UINT32 y = 0; y < 32; y++)
    {
        for(UINT32 x = 0; x < 32; x++)
        {
            pMouseBackground[x] = pFrame[x];
        }

        pMouseBackground += 32;
        pFrame += 320;
    }
}

VOID VgaRestoreMouseArea8Bit(POINT mousePosition)
{
    PUINT8 pMouseBackground = (PUINT8) &VgaMouseBackground[0];
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += mousePosition.Y * 320;
    pFrame += mousePosition.X;

    for(UINT32 y = 0; y < 32; y++)
    {
        for(UINT32 x = 0; x < 32; x++)
        {
            pFrame[x] = pMouseBackground[x];
        }

        pMouseBackground += 32;
        pFrame += 320;
    }
}

// -- 256 COLORS MODEX -
VOID SetPlane(UINT8 p)
{
    UINT8 pmask;

    p &= 3;
    pmask = 1 << p;

    /* set read plane */
	outportb(VGA_GC_INDEX, 4);
	outportb(VGA_GC_DATA, p);
/* set write plane */
	outportb(VGA_SEQ_INDEX, 2);
	outportb(VGA_SEQ_DATA, pmask);
}

VOID VgaDrawDevicePixelOverwrite8BitModeX(UINT32 screenX, UINT32 screenY, COLOR color)
{
    // Validation - Already done in caller functions

    // Drawing
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += screenY * 320 / 4;

    SetPlane(screenX);// & 3);
    pFrame[screenX / 4] = color;
}

VOID VgaDrawDevicePixelXor8BitModeX(UINT32 screenX, UINT32 screenY, COLOR color)
{
    // Validation - Already done in caller functions

    // Drawing
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += screenY * 320 / 4;

    SetPlane(screenX);// & 3);
    pFrame[screenX / 4] ^= color;
}

VOID VgaDrawDeviceFilledRectangle8BitModeX(UINT32 left, UINT32 top, UINT32 right, UINT32  bottom, COLOR color)
{
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += top * 320 / 4;

    UINT32 width = right - left;
    UINT32 height = bottom - top;

    if (width <= 0 || height <= 0) return;

    for(UINT32 y = 0; y < height; y++)
    {
        for(UINT32 x = 0; x < width; x++)
        {
            SetPlane(left + x);
            pFrame[(left + x) / 4] = color;        
        }

        pFrame += 320 / 4;
    }
}

VOID VgaShowMouse8BitModeX()
{
    if (VgaMouseShown)
    {
        return;
    }

    VgaSaveMouseArea8BitModeX(VgaMousePosition);
    GiDrawMouseCursor(VgaMousePosition.X, VgaMousePosition.Y, hVgaMouseCursor);

    VgaMousePositionOld = VgaMousePosition;
    VgaMouseShown = TRUE;
}

VOID VgaHideMouse8BitModeX()
{
    if (!VgaMouseShown)
    {
        return;
    }

    VgaRestoreMouseArea8BitModeX(VgaMousePositionOld);
    VgaMousePositionOld = VgaMousePosition;

    VgaMouseShown = FALSE;
}

VOID VgaMoveMouse8BitModeX(PPOINT pMousePosition)
{
    if (VgaMouseShown)
    {
        VgaMousePosition = pMousePosition[0];

        VgaRestoreMouseArea8BitModeX(VgaMousePositionOld);
        VgaSaveMouseArea8BitModeX(VgaMousePosition);

        GiDrawMouseCursor(VgaMousePosition.X, VgaMousePosition.Y, hVgaMouseCursor);

        VgaMousePositionOld = VgaMousePosition;    
    }       
}

VOID VgaSaveMouseArea8BitModeX(POINT mousePosition)
{
    PUINT8 pMouseBackground = (PUINT8) &VgaMouseBackground[0];
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += mousePosition.Y * 320 / 4;

    for(UINT32 y = 0; y < 32; y++)
    {
        for(UINT32 x = 0; x < 32; x++)
        {
            SetPlane(mousePosition.X + x);
            pMouseBackground[x] = pFrame[(mousePosition.X + x) / 4];
        }

        pMouseBackground += 32;
        pFrame += 320 / 4;
    }
}

VOID VgaRestoreMouseArea8BitModeX(POINT mousePosition)
{
    PUINT8 pMouseBackground = (PUINT8) &VgaMouseBackground[0];
    PUINT8 pFrame = (PUINT8) VGA_FRAME_BUFFER_ADDRESS;
    pFrame += mousePosition.Y * 320 / 4;

    for(UINT32 y = 0; y < 32; y++)
    {
        for(UINT32 x = 0; x < 32; x++)
        {
            SetPlane(mousePosition.X + x);
            pFrame[(mousePosition.X + x) / 4] = pMouseBackground[x];
        }

        pMouseBackground += 32;
        pFrame += 320 / 4;
    }
}

// -- COMMON --

VOID VgaSetMousePosition(PPOINT pMousePosition)
{
    VgaMousePosition = pMousePosition[0];
}

VOID VgaGetMousePosition(OUT PPOINT pMousePosition)
{
    pMousePosition[0] = VgaMousePosition;
}

VOID VgaSetMouseCursor(HCURSOR hCursor)
{
    hVgaMouseCursor = hCursor;
}

HCURSOR VgaGetMouseCursor()
{
    return hVgaMouseCursor;
}
