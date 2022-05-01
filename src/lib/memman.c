/*
  - Date: 2018-04-02
  - Author: Joel
  - Purpose: MemoryManager functions
*/

#include <kldr.h>

#define MAX_MEMORY_MAP_ENTRIES					1024
#define _1MB									1024 * 1024

UINT32 pageSize;

MEMORY_MAP_ENTRY MemoryMap[MAX_MEMORY_MAP_ENTRIES];
UINT32 memoryMapEntryCount;

PMEMORY_DESCRIPTOR pFreeListBase;
PMEMORY_DESCRIPTOR pUsedListBase;
UINT32 freeListMaxEntries;
UINT32 usedListMaxEntries;
UINT32 freeListCurrentSize;
UINT32 usedListCurrentSize;
UINT32 maxAddressableFreeMem;
UINT32 availableFreeMem;

PCHAR Convert(PCHAR pDest, unsigned int num, PCHAR pUnit);
VOID InitFreeList();
VOID InitUsedList();
UINT32 GetMaxAddressableFreeMem();
UINT32 GetAvailableFreeMem();
BOOL IsRegionFree(UINT32 base, UINT32 size);
VOID AddToFreeList();
HANDLE AddToUsedList(UINT32 base, UINT32 size, MEMORY_FLAGS flags, PCHAR description);
VOID ZeroInit(HANDLE hMemory);
VOID CompactFreeList();

VOID MemoryManagerInit()
{
	if (!MachineMemoryGetMemoryMap(&MemoryMap[0], &memoryMapEntryCount))
	{
		ERROR("Memory cannot be initialized - Function call failed.");
		HALT;
	}
	
	if (memoryMapEntryCount == 0)
	{
		// Memory map query did not result in any entries
		ERROR("Memory cannot be initialized - Did not recieve any results from detection.");
		HALT;
	}

	if (memoryMapEntryCount >= MAX_MEMORY_MAP_ENTRIES)
	{
		// Memory map query resulted in more entries than what we can handle
		ERROR("Memory cannot be initialized - Recieved too many entries from detection.");
		HALT;
	}

	maxAddressableFreeMem = GetMaxAddressableFreeMem();
	if (maxAddressableFreeMem == 0)
	{
		// Memory map query resulted in no areas reported as free memory
		ERROR("Memory cannot be initialized - Recieved no free region from detection.");
		HALT;
	}

	availableFreeMem = GetAvailableFreeMem();
	if (availableFreeMem < _1MB)
	{
		// Memory map query resulted in not enough free memory
		ERROR("Not enough free memory to run Kore OS.");
		//HALT;
	}

	pageSize = MachineMemoryGetPageSize();

	// We have the actual results back - build Free and Occupied lists
	InitFreeList();
	InitUsedList();

	UiDrawText("MemoryManagerInit............DONE");
}

PCHAR Convert(PCHAR pDest, unsigned int num, PCHAR pUnit)
{
	CHAR temp[20] = {0};
	PCHAR pTemp = &temp[0];
	PCHAR pText = pDest;

	// Hex	
	uitoa(num, pTemp, 16);
	strcpy(pText, pTemp);
	pText += strlen(pTemp);

	// Opening Parenthesis
	*pText++ = ' ';
	*pText++ = '(';

    // Decimal
	uitoa(num, pTemp, 10);
	strcpy(pText, pTemp);
	pText += strlen(pTemp);

	// Unit
	strcpy(pText, pUnit);
	pText += strlen(pUnit);

    // Closing Parenthesis
	*pText++ = ')';
	*pText++ = '\0';
	
	return pDest;
}

