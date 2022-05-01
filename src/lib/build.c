/*
  - Date: 2018-02-22
  - Author: Joel
  - Purpose: Store Build information in a data structure
*/

#include <kldr.h>

// /* Architecture Types */
// static PCHAR pArchTypeX8632 = "X86-32\0";
// static PCHAR pArchTypeX8664 = "X86-64\0";
// static PCHAR pArchTypeARM = "ARM\0";
// static PCHAR pArchTypeMIPS = "MIPS\0";
// static PCHAR pArchTypePOWERPC = "POWERPC\0";

// /* Machine Types */
// static PCHAR pMachineTypePC = "PC\0";
// static PCHAR pMachineTypeXBOX = "XBOX\0";
// static PCHAR pMachineTypeUnknown = "UNKNOWN\0";

// /* Build Types */
// static PCHAR pBuildTypeDEBUG = "Debug Build\0";
// static PCHAR pBuildTypeRELEASE = "Release Build\0";

// /* Debug Levels */
// static PCHAR pDebugLevelDEBUG = "Debug\0";
// static PCHAR pDebugLevelWARNING = "Warning\0";
// static PCHAR pDebugLevelTRACE = "Trace\0";

/* Architecture Types */
static CHAR ArchTypeX8632[]		 = "X86-32\0";
static CHAR ArchTypeX8664[]		 = "X86-64\0";
static CHAR ArchTypeARM[]			 = "ARM\0";
static CHAR ArchTypeMIPS[]			 = "MIPS\0";
static CHAR ArchTypePOWERPC[]		 = "POWERPC\0";

/* Machine Types */
static CHAR MachineTypePC[]		 = "PC\0";
static CHAR MachineTypeXBOX[]		 = "XBOX\0";
static CHAR MachineTypeUnknown[]	 = "UNKNOWN\0";

/* Build Types */
static CHAR BuildTypeDEBUG[]		 = "Debug Build\0";
static CHAR BuildTypeRELEASE[]		 = "Release Build\0";

/* Debug Levels */
static CHAR DebugLevelDEBUG[]		 = "Debug\0";
static CHAR DebugLevelWARNING[]	 = "Warning\0";
static CHAR DebugLevelTRACE[]		 = "Trace\0";

VOID __cdecl GetBuildInformation(OUT PBUILD_INFO_BLOCK pBuildInfoBlock)
{
	/* ARCH & MACHINE TYPE */
	#ifdef KORE_MACHINE_TYPE_X86_32_PC
		pBuildInfoBlock->Architecture = (PCHAR) &ArchTypeX8632[0];
		pBuildInfoBlock->Machine = (PCHAR) &MachineTypePC[0];
	#endif
	#ifdef KORE_MACHINE_TYPE_X86_64_PC
		pBuildInfoBlock->Architecture = (PCHAR) &ArchTypeX8664[0];
		pBuildInfoBlock->Machine = (PCHAR) &MachineTypePC[0];
	#endif
	#ifdef KORE_MACHINE_TYPE_X86_32_XBOX
		pBuildInfoBlock->Architecture = (PCHAR) &ArchTypeX8632[0];
		pBuildInfoBlock->Machine = (PCHAR) &MachineTypeXBOX[0];
	#endif
	#ifdef KORE_MACHINE_TYPE_ARM
		pBuildInfoBlock->Architecture = (PCHAR) &ArchTypeARM[0];
		pBuildInfoBlock->Machine = (PCHAR) &MachineTypeUnknown[0];
	#endif
	#ifdef KORE_MACHINE_TYPE_MIPS
		pBuildInfoBlock->Architecture = (PCHAR) &ArchTypeMIPS[0];
		pBuildInfoBlock->Machine = (PCHAR) &MachineTypeUnknown[0];
	#endif
	#ifdef KORE_MACHINE_TYPE_POWERPC
		pBuildInfoBlock->Architecture = (PCHAR) &ArchTypePOWERPC[0];
		pBuildInfoBlock->Machine = (PCHAR) &MachineTypeUnknown[0];
	#endif

	/* BUILD TYPES */ 
	#ifdef BUILD_TYPE_DEBUG
		pBuildInfoBlock->Type = (PCHAR) &BuildTypeDEBUG[0];
	#endif
	#ifdef BUILD_TYPE_RELEASE
		pBuildInfoBlock->Type = (PCHAR) &BuildTypeRELEASE[0];
	#endif

	/* DEBUG LEVES */
	#ifdef DEBUG_LEVEL_DEBUG
		pBuildInfoBlock->DebugLevel = (PCHAR) &DebugLevelDEBUG[0];
	#endif
	#ifdef DEBUG_LEVEL_WARNING
		pBuildInfoBlock->DebugLevel = (PCHAR) &DebugLevelWARNING[0];
	#endif
	#ifdef DEBUG_LEVEL_TRACE
		pBuildInfoBlock->DebugLevel = (PCHAR) &DebugLevelTRACE[0];
	#endif
}
