/*
  - Date:		2018-02-20
  - Author:		Joel
  - Purpose:	Loader command line options implementation:
				The loader receives a pointer to the command line options.
				The different options will be cut and placed in a different string.
*/

/* INCLUDES */
#include <kldr.h>
#include <crt/string.h>

/* DEFINITIONS */

/* GLOBALS */
typedef struct _CommandLineInfoBlock
{
	PCHAR pDebugOptions;
} COMMAND_LINE_INFO_BLOCK, *PCOMMAND_LINE_INFO_BLOCK;

COMMAND_LINE_INFO_BLOCK CommandLineInfoBlock;
CHAR DebugOptions[256];

/* FUNCTIONS */
VOID CommandLineOptionsParse(PCHAR pCommandLineOptions)
{
	size_t OptionStringLength;
	PCHAR pDebugOption;
	PCHAR pCurrentOptionEnd;
	
	CommandLineInfoBlock.pDebugOptions = NULL;
	
	pDebugOption = strstr(pCommandLineOptions, "DEBUG=");
	
	if (pDebugOption)
	{
		pDebugOption += strlen("DEBUG=\0");
		pCurrentOptionEnd = strstr(pDebugOption, " ");

		if (pCurrentOptionEnd)
		{
			OptionStringLength = pCurrentOptionEnd - pDebugOption;
		}
		else
		{
			OptionStringLength = strlen(pDebugOption); //sizeof(DebugOptions);
		}
		
		strncpy(DebugOptions, pDebugOption, OptionStringLength);
		DebugOptions[OptionStringLength] = '\0';
		strupr(DebugOptions);
		
		CommandLineInfoBlock.pDebugOptions = (PCHAR) &DebugOptions;
	}	
}

char* GetCommandLineOptions()
{
	return CommandLineInfoBlock.pDebugOptions;
}