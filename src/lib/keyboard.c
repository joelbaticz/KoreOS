/*
  - Date: 2022-02-02
  - Author: Joel
  - Purpose: PS/2 keyboard driver
*/

#include <kldr.h>

#define MAX_NUMBER_OF_SCAN_CODES                        256//16
#define MAX_NYMBER_OF_VIRTUAL_KEYS						256

#define KEYBOARD_COMMAND_DISABLE_KEYBOARD               0
#define KEYBOARD_COMMAND_DISABLE_AUXILARY               0
#define KEYBOARD_COMMAND_ENABLE_KEYBOARD                0
#define KEYBOARD_COMMAND_ENABLE_AUXILARY                0

#define KEYBOARD_COMMAND_READ_CONFIGURATION_BYTE        0
#define KEYBOARD_COMMAND_WRITE_CONFIGURATION_BYTE       0

#define KEYBOARD_COMMAND_EXECUTE_SELF_TEXT              0
#define KEYBOARD_SELF_TEST_SUCCESS                      0

#define KEYBOARD_COMMAND_PORT                           0
#define KEYBOARD_DATA_PORT                              0
#define KEYBOARD_STATUS_PORT                            0

#define KB_IN_FULL                                      0
#define KB_OUT_FULL                                     0
#define KEYBOARD_AUXILARY_BYTE                          0

static UINT32 scanCodeCount;
static UINT8  scanCodes[MAX_NUMBER_OF_SCAN_CODES];			// Hardware dependent scan-codes
static UINT8  virtualKeyStates[256];						// System standardized virtual key codes

static BOOL isShiftDown;
static BOOL isAltDown;
static BOOL isControlDown;
static BOOL isNumLockDown;

// KSTATUS KeyboardInit();
// VOID    KeyboardWriteCommand(UINT32 command);
// VOID    KeyboardWriteCommand2(UINT32 command, UINT32 data);
// UINT32  KeyboardReadData();
// BOOL    KeyboardWait();
// VOID    KeyboardScan(PUINT8 pByte, PBOOL pIsAuxilaryPort);
// VOID    KeyboardSetLEDs(UINT32 ledMask);

uint8_t keyboard_to_ascii(uint8_t key);

UINT32 ConvertToVirtualKey(UINT8 scanCode);

enum KEYS
{
	KEY_NULL    	= 0x00,

	KEY_ESCAPE  	= 0x01,
	KEY_1       	= 0x02,
	KEY_2       	= 0x03,
	KEY_3       	= 0x04,
	KEY_4       	= 0x05,
	KEY_5       	= 0x06,
	KEY_6       	= 0x07,
	KEY_7       	= 0x08,
	KEY_8       	= 0x09,
	KEY_9       	= 0x0A,
	KEY_0	    	= 0x0B,
	KEY_MINUS   	= 0x0C,
	KEY_EQUALS  	= 0x0D,
	KEY_BACKSPACE	= 0x0E,

	KEY_TAB     	= 0x0F,
	KEY_Q       	= 0x10,
	KEY_W       	= 0x11,
	KEY_E       	= 0x12,
	KEY_R       	= 0x13,
	KEY_T       	= 0x14,
	KEY_Y       	= 0x15,
	KEY_U       	= 0x16,
	KEY_I       	= 0x17,
	KEY_O       	= 0x18,
	KEY_P       	= 0x19,
	KEY_BRACKETOPEN = 0x1A,
	KEY_BRACKETCLOSED = 0x1B,
	KEY_ENTER       = 0x1C,
	KEY_ALT  	    = 0x1D,

	KEY_A       	= 0x1E,
	KEY_S       	= 0x1F,
	KEY_D       	= 0x20,
	KEY_F       	= 0x21,
	KEY_G       	= 0x22,
	KEY_H       	= 0x23,
	KEY_J       	= 0x24,
	KEY_K       	= 0x25,
	KEY_L       	= 0x26,
	KEY_SEMICOLON   = 0x27,
	KEY_INVCOMMA    = 0x28,
	KEY_APHOSTROPHY = 0x29,