VOID MemoryDisplayMap()
{
	UINT32 x = 4;
	UINT32 y = 3;
	UINT32 x2 = 76;
	UINT32 y2 = 17 + y;
	
	/* TABLE HEADER */

	UiDrawBox(x, y, x2, y2, 0x3b, LINE_STYLE_SINGLE, "");
	UiDrawTextEx(x + 3, y, COLOR_DO_NOT_CARE, "Memory Descriptors:     ");
	
	CHAR temp[20] = {0};
	itoa(memoryMapEntryCount, &temp[0], 10);
	UiDrawTextEx(x + 22, y, COLOR_DO_NOT_CARE, &temp[0]);

	UiDrawTextEx(x + 2, y + 2, COLOR_DO_NOT_CARE, "BASE                   | LENGTH                     | TYPE");
	UiDrawTextEx(x + 2, y + 3, COLOR_DO_NOT_CARE, "-----------------------+----------------------------+---------------");
	CHAR text[80] = {0};
	
	//int tempLength = 0;
	//int textLength = 0;

	/* TABLE BODY */
	
	for(int index = 0; index < memoryMapEntryCount; index++)
	{
		
		UiDrawTextEx(x +  2, y + 4 + index, COLOR_DO_NOT_CARE, Convert(&text[0], MemoryMap[index].base, ""));
		UiDrawTextEx(x + 27, y + 4 + index, COLOR_DO_NOT_CARE, Convert(&text[0], MemoryMap[index].size, " bytes"));
		//UiDrawTextEx(x + 52, y + 4 + index, COLOR_DO_NOT_CARE, Convert(&text[0], MemoryMap[index].type));

		PCHAR pMemoryTypeText;

		if (MemoryMap[index].type == MEMORY_MAP_ENTRY_TYPE_FREE)
		{
		  pMemoryTypeText = (PCHAR)&"FREE\0";
		}
		else if (MemoryMap[index].type == MEMORY_MAP_ENTRY_TYPE_RESERVED)
		{
		  pMemoryTypeText = (PCHAR)&"RESERVED\0";
		}
		else if (MemoryMap[index].type == MEMORY_MAP_ENTRY_TYPE_BAD)
		{
		  pMemoryTypeText = (PCHAR)&"BAD\0";
		}
		else if (MemoryMap[index].type == MEMORY_MAP_ENTRY_TYPE_KERNEL)
		{
		  pMemoryTypeText = (PCHAR)&"KERNEL\0";
		}
		else if (MemoryMap[index].type == MEMORY_MAP_ENTRY_TYPE_UNKNOWN)
		{
		  pMemoryTypeText = (PCHAR)&"UNKNOWN\0";
		}

		UiDrawTextEx(x + 56, y + 4 + index, COLOR_DO_NOT_CARE, pMemoryTypeText);
				
		UiDrawTextEx(x + 25, y + 4 + index, COLOR_DO_NOT_CARE, "|");
		UiDrawTextEx(x + 54, y + 4 + index, COLOR_DO_NOT_CARE, "|");
		
		//DebugPrintf("Base = %x, Length = %x, Type = %x", MemoryMap[index].base, MemoryMap[index].length, MemoryMap[index].type);
	}
	
	int index = memoryMapEntryCount;
	
	UiDrawTextEx(x + 2, y + 4 + index, COLOR_DO_NOT_CARE, "-----------------------+----------------------------+---------------");
	index+=2;
	
	/* TABLE FOOTER */

	UiDrawTextEx(x +  2, y + 4 + index, COLOR_DO_NOT_CARE, "Page size: ");
	UiDrawTextEx(x + 40, y + 4 + index, COLOR_DO_NOT_CARE, Convert(&text[0], pageSize, " bytes"));

	UiDrawTextEx(x +  2, y + 5 + index, COLOR_DO_NOT_CARE, "Max memory: ");
	UiDrawTextEx(x + 40, y + 5 + index, COLOR_DO_NOT_CARE, Convert(&text[0], maxAddressableFreeMem / 1024, " KBytes"));

	UiDrawTextEx(x +  2, y + 6 + index, COLOR_DO_NOT_CARE, "Available memory (reported by BIOS): ");
	UiDrawTextEx(x + 40, y + 6 + index, COLOR_DO_NOT_CARE, Convert(&text[0], availableFreeMem / 1024, " KBytes"));
}

