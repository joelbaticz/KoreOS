/*
  - Date:		2018-02-20
  - Author:		Joel
  - Purpose:	Loader debug implementation:
				Using the debug part of command-line options or using the options
				supplied via the BOOT.INI debug levels are set for different debug channels.
				Debug also provides functionality for displaying Tracing, Warning and Error
				information on the set debug ports.
*/

#include <kldr.h>

#define DEBUG_LEVEL_ALL DEBUG_LEVEL_TRACE | DEBUG_LEVEL_WARNING | DEBUG_LEVEL_ERROR
INT8 DebugChannels[DEBUG_CHANNELS_COUNT];
INT8 DebugPorts;

UINT8 DebugCursorX;
UINT8 DebugCursorY;

INT8 DebugSerialConnectionComPort;
INT8 DebugSerialConnectionBaudRate;
INT8 DebugSerialConnectionIrq;

VOID DebugParseOptions(char* pDebugOptions);
VOID SetDebugPorts(INT8 NewDebugPorts);
VOID SetDebugLevelForAllChannels(INT8 NewDebugLevel); 

VOID DebugInit(DEBUG_SETTINGS debugSettings)
{
	DebugPorts = DEBUG_PORT_DEFAULT;
	CHAR DebugOptions[256];
	PCHAR pDebugOptions = &DebugOptions[0];
	DebugCursorX = 0;
	DebugCursorY = 0;
	
	//UiGetCursorPosition(&DebugCursorX, &DebugCursorY);

	/* Set defaults */
	SetDebugPorts(DEBUG_PORT_SCREEN);
	
	#ifdef KORE_BUILD_DEBUG_LEVEL_ALL
		SetDebugLevelForAllChannels(DEBUG_LEVEL_TRACE | DEBUG_LEVEL_WARNING | DEBUG_LEVEL_ERROR);	
	#endif
	#ifdef KORE_BUILD_DEBUG_LEVEL_WARNING
		SetDebugLevelForAllChannels(DEBUG_LEVEL_WARNING | DEBUG_LEVEL_ERROR);	
	#endif
	#ifdef KORE_BUILD_DEBUG_LEVEL_ERROR
		SetDebugLevelForAllChannels(DEBUG_LEVEL_ERROR);	
	#endif
	
	if (debugSettings & DEBUG_SETTING_INI_FILE == DEBUG_SETTING_INI_FILE)
	{
		/* Process Debug Options came through the BOOT.INI file */
		NOT_IMPLEMENTED;
		
		PCHAR pIniSection;		
		//if (!IniFileOpenSection("KoreOS", pIniSection)) return;
		//if (!IniFileReadSectionByName(pIniSection, "DEBUG"), DebugOpions, sizeof(DebugOptions)) return;

		return;
	}

	if (debugSettings & DEBUG_SETTING_CMD_LINE == DEBUG_SETTING_CMD_LINE)
	{
		/* Debug Options came through Command-line instead*/	
		pDebugOptions = GetCommandLineOptions();
		if (pDebugOptions != NULL)
		{
			/* FIX ME! Not sure why, but we come here and since DebugHalt is implemented it hangs here */
			DebugParseOptions(pDebugOptions);
		}	
	}

	UiDrawText("DebugInit....................DONE");
}

VOID DebugParseOptions(char* pDebugOptions)
{
	NOT_IMPLEMENTED;
}

VOID SetDebugPorts(INT8 NewDebugPorts)
{
	DebugPorts = NewDebugPorts;

	// TODO: Should setup Bochs and Serial debug ports, baudrate, irq
}

VOID SetDebugLevelForChannel(INT8 DebugChannelNumber, INT8 NewDebugLevel)
{
	DebugChannels[DebugChannelNumber] = NewDebugLevel;
}

VOID SetDebugLevelForAllChannels(INT8 NewDebugLevel)
{
	for(size_t i = 0; i < DEBUG_CHANNELS_COUNT; i++)
	{
		SetDebugLevelForChannel(i, NewDebugLevel);
	}
}