	KEY_LEFTSHIFT   = 0x2A,
	KEY_BACKSLASH   = 0x2B,
	KEY_Z       	= 0x2C,
	KEY_X       	= 0x2D,
	KEY_C       	= 0x2E,
	KEY_V       	= 0x2F,
	KEY_B       	= 0x30,
	KEY_N       	= 0x31,
	KEY_M       	= 0x32,
	KEY_COMMA       = 0x33,
	KEY_PERIOD      = 0x34,
	KEY_SLASH       = 0x35,
	KEY_RIGHTSHIFT  = 0x36,

	KEY_PRINTSCREEN = 0x37,
	KEY_CONTROL     = 0x38,
	KEY_SPACE       = 0x39,
	KEY_CAPSLOCK    = 0x3A,

	KEY_F1          = 0x3B,
	KEY_F2          = 0x3C,
	KEY_F3          = 0x3D,
	KEY_F4          = 0x3E,
	KEY_F5          = 0x3F,
	KEY_F6          = 0x40,
	KEY_F7          = 0x41,
	KEY_F8          = 0x42,
	KEY_F9          = 0x43,
	KEY_F10         = 0x44,

	KEY_NUMLOCK     = 0x45,
	KEY_SCROLLOCK   = 0x46,
	KEY_NUMPAD7     = 0x47,
	KEY_NUMPAD8     = 0x48,
	KEY_NUMPAD9     = 0x49,
	KEY_NUMPADMINUS = 0x4A,
	KEY_NUMPAD4     = 0x4B,
	KEY_NUMPAD5     = 0x4C,
	KEY_NUMPAD6     = 0x4D,
	KEY_NUMPADPLUS  = 0x4E,
	KEY_NUMPAD1     = 0x4F,
	KEY_NUMPAD2     = 0x50,
	KEY_NUMPAD3     = 0x51,
	KEY_NUMPADINSERT = 0x52,
	KEY_NUMPADDELETE = 0x53,
};

VOID KeyboardDriverInit()
{
    scanCodeCount = 0;
    memset(scanCodes, 'a', MAX_NUMBER_OF_SCAN_CODES);

	isShiftDown = FALSE;
	isAltDown = FALSE;
	isControlDown = FALSE;
	isNumLockDown = FALSE;

    keyboardServiceInstalled = TRUE;
}

VOID KeyboardHandler()
{
    // Read keyboard scan code

    UINT8 status = MachineIoReadPortUint8(0x64);

    if (status & 1)
    {
        UINT8 scanCode = MachineIoReadPortUint8(0x60);

        // Store scan code
        if (scanCodeCount < MAX_NUMBER_OF_SCAN_CODES)
        {
            scanCodes[scanCodeCount] = scanCode;

            scanCodeCount++;
        }
    }
}

VOID KeyboardProcessKey()
{
    if (scanCodeCount > 0)
    {
        UINT8 scanCode = scanCodes[0];

        for(UINT32 i = 0; i < MAX_NUMBER_OF_SCAN_CODES - 1; i++)
        {
            scanCodes[i] = scanCodes[i + 1];
        }

        scanCodeCount--;

		// Get focused window
		HWINDOW hWindowDestination = GetFocusedWindow();

		// If null get active window
		if (hWindowDestination == (HWINDOW) NULL)
		{
			hWindowDestination = GetActiveWindow();
		}

		// Determine if it is a press or release and which VIRTUAL_KEY_
		BOOL isKeyPress = TRUE;
		if (scanCode & 0x80) isKeyPress = FALSE;

		UINT32 virtualKey = ConvertToVirtualKey(scanCode);

		// Stored the state of the button in a 256 element array (pressed/released)

		if (isKeyPress && virtualKey == VKEY_LEFTSHIFT) isShiftDown = TRUE;
		if (isKeyPress && virtualKey == VKEY_RIGHTSHIFT) isShiftDown = TRUE;
		if (isKeyPress && virtualKey == VKEY_ALT) isAltDown = TRUE;
		if (isKeyPress && virtualKey == VKEY_CONTROL) isControlDown = TRUE;
		if (isKeyPress && virtualKey == VKEY_NUMLOCK) isNumLockDown = TRUE;

		if (!isKeyPress && virtualKey == VKEY_LEFTSHIFT) isShiftDown = FALSE;
		if (!isKeyPress && virtualKey == VKEY_RIGHTSHIFT) isShiftDown = FALSE;
		if (!isKeyPress && virtualKey == VKEY_ALT) isAltDown = FALSE;
		if (!isKeyPress && virtualKey == VKEY_CONTROL) isControlDown = FALSE;
		if (!isKeyPress && virtualKey == VKEY_NUMLOCK) isNumLockDown = FALSE;

		// TODO
		// if (isKeyPress)
		// {
		// 	virtualKeyStates[virtualKey] = 1;
		// }
		// else
		// {
		// 	virtualKeyStates[virtualKey] = 0;
		// }

		// Send message
		if (isKeyPress)
		{
			SendMessage(hWindowDestination, WINDOW_MESSAGE_KEYDOWN, virtualKey, 0);
		}
		else
		{
			SendMessage(hWindowDestination, WINDOW_MESSAGE_KEYUP, virtualKey, 0);
		}
        
    }
}

