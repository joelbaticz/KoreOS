/*
  - Date: 2018-04-04
  - Author: Joel
  - Purpose: PC Memory implementation:
		 Machine specific memory functions.
*/

#include <kldr.h>

#define PAGE_SIZE									4096
#define BIOS_MEMORY_MAP_TYPE_USABLE					1
#define BIOS_MEMORY_MAP_TYPE_RESERVED				2
#define BIOS_MEMORY_MAP_TYPE_ACPI_RECLAIMABLE		3
#define BIOS_MEMORY_MAP_TYPE_ACPI_NVS				4
#define BIOS_MEMORY_MAP_TYPE_BAD					5
#define BIOS_MEMORY_MAP_TYPE_KERNEL					6
	
//__attribute__((packed))
typedef struct _BIOS_MEMORY_MAP_ENTRY
{
	UINT32 baseLow;
	UINT32 baseHigh;
	UINT32 lengthLow;
	UINT32 lengthHigh;
	UINT32 type;
	UINT32 ACPI;
} __attribute__((packed, aligned(0x10))) BIOS_MEMORY_MAP_ENTRY, *PBIOS_MEMORY_MAP_ENTRY;

PMEMORY_MAP_ENTRY pPcMemoryMap;
UINT32 pcMapEntries;

BOOL PcMemoryBiosDetectMemory();
VOID PcMemoryAddUsedBlocks();
VOID PcMemoryAddBlock(UINT32 base, UINT32 size, UINT32 type);

BOOL PcMemoryGetMemoryMap(OUT PMEMORY_MAP_ENTRY pMemoryMap, OUT PUINT32 pMemoryMapEntryCount)
{
	pPcMemoryMap = pMemoryMap;
	pcMapEntries = 0;

	if (PcMemoryBiosDetectMemory())
	{
		PcMemoryAddUsedBlocks();

		*pMemoryMapEntryCount = pcMapEntries;
		return TRUE;
	}

	return FALSE;
}

BOOL PcMemoryBiosDetectMemory()
{
	BOOL validE820Call = TRUE;
	
	/*
		INT 15h - Bios Memory services.
		
		On entry:
			EAX = e820h - Query System Memory Map
			EBX = Continuation value (Index = 0)
			ECX = Buffer size
			EDX = Signature ("SMAP")
			ES:DI = Pointer to Buffer
		On return:
			EAX = e820h
			EBX = Continuation value (zero if last entry)
			ECX = Buffer size (actual)
			EDX = Signature ??? not EAX?
			ES:DI = Pointer to filled Buffer

	*/	
	
	REGISTERS regs;
	PREGISTERS pRegs = &regs;
		
	PBIOS_MEMORY_MAP_ENTRY pBiosBuffer = (PBIOS_MEMORY_MAP_ENTRY) (UINT32)X86_REAL_MODE_BUFFER_ADDRESS;
		
	regs.EAX = 0xe820;
	regs.EBX = 0;
	regs.ECX = 24;
	regs.EDX = 0x534d4150;
	regs.ESI = 0;
	regs.EDI = OFFSET((UINT32)pBiosBuffer);
	regs.EBP = 0;
	regs.DS = 0;
	regs.ES = SEGMENT((UINT32)pBiosBuffer);
	regs.FS = 0;
	regs.GS = 0;
	regs.EFLAGS = 0;
	
	while(TRUE)
	{
		regs.EAX = 0xe820; /* Needs to be reset before every function call */
		
		PcBiosCallInterrupt(0x15, pRegs, pRegs);			
		
		//DebugPrintf("INT 0x15 Returned: EFLAGS_CARRY=%x, EAX=%x, EBX=%x, ECX=%x, EDX=%x, ES=%x, EDI=%x",regs.EFLAGS & EFLAGS_CARRY, regs.EAX, regs.EBX, regs.ECX, regs.EDX, regs.ES, regs.EDI);
		//PAUSE;
		
		if ((regs.EFLAGS & EFLAGS_CARRY) || (regs.EAX != 0x534d4150))
		{
			/* Function call failed */
			return FALSE;
		}
		
		if (regs.EBX == 0)
		{
			/* Last entry reached --> return with success */
			return TRUE;
		}
		
		/* Successful function call - now copy entry to descriptor */
			
		//DebugPrintf("BUFFER: BASE_LOW=%x, BASE_HIGH=%x, LENGTH_LOW=%x, LENGTH=HIGH=%x, TYPE=%x",pBiosBuffer->baseLow, pBiosBuffer->baseHigh, pBiosBuffer->lengthLow, pBiosBuffer->lengthHigh, pBiosBuffer->type);
		//PAUSE;

		PcMemoryAddBlock(pBiosBuffer->baseLow, pBiosBuffer->lengthLow, pBiosBuffer->type);

		// pMemoryMap->base = pBiosBuffer->baseLow;
		// pMemoryMap->length = pBiosBuffer->lengthLow; // Round up
		
		// /* Type conversion */
		// pMemoryMap->type = pBiosBuffer->type;
			
		// if (pBiosBuffer->type == BIOS_MEMORY_MAP_TYPE_USABLE)
		// {
		//   pMemoryMap->type = MEMORY_DESCRIPTOR_TYPE_FREE;
		// }				
		// else if (pBiosBuffer->type == BIOS_MEMORY_MAP_TYPE_RESERVED ||
		// 		 pBiosBuffer->type == BIOS_MEMORY_MAP_TYPE_ACPI_RECLAIMABLE ||
		// 		 pBiosBuffer->type == BIOS_MEMORY_MAP_TYPE_ACPI_NVS)
		// {
		//   pMemoryMap->type = MEMORY_DESCRIPTOR_TYPE_RESERVED;
		// }	
		// else if (pBiosBuffer->type == BIOS_MEMORY_MAP_TYPE_BAD)
		// {
		//   pMemoryMap->type = MEMORY_DESCRIPTOR_TYPE_BAD;
		// }	
		
		// pMemoryMap++;
		// (*pMemoryMapEntryCount)++;
	}
}