VOID InitFreeList()
{
	// Got the device independent memory map
	// This can contain overlapping entries, free and used blocks.
	// To discover truely free memory we need to walk the map
	// Every time 4K (one page) memory will be "probed"
	// It needs to be marked as free and it should not exist within other "used" blocks
	// If that is true than it is truely free ram

	pFreeListBase = (PMEMORY_DESCRIPTOR) MachineMemoryGetFreeListBase();
	UINT32 freeListRegionSize = MachineMemoryGetFreeListSize();
	freeListCurrentSize = 0;
	memset((char*) pFreeListBase, 0, freeListRegionSize);

	freeListMaxEntries = freeListRegionSize / sizeof(MEMORY_DESCRIPTOR);

	UINT32 lastFreeRegionBase = 0;
	UINT32 lastFreeRegionSize = 0;

	for(UINT32 regionStart = 0; regionStart < maxAddressableFreeMem; regionStart += pageSize)
	{
		BOOL isRegionFree = IsRegionFree(regionStart, pageSize);
		
		if (isRegionFree)
		{
			if (lastFreeRegionSize == 0)
		 	{
		 		// New region found
		 		lastFreeRegionBase = regionStart;
		 	}
		
		 	// Add this to previous region's length
		 	lastFreeRegionSize += pageSize;
		}
		else
		{
			// Write last region
			if (lastFreeRegionSize > 0)
			{
				AddToFreeList(lastFreeRegionBase, lastFreeRegionSize);

				lastFreeRegionSize = 0;
			}
		}
	}

	if (lastFreeRegionSize > 0)
	{
		AddToFreeList(lastFreeRegionBase, lastFreeRegionSize);
	}
}

VOID InitUsedList()
{
	pUsedListBase = (PMEMORY_DESCRIPTOR) MachineMemoryGetUsedListBase();
	UINT32 usedListRegionSize = MachineMemoryGetUsedListSize();
	usedListCurrentSize = 0;
	memset((char*) pUsedListBase, 0, usedListRegionSize);

	usedListMaxEntries = usedListRegionSize / sizeof(MEMORY_DESCRIPTOR);
}

UINT32 GetMaxAddressableFreeMem()
{
	UINT32 maxMem = 0;

	for(int index = 0; index < memoryMapEntryCount; index++)
	{
		if (MemoryMap[index].type == MEMORY_MAP_ENTRY_TYPE_FREE &&
			maxMem < MemoryMap[index].base + MemoryMap[index].size)
		{
			maxMem = MemoryMap[index].base + MemoryMap[index].size;
		}
	}

	return maxMem;
}

UINT32 GetAvailableFreeMem()
{
	UINT32 availableMem = 0;

	for(int index = 0; index < memoryMapEntryCount; index++)
	{
		if (MemoryMap[index].type == MEMORY_MAP_ENTRY_TYPE_FREE)
		{
			availableMem += MemoryMap[index].size;
		}
	}

	return availableMem;
}

BOOL IsRegionFree(UINT32 base, UINT32 size)
{
	// Walk the detection map and check if this region has been marked free
	// But also check if it hasn't been marked something else
	// This is needed, because some BIOSes can report overlapping regions
	// And the memory map provided has the kernel occupied space marked at the end
	// of the map.

	BOOL isFree = FALSE;

	/* Check if region is marked free */
	for(UINT32 i = 0; i < memoryMapEntryCount; i++)
	{
		UINT32 mapEntryBase = MemoryMap[i].base;
		UINT32 mapEntrySize = MemoryMap[i].size;
		UINT32 mapEntryType = MemoryMap[i].type;

		if (mapEntryType == MEMORY_MAP_ENTRY_TYPE_FREE)
		{
			if (mapEntryBase <= base && mapEntryBase + mapEntrySize >= base + size)
			{
				isFree = TRUE;
			}
		}
	}

	/* Check if region is not marked anything else */
	for(UINT32 i = 0; i < memoryMapEntryCount; i++)
	{
		UINT32 mapEntryBase = MemoryMap[i].base;
		UINT32 mapEntrySize = MemoryMap[i].size;
		UINT32 mapEntryType = MemoryMap[i].type;

		if (mapEntryType != MEMORY_MAP_ENTRY_TYPE_FREE)
		{
			if (base + size <= mapEntryBase ||
				base >= mapEntryBase + mapEntrySize)
			{
				continue;
			}

			return FALSE;
		}
	}

	return isFree;
}