UINT32 ConvertToVirtualKey(UINT8 scanCode)
{
	// Remove release flag
	scanCode &= ~0x80;

	if (scanCode == KEY_0) return VKEY_0;
	if (scanCode == KEY_1) return VKEY_1;
	if (scanCode == KEY_2) return VKEY_2;
	if (scanCode == KEY_3) return VKEY_3;
	if (scanCode == KEY_4) return VKEY_4;
	if (scanCode == KEY_5) return VKEY_5;
	if (scanCode == KEY_6) return VKEY_6;
	if (scanCode == KEY_7) return VKEY_7;
	if (scanCode == KEY_8) return VKEY_8;
	if (scanCode == KEY_9) return VKEY_9;

	if (scanCode == KEY_A) return VKEY_A;
	if (scanCode == KEY_B) return VKEY_B;
	if (scanCode == KEY_C) return VKEY_C;
	if (scanCode == KEY_D) return VKEY_D;
	if (scanCode == KEY_E) return VKEY_E;
	if (scanCode == KEY_F) return VKEY_F;
	if (scanCode == KEY_G) return VKEY_G;
	if (scanCode == KEY_H) return VKEY_H;
	if (scanCode == KEY_I) return VKEY_I;
	if (scanCode == KEY_J) return VKEY_J;
	if (scanCode == KEY_K) return VKEY_K;
	if (scanCode == KEY_L) return VKEY_L;
	if (scanCode == KEY_M) return VKEY_M;
	if (scanCode == KEY_N) return VKEY_N;
	if (scanCode == KEY_O) return VKEY_O;
	if (scanCode == KEY_P) return VKEY_P;
	if (scanCode == KEY_Q) return VKEY_Q;
	if (scanCode == KEY_R) return VKEY_R;
	if (scanCode == KEY_S) return VKEY_S;
	if (scanCode == KEY_T) return VKEY_T;
	if (scanCode == KEY_U) return VKEY_U;
	if (scanCode == KEY_V) return VKEY_V;
	if (scanCode == KEY_W) return VKEY_W;
	if (scanCode == KEY_X) return VKEY_X;
	if (scanCode == KEY_Y) return VKEY_Y;
	if (scanCode == KEY_Z) return VKEY_Z;

	if (scanCode == KEY_MINUS) return VKEY_MINUS;
	if (scanCode == KEY_EQUALS) return VKEY_EQUALS;
	if (scanCode == KEY_BRACKETOPEN) return VKEY_BRACKETOPEN;
	if (scanCode == KEY_BRACKETCLOSED) return VKEY_BRACKETCLOSED;
	if (scanCode == KEY_SEMICOLON) return VKEY_SEMICOLON;
	if (scanCode == KEY_INVCOMMA) return VKEY_INVCOMMA;
	if (scanCode == KEY_APHOSTROPHY) return VKEY_APHOSTROPHY;
	if (scanCode == KEY_COMMA) return VKEY_COMMA;
	if (scanCode == KEY_PERIOD) return VKEY_PERIOD;
	if (scanCode == KEY_SLASH) return VKEY_SLASH;
	if (scanCode == KEY_BACKSLASH) return VKEY_BACKSLASH;
	if (scanCode == KEY_SPACE) return VKEY_SPACE;

	if (scanCode == KEY_ENTER) return VKEY_ENTER;
	if (scanCode == KEY_ESCAPE) return VKEY_ESCAPE;
	if (scanCode == KEY_BACKSPACE) return VKEY_BACKSPACE;
	if (scanCode == KEY_LEFTSHIFT) return VKEY_LEFTSHIFT;
	if (scanCode == KEY_RIGHTSHIFT) return VKEY_RIGHTSHIFT;
	if (scanCode == KEY_CONTROL) return VKEY_CONTROL;
	if (scanCode == KEY_ALT) return VKEY_ALT;
	if (scanCode == KEY_CAPSLOCK) return VKEY_CAPSLOCK;
	if (scanCode == KEY_NUMLOCK) return VKEY_NUMLOCK;
	if (scanCode == KEY_SCROLLOCK) return VKEY_SCROLLOCK;
	if (scanCode == KEY_PRINTSCREEN) return VKEY_PRINTSCREEN;

	if (scanCode == KEY_NUMPAD1) return VKEY_NUMPAD1;
	if (scanCode == KEY_NUMPAD2) return VKEY_NUMPAD2;
	if (scanCode == KEY_NUMPAD3) return VKEY_NUMPAD3;
	if (scanCode == KEY_NUMPAD4) return VKEY_NUMPAD4;
	if (scanCode == KEY_NUMPAD5) return VKEY_NUMPAD5;
	if (scanCode == KEY_NUMPAD6) return VKEY_NUMPAD6;
	if (scanCode == KEY_NUMPAD7) return VKEY_NUMPAD7;
	if (scanCode == KEY_NUMPAD8) return VKEY_NUMPAD8;
	if (scanCode == KEY_NUMPAD9) return VKEY_NUMPAD9;
	if (scanCode == KEY_NUMPADMINUS) return VKEY_NUMPADMINUS;
	if (scanCode == KEY_NUMPADPLUS) return VKEY_NUMPADPLUS;
	if (scanCode == KEY_NUMPADINSERT) return VKEY_NUMPADINSERT;
	if (scanCode == KEY_NUMPADDELETE) return VKEY_NUMPADDELETE;

	if (scanCode == KEY_TAB) return VKEY_TAB;

	return VKEY_NULL;
}

