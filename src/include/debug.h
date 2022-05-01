/*
  - Date: 2018-02-20
  - Author: Joel
  - Purpose: Debugging definitions
*/

#ifndef __DEBUG_H
#define __DEBUG_H

/* DEFINITIONS */
typedef UINT32 DEBUG_SETTINGS;

#define DEBUG_CHANNEL_NONE				      0 	/* No specific debug channel */
#define DEBUG_CHANNEL_UI			        	1
#define DEBUG_CHANNEL_MEMORY		      	2
#define DEBUG_CHANNEL_FILESYSTEM      	3
#define DEBUG_CHANNEL_DISK			      	4
#define DEBUG_CHANNEL_CACHE			      	5

#define DEBUG_CHANNELS_COUNT	        	6
#define DEBUG_CHANNEL_DEFAULT		      	DEBUG_CHANNEL_NONE

#define DEBUG_LEVEL_ERROR			         	1
#define DEBUG_LEVEL_WARNING				      2
#define DEBUG_LEVEL_TRACE		      	  	4

#define DEBUG_PORT_SCREEN			        	1
#define DEBUG_PORT_SERIAL	      		  	2
#define DEBUG_PORT_BOSCH		  	      	4

#define DEBUG_PORT_DEFAULT		      		DEBUG_PORT_SCREEN

#define DEBUG_SERIAL_DEFAULT_BAUD_RATE	115200
#define DEBUG_SERIAL_DEFAULT_COM_PORT	  0
#define DEBUG_SERIAL_DEFAULT_IRQ	    	0

#define DEBUG_SETTING_NONE             0
#define DEBUG_SETTING_INI_FILE         1
#define DEBUG_SETTING_CMD_LINE         2

#define ERROR(Format, ...)				DebugPrintfWithDebugInfo(DEBUG_CHANNEL_DEFAULT, DEBUG_LEVEL_ERROR, __FILE__, __LINE__, Format, ##__VA_ARGS__)
#define WARNING(Format, ...)			DebugPrintfWithDebugInfo(DEBUG_CHANNEL_DEFAULT, DEBUG_LEVEL_WARNING, __FILE__, __LINE__, Format, ##__VA_ARGS__)
#define TRACE(Format, ...)				DebugPrintfWithDebugInfo(DEBUG_CHANNEL_DEFAULT, DEBUG_LEVEL_TRACE, __FILE__, __LINE__, Format, ##__VA_ARGS__)

#define ERROR_WITH_CHANNEL(DebugChannel, Format, ...)	DebugPrintfWithDebugInfo(DebugChannel, DEBUG_LEVEL_ERROR, __FILE__, __LINE__, Format, ##__VA_ARGS__)
#define WARNING_WITH_CHANNEL(DebugChannel, Format, ...)	DebugPrintfWithDebugInfo(DebugChannel, DEBUG_LEVEL_WARNING, __FILE__, __LINE__, Format, ##__VA_ARGS__)
#define TRACE_WITH_CHANNEL(DebugChannel, Format, ...)	DebugPrintfWithDebugInfo(DebugChannel, DEBUG_LEVEL_TRACE, __FILE__, __LINE__, Format, ##__VA_ARGS__)

#define PAUSE						        	DebugPause()
#define HALT					        		DebugHalt()

#define NOT_IMPLEMENTED				  	DebugPrintf("WARNING: [%s : %d] %s is not implemented!\n", __FILE__, __LINE__, __FUNCTION__); //DebugHalt()

/* FUNCTIONS DEFINITIONS */
VOID DebugInit(DEBUG_SETTINGS ProcessIniFileSettings);
VOID DebugPrintF(const char* str);
VOID DebugPrintf(const char *Format, ...);
VOID DebugPrintfWithDebugInfo(UINT32 DebugChannel, UINT32 DebugLevel, const char *File, UINT32 Line, const char *Format, ...);
VOID DebugPause();
VOID DebugHalt();
VOID DebugBreakPoint();
VOID DebugDumpBuffer(PCHAR pBuffer);

#endif /* __DEBUG_H */