VOID AddToFreeList(UINT32 base, UINT32 size)
{
	PMEMORY_DESCRIPTOR pFreeListEntry = pFreeListBase + freeListCurrentSize;// + (freeListCurrentSize * sizeof(MEMORY_DESCRIPTOR));

	pFreeListEntry->base = base;
	pFreeListEntry->size = size ;
	pFreeListEntry->description = "Free region.\0";
	
	if (freeListCurrentSize == 0)
	{
		pFreeListEntry->next = NULL;
		pFreeListEntry->prev = 0;
	}
	else
	{
		pFreeListEntry->prev = pFreeListEntry - 1;
		((PMEMORY_DESCRIPTOR)pFreeListEntry->prev)->next = pFreeListEntry;
	}

	freeListCurrentSize++;
}

VOID MemoryDisplayFreeList()
{
	UINT32 x = 1;
	UINT32 y = 1;
	UINT32 x2 = 78;
	UINT32 y2 = y  + 5 + freeListCurrentSize;
	
	/* TABLE HEADER */

	UiDrawBox(x, y, x2, y2, 0x3b, LINE_STYLE_SINGLE, "");
	UiDrawTextEx(x + 3, y, COLOR_DO_NOT_CARE, "Free List Entries:     ");

	CHAR temp[20] = {0};
	itoa(freeListCurrentSize, &temp[0], 10);
	UiDrawTextEx(x + 22, y, COLOR_DO_NOT_CARE, &temp[0]);

	UiDrawTextEx(x + 2, y + 2, COLOR_DO_NOT_CARE, "BASE                   | LENGTH                | TYPE");
	UiDrawTextEx(x + 2, y + 3, COLOR_DO_NOT_CARE, "-----------------------+-----------------------+--------------------");
	CHAR text[80] = {0};

	/* TABLE BODY */
	PMEMORY_DESCRIPTOR pFreeListEntry = pFreeListBase;

	int index = 0;
	for(int i = 0; i < freeListMaxEntries; i++)
	{
		if (pFreeListEntry->size != 0)
		{
			UiDrawTextEx(x +  2, y + 4 + index, COLOR_DO_NOT_CARE, Convert(&text[0], pFreeListEntry->base, ""));
			UiDrawTextEx(x + 27, y + 4 + index, COLOR_DO_NOT_CARE, Convert(&text[0], pFreeListEntry->size, ""));

			if (pFreeListEntry->description == NULL)
			{
				UiDrawTextEx(x + 51, y + 4 + index, COLOR_DO_NOT_CARE, (PCHAR)&"NULL");
			}
			else
			{
				UiDrawTextEx(x + 51, y + 4 + index, COLOR_DO_NOT_CARE, pFreeListEntry->description);
			}

			UiDrawTextEx(x + 25, y + 4 + index, COLOR_DO_NOT_CARE, "|");
			UiDrawTextEx(x + 49, y + 4 + index, COLOR_DO_NOT_CARE, "|");

			index++;
		}

		//pFreeListEntry = pFreeListEntry->next;
		pFreeListEntry++;
	}
}

VOID MemoryDisplayUsedList()
{
	UINT32 x = 1;
	UINT32 y = 1;
	UINT32 x2 = 78;
	UINT32 y2 = y  + 5 + usedListCurrentSize;
	
	/* TABLE HEADER */

	UiDrawBox(x, y, x2, y2, 0x3b, LINE_STYLE_SINGLE, "");
	UiDrawTextEx(x + 3, y, COLOR_DO_NOT_CARE, "Used List Entries:     ");

	CHAR temp[20] = {0};
	itoa(usedListCurrentSize, &temp[0], 10);
	UiDrawTextEx(x + 22, y, COLOR_DO_NOT_CARE, &temp[0]);

	UiDrawTextEx(x + 2, y + 2, COLOR_DO_NOT_CARE, "BASE                   | LENGTH                | TYPE");
	UiDrawTextEx(x + 2, y + 3, COLOR_DO_NOT_CARE, "-----------------------+-----------------------+---------------");
	CHAR text[80] = {0};

	/* TABLE BODY */
	PMEMORY_DESCRIPTOR pUsedListEntry = pUsedListBase;

	//pUsedListEntry += 20;

	int index = 0;
	for(int i = 0; i < usedListMaxEntries; i++)
	{
		if (pUsedListEntry->size != 0)
		{
			UiDrawTextEx(x +  2, y + 4 + index, COLOR_DO_NOT_CARE, Convert(&text[0], pUsedListEntry->base, ""));
			UiDrawTextEx(x + 27, y + 4 + index, COLOR_DO_NOT_CARE, Convert(&text[0], pUsedListEntry->size, " bytes"));

			if (pUsedListEntry->description == NULL)
			{
				UiDrawTextEx(x + 51, y + 4 + index, COLOR_DO_NOT_CARE, (PCHAR)&"NULL");
			}
			else
			{
				UiDrawTextEx(x + 51, y + 4 + index, COLOR_DO_NOT_CARE, pUsedListEntry->description);
			}			

			UiDrawTextEx(x + 25, y + 4 + index, COLOR_DO_NOT_CARE, "|");
			UiDrawTextEx(x + 49, y + 4 + index, COLOR_DO_NOT_CARE, "|");

			index++;
		}

		//pUsedListEntry = pUsedListEntry->next;
		pUsedListEntry++;
	}
}