UINT8 VirtualKeyToAscii(UINT8 virtualKey)
{
	if (isAltDown || isControlDown) return '\0';

	if (!isShiftDown)
	{
		if (virtualKey == VKEY_1) return '1';
		if (virtualKey == VKEY_2) return '2';
		if (virtualKey == VKEY_3) return '3';
		if (virtualKey == VKEY_4) return '4';
		if (virtualKey == VKEY_5) return '5';
		if (virtualKey == VKEY_6) return '6';
		if (virtualKey == VKEY_7) return '7';
		if (virtualKey == VKEY_8) return '8';
		if (virtualKey == VKEY_9) return '9';
		if (virtualKey == VKEY_0) return '0';

		if (virtualKey == VKEY_A) return 'a';
		if (virtualKey == VKEY_B) return 'b';
		if (virtualKey == VKEY_C) return 'c';
		if (virtualKey == VKEY_D) return 'd';
		if (virtualKey == VKEY_E) return 'e';
		if (virtualKey == VKEY_F) return 'f';
		if (virtualKey == VKEY_G) return 'g';
		if (virtualKey == VKEY_H) return 'h';
		if (virtualKey == VKEY_I) return 'i';
		if (virtualKey == VKEY_J) return 'j';
		if (virtualKey == VKEY_K) return 'k';
		if (virtualKey == VKEY_L) return 'l';
		if (virtualKey == VKEY_M) return 'm';
		if (virtualKey == VKEY_N) return 'n';
		if (virtualKey == VKEY_O) return 'o';
		if (virtualKey == VKEY_P) return 'p';
		if (virtualKey == VKEY_Q) return 'q';
		if (virtualKey == VKEY_R) return 'r';
		if (virtualKey == VKEY_S) return 's';
		if (virtualKey == VKEY_T) return 't';
		if (virtualKey == VKEY_U) return 'u';
		if (virtualKey == VKEY_V) return 'v';
		if (virtualKey == VKEY_W) return 'w';
		if (virtualKey == VKEY_X) return 'x';
		if (virtualKey == VKEY_Y) return 'y';
		if (virtualKey == VKEY_Z) return 'z';

		if (virtualKey == VKEY_APHOSTROPHY) return '`';
		if (virtualKey == VKEY_MINUS) return '-';
		if (virtualKey == VKEY_EQUALS) return '=';
		if (virtualKey == VKEY_BRACKETOPEN) return '[';
		if (virtualKey == VKEY_BRACKETCLOSED) return ']';
		if (virtualKey == VKEY_SEMICOLON) return ';';
		if (virtualKey == VKEY_INVCOMMA) return '.';
		if (virtualKey == VKEY_COMMA) return ',';
		if (virtualKey == VKEY_PERIOD) return '.';
		if (virtualKey == VKEY_SLASH) return '/';
		if (virtualKey == VKEY_BACKSLASH) return '\\';
		if (virtualKey == VKEY_SPACE) return ' ';


		if (virtualKey == VKEY_NUMPADMINUS) return '-';
		if (virtualKey == VKEY_NUMPADPLUS) return '+';

		// if (isNumLockDown)
		// {
		// 	if (virtualKey == VKEY_NUMPAD1) return '1';
		// 	if (virtualKey == VKEY_NUMPAD2) return '2';
		// 	if (virtualKey == VKEY_NUMPAD3) return '3';
		// 	if (virtualKey == VKEY_NUMPAD4) return '4';
		// 	if (virtualKey == VKEY_NUMPAD5) return '5';
		// 	if (virtualKey == VKEY_NUMPAD6) return '6';
		// 	if (virtualKey == VKEY_NUMPAD7) return '7';
		// 	if (virtualKey == VKEY_NUMPAD8) return '8';
		// 	if (virtualKey == VKEY_NUMPAD9) return '9';
		// 	if (virtualKey == KEY_NUMPADINSERT) return '0';
		// 	if (virtualKey == KEY_NUMPADDELETE) return '.';
		// }
		// else
		// {
		// 	if (virtualKey == VKEY_NUMPAD2) return 'D';
		// 	if (virtualKey == VKEY_NUMPAD4) return 'L';
		// 	if (virtualKey == VKEY_NUMPAD6) return 'R';
		// 	if (virtualKey == VKEY_NUMPAD8) return 'U';
		// }

		return '\0';
	}

	if (virtualKey == VKEY_1) return '!';
	if (virtualKey == VKEY_2) return '@';
	if (virtualKey == VKEY_3) return '#';
	if (virtualKey == VKEY_4) return '$';
	if (virtualKey == VKEY_5) return '%';
	if (virtualKey == VKEY_6) return '^';
	if (virtualKey == VKEY_7) return '&';
	if (virtualKey == VKEY_8) return '*';
	if (virtualKey == VKEY_9) return '(';
	if (virtualKey == VKEY_0) return ')';

	if (virtualKey == VKEY_A) return 'A';
	if (virtualKey == VKEY_B) return 'B';
	if (virtualKey == VKEY_C) return 'C';
	if (virtualKey == VKEY_D) return 'D';
	if (virtualKey == VKEY_E) return 'E';
	if (virtualKey == VKEY_F) return 'F';
	if (virtualKey == VKEY_G) return 'G';
	if (virtualKey == VKEY_H) return 'H';
	if (virtualKey == VKEY_I) return 'I';
	if (virtualKey == VKEY_J) return 'J';
	if (virtualKey == VKEY_K) return 'K';
	if (virtualKey == VKEY_L) return 'L';
	if (virtualKey == VKEY_M) return 'M';
	if (virtualKey == VKEY_N) return 'N';
	if (virtualKey == VKEY_O) return 'O';
	if (virtualKey == VKEY_P) return 'P';
	if (virtualKey == VKEY_Q) return 'Q';
	if (virtualKey == VKEY_R) return 'R';
	if (virtualKey == VKEY_S) return 'S';
	if (virtualKey == VKEY_T) return 'T';
	if (virtualKey == VKEY_U) return 'U';
	if (virtualKey == VKEY_V) return 'V';
	if (virtualKey == VKEY_W) return 'W';
	if (virtualKey == VKEY_X) return 'X';
	if (virtualKey == VKEY_Y) return 'Y';
	if (virtualKey == VKEY_Z) return 'Z';

	if (virtualKey == VKEY_APHOSTROPHY) return '~';
	if (virtualKey == VKEY_MINUS) return '_';
	if (virtualKey == VKEY_EQUALS) return '+';
	if (virtualKey == VKEY_BRACKETOPEN) return '{';
	if (virtualKey == VKEY_BRACKETCLOSED) return '}';
	if (virtualKey == VKEY_SEMICOLON) return ':';
	if (virtualKey == VKEY_INVCOMMA) return '"';
	if (virtualKey == VKEY_COMMA) return '<';
	if (virtualKey == VKEY_PERIOD) return '>';
	if (virtualKey == VKEY_SLASH) return '?';
	if (virtualKey == VKEY_BACKSLASH) return '|';
	if (virtualKey == VKEY_SPACE) return ' ';

	if (virtualKey == VKEY_NUMPADMINUS) return '-';
	if (virtualKey == VKEY_NUMPADPLUS) return '+';

	// if (virtualKey == VKEY_NUMPAD1) return '1';
	// if (virtualKey == VKEY_NUMPAD2) return '2';
	// if (virtualKey == VKEY_NUMPAD3) return '3';
	// if (virtualKey == VKEY_NUMPAD4) return '4';
	// if (virtualKey == VKEY_NUMPAD5) return '5';
	// if (virtualKey == VKEY_NUMPAD6) return '6';
	// if (virtualKey == VKEY_NUMPAD7) return '7';
	// if (virtualKey == VKEY_NUMPAD8) return '8';
	// if (virtualKey == VKEY_NUMPAD9) return '9';
	// if (virtualKey == KEY_NUMPADINSERT) return '0';
	// if (virtualKey == KEY_NUMPADDELETE) return '.';

	return '\0';
}

