/*
  - Date: 2018-03-10
  - Author: Joel
  - Purpose: Machine and Arch init definitions
*/

/* INCLUDES */
#include <kldr.h>

#ifndef __MACHINE_H
#define __MACHINE_H

/* Machine specific function table */
typedef struct tagMACHINE_VTBL
{	
	VOID		(*IoWritePortUint8)(UINT16 port, UINT8 data);
	UINT8		(*IoReadPortUint8)(UINT16 port);
	VOID		(*IoWritePortUint16)(UINT16 port, UINT16 data);
	UINT16		(*IoReadPortUint16)(UINT16 port);
	VOID		(*IoDelay)();
	VOID		(*IoDisableInterrupts)();
	VOID		(*IoEnableInterrupts)();
	VOID		(*IoAcknowledgeInterrupt)(UINT32 interruptNumber);
	VOID		(*IoInitializeInterruptVectorTable)(PISR_VTBL pIsrFunctions);
	
	VOID		(*ConsoleWaitForKeyPress)();
	CHAR		(*ConsoleGetChar)();
	VOID		(*ConsolePutChar)(CHAR ch);
	UINT8		(*ConsoleDetectBestDisplayMode)();
	VOID		(*ConsoleSetDisplayMode)(UINT8 requestedDisplayMode);
	VOID		(*ConsoleShowCursor)(BOOL doShowCursor);
	VOID		(*ConsoleGetCursorPosition)(PUINT8 pX, PUINT8 pY);
	VOID		(*ConsoleSetCursorPosition)(UINT8 x, UINT8 y);
		
	VOID		(*UtilityBeep)();
	UINT32		(*UtilityGetDateTime)();
	VOID		(*UtilitySetDateTime)(UINT32 dateTime);
	VOID		(*UtilitySetStatusLed)(UINT8 ledId);
	BOOL        (*UtilityCheckProcessorCompatibility)();
	VOID		(*UtilityDelay)(UINT32 milliSecs);
	VOID		(*UtilityPause)();
	VOID		(*UtilityHalt)();
	VOID		(*UtilityReboot)();
	
	BOOL		(*MemoryGetMemoryMap)(OUT PMEMORY_MAP_ENTRY pMemoryMap, OUT PUINT32 pMemoryMapEntryCount);
	UINT32		(*MemoryGetPageSize)();
	PUINT32		(*MemoryGetFreeListBase)();
	PUINT32		(*MemoryGetUsedListBase)();
	UINT32		(*MemoryGetFreeListSize)();
	UINT32		(*MemoryGetUsedListSize)();

} __attribute__((packed)) MACHINE_VTBL, *PMACHINE_VTBL;

/* GLOBAL VARIABLES */
MACHINE_VTBL MachineVtbl;

/* GLOBAL FUNCTIONS */
BOOL MachineInit();

/*
	These functions will call the machine specific versions which are
	implemented in *io.c, *console.c, *utility.c, *memory.c, *disk.c files respectively
*/

#define outportb(port, data)        									MachineIoWritePortUint8(port, data)
#define inportb(port)               									MachineIoReadPortUint8(port)

// #define outportb(port, data)        									MachineVtbl.IoWritePortUint8(port, data)
// #define inportb(port)               									MachineVtbl.IoReadPortUint8(port)

#define MachineIoWritePortUint8(port, data)								MachineVtbl.IoWritePortUint8(port, data)
#define MachineIoReadPortUint8(port)									MachineVtbl.IoReadPortUint8(port)
#define MachineIoWritePortUint16(port, data)							MachineVtbl.IoWritePortUint16(port, data)
#define MachineIoDelay()												MachineVtbl.IoDelay()
#define MachineIoReadPortUint16(port)									MachineVtbl.IoReadPortUint16(port)
#define MachineDisableInterrupts()										MachineVtbl.IoDisableInterrupts()
#define MachineEnableInterrupts()										MachineVtbl.IoEnableInterrupts()
#define MachineAcknowledgeInterrupt(interruptNumber)					MachineVtbl.IoAcknowledgeInterrupt(interruptNumber)
#define MachineInitializeInterruptVectorTable(pIsrFunctions)			MachineVtbl.IoInitializeInterruptVectorTable(pIsrFunctions)

#define MachineConsoleWaitForKeyPress()									MachineVtbl.ConsoleWaitForKeyPress()
#define MachineConsoleGetChar()											MachineVtbl.ConsoleGetChar()
#define MachineConsolePutChar(ch)										MachineVtbl.ConsolePutChar(ch)
#define MachineConsoleDetectBestDisplayMode()							MachineVtbl.ConsoleDetectBestDisplayMode()
#define MachineConsoleSetDisplayMode(requestedDisplayMode)				MachineVtbl.ConsoleSetDisplayMode(requestedDisplayMode)
#define MachineConsoleShowCursor(doShowCursor)							MachineVtbl.ConsoleShowCursor(doShowCursor)
#define MachineConsoleGetCursorPosition(pX, pY)							MachineVtbl.ConsoleGetCursorPosition(pX, pY)
#define MachineConsoleSetCursorPosition(x, y)							MachineVtbl.ConsoleSetCursorPosition(x, y)

#define MachineUtilityBeep()											MachineVtbl.UtilityBeep()
#define MachineUtilityGetDateTime()										MachineVtbl.UtilityGetDateTime()
#define MachineUtilitySetDateTime(dateTime)								MachineVtbl.UtilitySetDateTime(dateTime)
#define MachineUtilitySetStatusLed(ledId)								MachineVtbl.UtilitySetStatusLed(ledId)
#define MachineUtilityGetBiosDate()										MachineVtbl.UtilityGetBiosDate()
#define MachineUtilityCheckProcessorCompatibility()						MachineVtbl.UtilityCheckProcessorCompatibility()
#define MachineUtilityDelay(milliSecs)									MachineVtbl.UtilityDelay(milliSecs)
#define MachineUtilityPause()											MachineVtbl.UtilityPause()
#define MachineUtilityHalt()											MachineVtbl.UtilityHalt()
#define MachineUtilityReboot()											MachineVtbl.UtilityReboot()

#define MachineMemoryGetMemoryMap(pMemoryMap, pMemoryMapEntryCount)		MachineVtbl.MemoryGetMemoryMap(pMemoryMap, pMemoryMapEntryCount)
#define MachineMemoryGetPageSize()										MachineVtbl.MemoryGetPageSize()
#define MachineMemoryGetFreeListBase()									MachineVtbl.MemoryGetFreeListBase()
#define MachineMemoryGetUsedListBase()									MachineVtbl.MemoryGetUsedListBase()
#define MachineMemoryGetFreeListSize()									MachineVtbl.MemoryGetFreeListSize()
#define MachineMemoryGetUsedListSize()									MachineVtbl.MemoryGetUsedListSize()

#endif /* __MACHINE_H */