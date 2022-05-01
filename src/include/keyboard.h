/*
  - Date: 2022-02-02
  - Author: Joel
  - Purpose: PS/2 keyboard driver
*/

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

enum VKEYS
{
	VKEY_NULL		= 0x00,

	VKEY_0	    	= 0x30,
	VKEY_1      	= 0x31,
	VKEY_2      	= 0x32,
	VKEY_3      	= 0x33,
	VKEY_4      	= 0x34,
	VKEY_5      	= 0x35,
	VKEY_6      	= 0x36,
	VKEY_7      	= 0x37,
	VKEY_8      	= 0x38,
	VKEY_9      	= 0x39,

	VKEY_A      	= 0x41,
	VKEY_B      	= 0x42,
	VKEY_C      	= 0x43,
	VKEY_D      	= 0x44,
	VKEY_E      	= 0x45,
	VKEY_F      	= 0x46,
	VKEY_G      	= 0x47,
	VKEY_H      	= 0x48,
	VKEY_I      	= 0x49,
	VKEY_J      	= 0x4A,
	VKEY_K      	= 0x4B,
	VKEY_L      	= 0x4C,
	VKEY_M      	= 0x4D,
	VKEY_N      	= 0x4E,
	VKEY_O      	= 0x4F,
	VKEY_P      	= 0x50,
	VKEY_Q      	= 0x51,
	VKEY_R      	= 0x52,
	VKEY_S      	= 0x53,
	VKEY_T      	= 0x54,
	VKEY_U      	= 0x55,
	VKEY_V      	= 0x56,
	VKEY_W      	= 0x57,
	VKEY_X      	= 0x58,
	VKEY_Y      	= 0x59,
	VKEY_Z      	= 0x5A,

	VKEY_MINUS   	= 0x60,
	VKEY_EQUALS  	= 0x61,
	VKEY_BRACKETOPEN = 0x62,
	VKEY_BRACKETCLOSED = 0x63,
	VKEY_SEMICOLON   = 0x64,
	VKEY_INVCOMMA    = 0x65,
	VKEY_APHOSTROPHY = 0x66,
	VKEY_COMMA       = 0x67,
	VKEY_PERIOD      = 0x68,
	VKEY_SLASH       = 0x69,
	VKEY_BACKSLASH   = 0x70,
	VKEY_SPACE       = 0x71,

	VKEY_ENTER       = 0x80,
	VKEY_ESCAPE  	= 0x81,
	VKEY_BACKSPACE	= 0x82,
	VKEY_LEFTSHIFT   = 0x83,
	VKEY_RIGHTSHIFT  = 0x84,
	VKEY_CONTROL     = 0x85,
	VKEY_ALT         = 0x86,
	VKEY_CAPSLOCK    = 0x87,
	VKEY_NUMLOCK     = 0x88,
	VKEY_SCROLLOCK   = 0x89,
	VKEY_PRINTSCREEN = 0x8A,

	VKEY_NUMPAD1     = 0x8B,
	VKEY_NUMPAD2     = 0x8C,
	VKEY_NUMPAD3     = 0x8D,
	VKEY_NUMPAD4     = 0x8E,
	VKEY_NUMPAD5     = 0x8F,
	VKEY_NUMPAD6     = 0x90,
	VKEY_NUMPAD7     = 0x91,
	VKEY_NUMPAD8     = 0x92,
	VKEY_NUMPAD9     = 0x93,
	VKEY_NUMPADMINUS = 0x94,
	VKEY_NUMPADPLUS  = 0x95,
	VKEY_NUMPADINSERT = 0x96,
	VKEY_NUMPADDELETE = 0x97,

	VKEY_TAB		  = 0x98,
	// Missing VKEY codes:
	// VKEY_F1
	// ...
	// VKEY_F12
	// VK_LWIN
	// VK_RWIN
	// VK_INSERT
	// VK_DELETE
	// VK_UP
	// VK_RIGHT
	// VK_DOWN
	// VK_LEFT
	// VK_HOME
	// VK_END
	// VK_PAGEUP
	// VK_PAGEDOWN
	// VK_PAUSE
};

BOOL keyboardServiceInstalled;

VOID KeyboardDriverInit();
VOID KeyboardHandler();
VOID KeyboardProcessKey();

UINT8 VirtualKeyToAscii(UINT8 virtualKey);

#endif // __KEYBOARD_H