/*
  - Date: 2018-03-10
  - Author: Joel
  - Purpose: Machine and Arch init implementation:
			 Based on the build script the loader chooses architecture/machine
			 specific initialization routines. MachPC and MachXBox contain
			 machine specific functions.
*/

/* INCLUDES */
#include <kldr.h>

/* FUNCTIONS */
VOID PcInit()
{
	MachineVtbl.IoWritePortUint8 = (PVOID) PcIoWritePortUint8;
	MachineVtbl.IoReadPortUint8 = (PVOID) PcIoReadPortUint8;
	MachineVtbl.IoWritePortUint16 = (PVOID) PcIoWritePortUint16;
	MachineVtbl.IoReadPortUint16 = (PVOID) PcIoReadPortUint16;
	MachineVtbl.IoDelay = (PVOID) PcIoDelay;
	MachineVtbl.IoDisableInterrupts = (PVOID) PcIoDisableInterrupts;
	MachineVtbl.IoEnableInterrupts = (PVOID) PcIoEnableInterrupts;
	MachineVtbl.IoAcknowledgeInterrupt = (PVOID) PcIoAcknowledgeInterrupt;
	MachineVtbl.IoInitializeInterruptVectorTable = (PVOID) PcIoInitializeInterruptVectorTable;	
		
	MachineVtbl.ConsoleWaitForKeyPress = (PVOID) PcConsoleWaitForKeyPress;
	MachineVtbl.ConsoleGetChar = (PVOID) PcConsoleGetChar;
	MachineVtbl.ConsolePutChar = (PVOID) PcConsolePutChar;
	MachineVtbl.ConsoleDetectBestDisplayMode = (PVOID) PcConsoleDetectBestDisplayMode;
	MachineVtbl.ConsoleSetDisplayMode = (PVOID) PcConsoleSetDisplayMode;
	MachineVtbl.ConsoleShowCursor = (PVOID) PcConsoleShowCursor;
	MachineVtbl.ConsoleGetCursorPosition = (PVOID) PcConsoleGetCursorPosition;
	MachineVtbl.ConsoleSetCursorPosition = (PVOID) PcConsoleSetCursorPosition;
		
	MachineVtbl.UtilityBeep = (PVOID) PcUtilityBeep;
	MachineVtbl.UtilityGetDateTime = (PVOID) PcUtilityGetDateTime;
	MachineVtbl.UtilitySetDateTime = (PVOID) PcUtilitySetDateTime;
	MachineVtbl.UtilitySetStatusLed = (PVOID) PcUtilitySetStatusLed;
	MachineVtbl.UtilityCheckProcessorCompatibility = (PVOID) PcUtilityCheckProcessorCompatibility;
	MachineVtbl.UtilityDelay = (PVOID) PcUtilityDelay;
	MachineVtbl.UtilityPause = (PVOID) PcUtilityPause;
	MachineVtbl.UtilityHalt = (PVOID) PcUtilityHalt;
	MachineVtbl.UtilityReboot = (PVOID) PcUtilityReboot;

	MachineVtbl.MemoryGetMemoryMap = (PVOID) PcMemoryGetMemoryMap;
	MachineVtbl.MemoryGetPageSize = (PVOID) PcMemoryGetPageSize;
	MachineVtbl.MemoryGetFreeListBase = (PVOID) PcMemoryGetFreeListBase;
	MachineVtbl.MemoryGetUsedListBase = (PVOID) PcMemoryGetUsedListBase;
	MachineVtbl.MemoryGetFreeListSize = (PVOID) PcMemoryGetFreeListSize;
	MachineVtbl.MemoryGetUsedListSize = (PVOID) PcMemoryGetUsedListSize;
}