VOID DebugPutChar(CHAR ch)
{
	if (DebugPorts & DEBUG_PORT_SCREEN)
	{
		UiPutChar(ch);

		DebugCursorX++;
		if (DebugCursorX >= 80)
		{
			DebugCursorX = 0;
			DebugCursorY++;
		}

		UiSetCursorPosition(DebugCursorX, DebugCursorY);
	}
	if (DebugPorts & DEBUG_PORT_SERIAL)
	{
		// Needs init!
		// SerialPutChar(ch);
	}
	if (DebugPorts & DEBUG_PORT_BOSCH)
	{
		// Needs init!
		// BoschPutChar(ch);
	}	
}

VOID DebugPrintF(const char* str)
{
	UiDrawText((PCHAR)str);
	// UiGetCursorPosition(&DebugCursorX, &DebugCursorY);

	// if (str)
	// {
	// 	while(str[0] != '\0')
	// 	{
	// 		DebugPutChar(str[0]);
	// 		str++;
	// 	}
	// }

	// DebugCursorX = 0;
	// DebugCursorY++;
	// UiSetCursorPosition(DebugCursorX, DebugCursorY);
}

VOID DebugPrintf(const char* Format, ...)
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
	
	DebugPrintF(pBuffer);
}
	
	
VOID DebugPrintfWithDebugInfo(UINT32 DebugChannel, UINT32 DebugLevel, const char *File, UINT32 Line, const char *Format, ...)
{
	va_list ap;
	UINT32 length;
	
	CHAR buffer[256] = {0};
	PCHAR pBuffer = (char*) buffer;
	CHAR buffer2[32] = {0};
	PCHAR pBuffer2 = (char*) buffer2;
	
	/* Handle Debug Levels */
	if (DebugLevel == DEBUG_LEVEL_ERROR) strcpy(buffer, "ERROR ");
	else if (DebugLevel == DEBUG_LEVEL_WARNING) strcpy(buffer, "WARNING ");
	else if (DebugLevel == DEBUG_LEVEL_TRACE) strcpy(buffer, "TRACE ");
	
	/* Handle Debug Channels */
	if (DebugChannel == DEBUG_CHANNEL_NONE) strcat(buffer, "[None]"); 
	else if (DebugChannel == DEBUG_CHANNEL_UI) strcat(buffer, "[UI]");
	else if (DebugChannel == DEBUG_CHANNEL_MEMORY) strcat(buffer, "[Memory]");
	else if (DebugChannel == DEBUG_CHANNEL_FILESYSTEM) strcat(buffer, "[FileSystem]");
	else if (DebugChannel == DEBUG_CHANNEL_DISK) strcat(buffer, "[Disk]");
	else if (DebugChannel == DEBUG_CHANNEL_CACHE) strcat(buffer, "[Cache]");
	
	strcat(buffer, " - \"");
	
	strcat(buffer, File);
	
	strcat(buffer, "\" (");
	
	strcat(buffer, itoa(Line, pBuffer2, 10));
	
	strcat(buffer, ") ");
	
	length = strlen(buffer);
	
	va_start(ap, Format);
	length = vsnprintf(buffer + length, sizeof(buffer) - length, Format, ap);
	va_end(ap);
	
	DebugPrintF(pBuffer);
}

VOID DebugPause()
{
	MachineUtilityPause();
}

VOID DebugHalt()
{
	MachineDisableInterrupts();
	MachineUtilityHalt();
}

VOID DebugBreakPoint()
{
	// Should implement xor ebx,ebx or what is the breakpoint in Bochs
	NOT_IMPLEMENTED;
}

VOID DebugDumpBuffer(PCHAR pBuffer)
{
   // Should be able to dump a buffer (like memory descriptions)
   	NOT_IMPLEMENTED;
}

	