// Goes through the FreeList and selects a suitable block of memory
// based on the flags.
// Adds the block to the UsedList and returns the handle (pointer to the UsedList entry)
HANDLE MemoryAllocate(UINT32 size, MEMORY_FLAGS flags, PCHAR pDescription)
{
	PMEMORY_DESCRIPTOR pFreeListEntry = pFreeListBase;

	//UiDrawText("Memory allocate called\0");

	if (freeListCurrentSize == 0)
	{
		BugCheck(NULL, (PCHAR) &"Freelist gone!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);
	}

	for(int index = 0; index < freeListCurrentSize; index++)
	{
		if (pFreeListEntry->size >= size)
		{
			// Add to used list
			HANDLE hMemory = AddToUsedList(pFreeListEntry->base, size, flags, pDescription);

			// Shrink free block size
			pFreeListEntry->base += size; 
			pFreeListEntry->size -= size; 

			// Erase entry if size = 0

			if (pFreeListEntry->size == 0)
			{
				pFreeListEntry->base = 0;
				pFreeListEntry->size = 0;
				pFreeListEntry->flags = 0;
				pFreeListEntry->lockCount = 0;
				pFreeListEntry->description = NULL;
				pFreeListEntry->prev = NULL;
				pFreeListEntry->next = NULL;
			}

			if (flags & MEMORY_FLAG_ZERO_INIT == MEMORY_FLAG_ZERO_INIT)
			{
				ZeroInit(hMemory);
			}

			return hMemory;
		}

		pFreeListEntry++;
	}

	// Could not find a suitable block

	return (HANDLE)NULL;
}

HANDLE AddToUsedList(UINT32 base, UINT32 size, MEMORY_FLAGS flags, PCHAR description)
{
	PMEMORY_DESCRIPTOR pUsedListEntry = pUsedListBase;

	for(int index = 0; index < usedListMaxEntries; index++)
	{
		if (pUsedListEntry->base == 0 &&
			pUsedListEntry->size == 0)
		{
			pUsedListEntry->base = base;
			pUsedListEntry->size = size;
			pUsedListEntry->flags = flags;
			pUsedListEntry->lockCount = 0;
			pUsedListEntry->description = description;

			usedListCurrentSize++;

			return (HANDLE)pUsedListEntry;			
		}

		pUsedListEntry++;
	}
}


// Uses the handle as a pointer to the UsedList entry and gives back the base address of the block
// Increments lock count if not fixed memory
PVOID MemoryLock(HANDLE hMemory)
{
	if (hMemory == (HANDLE) NULL)
	{
		BugCheck(NULL, (PCHAR) &"NULL Memory Handle", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Memory handle:", hMemory);   

		return NULL;
	}

	if ((UINT32) hMemory < X86_USEDLIST_BASE ||
		(UINT32) hMemory > X86_USEDLIST_BASE + X86_USEDLIST_SIZE)
	{
		BugCheck(NULL, (PCHAR) &"Memory handle out of range!", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, (PCHAR) &"Memory handle:", hMemory);   
	}

	PMEMORY_DESCRIPTOR pUsedListEntry = (PMEMORY_DESCRIPTOR) hMemory;

	if (!(pUsedListEntry->flags & MEMORY_FLAG_FIXED))
	{
		pUsedListEntry->lockCount++;
	}	

	//BugCheck(NULL, (PCHAR) pUsedListEntry->description, (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, NULL, 0);   

	if (pUsedListEntry->base == 0)
	{
		BugCheck(NULL, (PCHAR) &"Base 0?", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Memory handle:", hMemory);   
	}
	if (pUsedListEntry->size == 0)
	{
		BugCheck(NULL, (PCHAR) &"Size 0?", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Memory handle:", hMemory);   
	}
	if (pUsedListEntry->flags == 0)
	{
		BugCheck(NULL, (PCHAR) &"Flags 0?", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Memory handle:", hMemory);   
	}

	return (PVOID)pUsedListEntry->base;
}

// Decrements lock count if > 0
// Returns true when lock count is 0 if fixed memory
BOOL MemoryUnlock(HANDLE hMemory)
{
	if (hMemory == (HANDLE) NULL)
	{
		BugCheck(NULL, (PCHAR) &"NULL Memory Handle", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Memory handle:", hMemory);   

		return FALSE;
	}

	PMEMORY_DESCRIPTOR pUsedListEntry = (PMEMORY_DESCRIPTOR) hMemory;

	if (pUsedListEntry->flags & MEMORY_FLAG_FIXED)
	{
		return TRUE;
	}	

	if (pUsedListEntry->lockCount > 0)
	{
		pUsedListEntry->lockCount--;
	}

	if (pUsedListEntry->lockCount == 0)
	{
		return TRUE;
	}

	return FALSE;
}

// Returns the memory block held by the handle to the pool
// and frees the resources held.
// Returns true when successful
BOOL MemoryFree(HANDLE hMemory)
{
	if (hMemory == (HANDLE) NULL)
	{
		BugCheck(NULL, (PCHAR) &"NULL Memory Handle", (PCHAR) __FILE__, (PCHAR) __FUNCTION__, __LINE__, "Memory handle:", hMemory);   

		return FALSE;
	}

	PMEMORY_DESCRIPTOR pUsedListEntry = (PMEMORY_DESCRIPTOR) hMemory;

	if (pUsedListEntry->lockCount > 0)
	{
		return FALSE;
	}

	AddToFreeList(pUsedListEntry->base, pUsedListEntry->size);

	pUsedListEntry->base = 0;
	pUsedListEntry->size = 0;
	pUsedListEntry->flags = 0;
	pUsedListEntry->lockCount = 0;
	pUsedListEntry->description = NULL;
	pUsedListEntry->prev = NULL;
	pUsedListEntry->next = NULL;

	usedListCurrentSize--;

	CompactFreeList();

	return TRUE;
}

VOID ZeroInit(HANDLE hMemory)
{
		PMEMORY_DESCRIPTOR pUsedListEntry = (PMEMORY_DESCRIPTOR) hMemory;

		memset((char*) pUsedListEntry->base, 0, pUsedListEntry->size);
}

VOID CompactFreeList()
{}

UINT32 GetTotalPhysicalMemory()
{
	return maxAddressableFreeMem;
}

BOOL EnumerateMemoryMapEntries(PMEMORY_MAP_ENTRY pMemoryMapEntry)
{
	if (pMemoryMapEntry->base == 0 &&
	    pMemoryMapEntry->size == 0 &&
		pMemoryMapEntry->type == 0)
	{
		pMemoryMapEntry[0] = MemoryMap[0];

		return TRUE;
	}

	for(UINT32 i = 0; i < memoryMapEntryCount; i++)
	{
		if (MemoryMap[i].base == pMemoryMapEntry->base &&
	    	MemoryMap[i].size == pMemoryMapEntry->size &&
			MemoryMap[i].type == pMemoryMapEntry->type)
		{
			// Previous entry found, get next
			if (i + 1 < memoryMapEntryCount)
			{
				pMemoryMapEntry[0] = MemoryMap[i + 1];

				return TRUE;
			}
		}
	}

	return FALSE;
}