// static char* _qwertzuiop = "qwertzuiop"; // 0x10-0x1c
// static char* _asdfghjkl = "asdfghjkl";
// static char* _yxcvbnm = "yxcvbnm";
// static char* _num = "123456789";
// uint8_t keyboard_to_ascii(uint8_t key)
// {
//     if (key & 0x80)
//     {
//         return 0;
//     }

//     //key &= ~ 0x80;

// 	//kprintf("key=0x%x\n", key);
// 	if(key == 0x1C) return '\n';
// 	if(key == 0x39) return ' ';
// 	if(key == 0xE) return '\r';
// 	if(key == POINT_RELEASED) return '.';
// 	if(key == SLASH_RELEASED) return '/';
// 	if(key == ZERO_PRESSED) return '0';
// 	if(key >= ONE_PRESSED && key <= NINE_PRESSED)
// 		return _num[key - ONE_PRESSED];
// 	if(key >= 0x10 && key <= 0x1C)
// 	{
// 		return _qwertzuiop[key - 0x10];
// 	} else if(key >= 0x1E && key <= 0x26)
// 	{
// 		return _asdfghjkl[key - 0x1E];
// 	} else if(key >= 0x2C && key <= 0x32)
// 	{
// 		return _yxcvbnm[key - 0x2C];
// 	}
// 	return 0;
// }



