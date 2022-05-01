#include <kldr.h>

static CHAR MmwWindowClass[]        = "MEMORYMAP";
static CHAR MmwWindowTitle[]        = "Memory Map";

static CHAR MmwButtonCaptionClose[] = "Close";

static HBUTTON hButtonClose;

static CHAR textBuffer[40]  = { 0 };

static CHAR typeFree[]      = "Free";
static CHAR typeReserved[]  = "Reserved";
static CHAR typeKernel[]    = "Kernel";
static CHAR typeBad[]       = "Bad";
static CHAR typeUnknown[]   = "Unknown";

static INT32 valueY = 0;

UINT32 MemoryMapWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2);
VOID DrawMemoryMap(HDEVICE_CONTEXT hDeviceContext, RECTANGLE clientRectangle);
VOID MmwGetFormattedText(PCHAR pDestination, const char* Format, ...);

VOID RegisterMemoryMapWindow()
{
    RegisterWindow((PCHAR) &MmwWindowClass, 0, 0, 0, 0, GetColorFromComponents(20, 35, 80), (PVOID) MemoryMapWindowProcedure, 0);
}

VOID CreateMemoryMapWindow()
{
    valueY = 200;

    hMemoryMapWindow = CreateWindowByClass((PCHAR) &MmwWindowClass, (PCHAR) &MmwWindowTitle, 120, 20, 470, 400, 0, 0, WINDOW_FLAG_IS_VISIBLE | WINDOW_FLAG_HAS_DIALOG_BORDER | WINDOW_FLAG_HAS_CAPTION | WINDOW_FLAG_VERTICAL_SCROLLBAR);
    
    //hButtonClose = CreateButton((PCHAR) &MmwButtonCaptionClose, 365, 350, 75, 23, hMemoryMapWindow, WINDOW_FLAG_IS_VISIBLE | BUTTON_FLAG_IS_FOCUSED | BUTTON_FLAG_IS_DEFAULT);

    MemoryMapWindowCreatedHash = 0xBAADB005;
}

UINT32 MemoryMapWindowProcedure(HWINDOW hWindow, WINDOW_MESSAGE message, UINT32 param1, UINT32 param2)
{
    if (message == WINDOW_MESSAGE_PAINT)
    {
        HDEVICE_CONTEXT hDeviceContext = GetDeviceContext(hWindow);
        DrawBegin(hDeviceContext);

        RECTANGLE clientRectangle;
        GetClientRectangle(hWindow, &clientRectangle);
        
        DrawMemoryMap(hDeviceContext, clientRectangle);

        DrawEnd(hDeviceContext);

        return;
    }

    if (message == WINDOW_MESSAGE_COMMAND)
    {
        if (param1 == COMMAND_BUTTON_CLICKED && param2 == hButtonClose)
        {
            HideWindow(hWindow);

            return;
        }

        if (param1 == COMMAND_SCROLL_UP)
        {
            valueY -= 5;// 20;
            SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

            return;
        }

        if (param1 == COMMAND_SCROLL_DOWN)
        {
            valueY += 5;// 20;
            SendMessage(hWindow, WINDOW_MESSAGE_PAINT, 0, 0);

            return;
        }

    }

    DefaultWindowProcedure(hWindow, message, param1, param2);
}

VOID DrawMemoryMap(HDEVICE_CONTEXT hDeviceContext, RECTANGLE clientRectangle)
{
    // Edge
    //clientRectangle.Top += 2;
    clientRectangle.Bottom -= 1;
    clientRectangle.Right -= GetSystemMetric(METRIC_SCROLLBAR_SIZE);
    SetBackgroundColor(hDeviceContext, GetSystemColor(COLOR_CONTROL_CONTENT_AREA));
    SetTextColor(hDeviceContext, 0xFF000000);

    DrawControlState(hDeviceContext, clientRectangle.Left, clientRectangle.Top, clientRectangle.Right, clientRectangle.Bottom, DRAW_CONTROL_STATE_EDGE_SUNKEN);
    DrawFilledRectangleA(hDeviceContext, clientRectangle.Left + 2, clientRectangle.Top + 2, clientRectangle.Right - 2, clientRectangle.Bottom - 1);

    // Header

    clientRectangle.Top += 10 + valueY;
    clientRectangle.Left += 10;
    DrawText(hDeviceContext, "Base", &clientRectangle, 0);

    clientRectangle.Left += 175;
    DrawText(hDeviceContext, "Length (bytes)", &clientRectangle, 0);

    clientRectangle.Left += 175;
    DrawText(hDeviceContext, "Type", &clientRectangle, 0);

    // Entries
    MEMORY_MAP_ENTRY memoryMapEntry = { 0 };
    PCHAR pTypeText;

    while(EnumerateMemoryMapEntries(&memoryMapEntry))
    {
        clientRectangle.Top += 20;
        clientRectangle.Left -= 350;
        MmwGetFormattedText(&textBuffer[0], "%x (%u)", memoryMapEntry.base, memoryMapEntry.base);
        DrawText(hDeviceContext, &textBuffer[0], &clientRectangle, 0);

        clientRectangle.Left += 175;
        MmwGetFormattedText(&textBuffer[0], "%x (%u)", memoryMapEntry.size, memoryMapEntry.size);
        DrawText(hDeviceContext, &textBuffer[0], &clientRectangle, 0);

        clientRectangle.Left += 175;
        if (memoryMapEntry.type == MEMORY_MAP_ENTRY_TYPE_FREE)  pTypeText = &typeFree[0];
        if (memoryMapEntry.type == MEMORY_MAP_ENTRY_TYPE_RESERVED)  pTypeText = &typeReserved[0];
        if (memoryMapEntry.type == MEMORY_MAP_ENTRY_TYPE_KERNEL)  pTypeText = &typeKernel[0];
        if (memoryMapEntry.type == MEMORY_MAP_ENTRY_TYPE_BAD)  pTypeText = &typeBad[0];
        if (memoryMapEntry.type == MEMORY_MAP_ENTRY_TYPE_UNKNOWN)  pTypeText = &typeUnknown[0];
        MmwGetFormattedText(&textBuffer[0], "%u (%s)", memoryMapEntry.type, pTypeText);
        DrawText(hDeviceContext, &textBuffer[0], &clientRectangle, 0);
    }
}

VOID MmwGetFormattedText(PCHAR pDestination, const char* Format, ...)
{
	CHAR numberBuffer[32] = "abcdefghijklm\0";
	va_list ap;
	size_t length;
	
	CHAR buffer[256]= {0};
	PCHAR pBuffer;
	
	for (int i = 0; i < sizeof(buffer) - 1; i++)
	{
		buffer[i] = '/';
 	}
	
	pBuffer = (char*) buffer;
	
	va_start(ap, Format);
	length = vsnprintf(buffer, sizeof(buffer), Format, ap);
	va_end(ap);
	
    strcpy(&pDestination[0], (const char*) pBuffer);
    length = strlen(pBuffer);
    pDestination[length] = '\0';
}