VOID PcMemoryAddUsedBlocks()
{
	// BIOS Interrupt Vector Table
	PcMemoryAddBlock(0, 0x400, BIOS_MEMORY_MAP_TYPE_RESERVED);

	// Loader / Kernel used block
	//PcMemoryAddBlock(X86_LOADER_START, X86_LOADER_END - X86_LOADER_START, BIOS_MEMORY_MAP_TYPE_KERNEL);
	PcMemoryAddBlock(0x400, 0xFFC00, BIOS_MEMORY_MAP_TYPE_KERNEL); // 0->1MB
}

VOID PcMemoryAddBlock(UINT32 base, UINT32 size, UINT32 type)
{
		PMEMORY_MAP_ENTRY pEntry = pPcMemoryMap;
		pEntry += pcMapEntries;  // Get to the end of the list

		pEntry->base = base;
		pEntry->size = size; // Round up
			
		if (type == BIOS_MEMORY_MAP_TYPE_USABLE)
		{
		  pEntry->type = MEMORY_MAP_ENTRY_TYPE_FREE;
		}				
		else if (type == BIOS_MEMORY_MAP_TYPE_RESERVED ||
				 type == BIOS_MEMORY_MAP_TYPE_ACPI_RECLAIMABLE ||
				 type == BIOS_MEMORY_MAP_TYPE_ACPI_NVS)
		{
		  pEntry->type = MEMORY_MAP_ENTRY_TYPE_RESERVED;
		}	
		else if (type == BIOS_MEMORY_MAP_TYPE_BAD)
		{
		  pEntry->type = MEMORY_MAP_ENTRY_TYPE_BAD;
		}
		else if (type == BIOS_MEMORY_MAP_TYPE_KERNEL)
		{
		  pEntry->type = MEMORY_MAP_ENTRY_TYPE_KERNEL;
		}
		else
		{
			pEntry->type = MEMORY_MAP_ENTRY_TYPE_UNKNOWN;
		}
		
		pcMapEntries++;
}

UINT32 PcMemoryGetPageSize()
{
	return PAGE_SIZE;
}

UINT32 PcMemoryGetFreeListBase()
{
	return X86_FREELIST_BASE;
}

UINT32 PcMemoryGetUsedListBase()
{
	return X86_USEDLIST_BASE;
}

UINT32 PcMemoryGetFreeListSize()
{
	return X86_FREELIST_SIZE;
}

UINT32 PcMemoryGetUsedListSize()
{
	return X86_USEDLIST_SIZE;
}