// KSTATUS KeyboardInit()
// {
//     UINT8 configByte;
//     UINT8 selfTestResult;

//     KeyboardWriteCommand(KEYBOARD_COMMAND_DISABLE_KEYBOARD);
//     KeyboardWriteCommand(KEYBOARD_COMMAND_DISABLE_AUXILARY);

//     // Discard leftover keystrokes
//     KeyboardScan(NULL, NULL);

//     // Get current configration
//     KeyboardWriteCommand(KEYBOARD_COMMAND_READ_CONFIGURATION_BYTE);
//     configByte = KeyboardReadData();

//     // Execute controller self-test
//     KeyboardWriteCommand(KEYBOARD_COMMAND_EXECUTE_SELF_TEXT);
//     selfTestResult = KeyboardReadData();

//     if (selfTestResult != KEYBOARD_SELF_TEST_SUCCESS)
//     {
//         return STATUS_ERROR_UNKNOWN;
//     }

//     // Enable interrupt sending
//     configByte |= 0x1;
//     // Activate IRQ for AUX
//     //configByte |= 0x2;  ???
//     KeyboardWriteCommand2(KEYBOARD_COMMAND_WRITE_CONFIGURATION_BYTE, configByte);

//     // Re-enable keyboard device
//     KeyboardWriteCommand(KEYBOARD_COMMAND_ENABLE_KEYBOARD);

