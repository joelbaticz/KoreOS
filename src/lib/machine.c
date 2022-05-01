/*
  - Date: 2018-03-10
  - Author: Joel
  - Purpose: Machine and Arch init implementation:
			 Based on the build script the loader chooses architecture/machine
			 specific initialization routines. MachPC and MachXBox contain
			 machine specific functions.
*/

#include <kldr.h>

BOOL MachineInit()
{
	#ifdef KORE_MACHINE_TYPE_X86_32_PC
		PcInit();
	#endif

	#ifdef KORE_MACHINE_TYPE_X86_32_XBOX
		DEBUG_NOT_IMPLEMENTED;
		//XBoxInit();
	#endif

	if (!MachineUtilityCheckProcessorCompatibility())	
	{
		ERROR("Compatible processor is not present.");
		HALT;
	};

	UiDrawText("MachineInit..................DONE");
}