//     KeyboardWriteCommand(KEYBOARD_COMMAND_ENABLE_AUXILARY);
//     KeyboardWriteCommand2(0xD4, 0xF6);
//     KeyboardWriteCommand2(0xD4, 0xF4);

//     // Set LED state
//     KeyboardWait();
//     KeyboardSetLEDs(0);

//     return STATUS_SUCCESS;
// }

// VOID KeyboardWriteCommand(UINT32 command)
// {
//     UINT32 result;

//     KeyboardWait();

//     MachineIoWritePortUint8(KEYBOARD_COMMAND_PORT, command);
// }

// VOID KeyboardWriteCommand2(UINT32 command, UINT32 data)
// {
//     UINT32 result;

//     KeyboardWait();

//     MachineIoWritePortUint8(KEYBOARD_COMMAND_PORT, command);
//     MachineIoWritePortUint8(KEYBOARD_DATA_PORT, data);
// }

// UINT32 KeyboardReadData()
// {
//     UINT8 byte = 0;
//     UINT32 status;

//     status = MachineIoReadPortUint8(KEYBOARD_STATUS_PORT);

//     if (status & KB_OUT_FULL)    // ???
//     {
//         MachineIoDelay();

//         byte = MachineIoReadPortUint8(KEYBOARD_DATA_PORT);
//     }

//     return byte;
// }

// BOOL KeyboardWait()
// {
//     UINT32 status;
//     UINT8 byte = 0;
//     BOOL isAux ;

//     MachineIoDelay();

//     status = MachineIoReadPortUint8(KEYBOARD_STATUS_PORT);

//     if (status & KB_OUT_FULL)    // ???
//     {
//         KeyboardScan(&byte, &isAux);
//     }

//     if (!(status & (KB_IN_FULL | KB_OUT_FULL)))
//     {
//         return TRUE;
//     }

//     return FALSE;
// }

// VOID KeyboardScan(PUINT8 pByte, PBOOL pIsAuxilaryPort)
// {
//     UINT32 byte;
//     UINT32 status;

//     status = MachineIoReadPortUint8(KEYBOARD_STATUS_PORT);
//     byte = MachineIoReadPortUint8(KEYBOARD_DATA_PORT);

//     if (pByte != NULL)
//     {
//         *pByte = byte;
//     }

//     if (pIsAuxilaryPort != NULL)
//     {
//         *pIsAuxilaryPort = !(status & KEYBOARD_AUXILARY_BYTE);
//     }
// }

// VOID KeyboardSetLEDs(UINT32 ledMask)
// {
// 	// if (kbdout.avail == 0)
// 	// 	kbdout.offset = 0;
// 	// if (kbdout.offset + kbdout.avail + 2 > KBD_OUT_BUFSZ) {
// 	// 	/*
// 	// 	 * The output buffer is full.  Ignore this command.  Reset the
// 	// 	 * ACK flag.
// 	// 	 */
// 	// 	kbdout.expect_ack = 0;
// 	// } else {
// 	// 	kbdout.buf[kbdout.offset+kbdout.avail] = LED_CODE;
// 	// 	kbdout.buf[kbdout.offset+kbdout.avail+1] = ledmask;
// 	// 	kbdout.avail += 2;
// 	// }
// 	// if (!kbdout.expect_ack)
// 	// 	kbd_send();
// }

