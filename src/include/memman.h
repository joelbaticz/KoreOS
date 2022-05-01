/*
  - Date: 2018-04-02
  - Author: Joel
  - Purpose: Definitions for MemoryManager functions
*/

#ifndef __MEMMAN_H
#define __MEMMAN_H

#define MEMORY_MAP_ENTRY_TYPE_FREE				0
#define MEMORY_MAP_ENTRY_TYPE_RESERVED			1
#define MEMORY_MAP_ENTRY_TYPE_KERNEL			2
#define MEMORY_MAP_ENTRY_TYPE_BAD				3
#define MEMORY_MAP_ENTRY_TYPE_UNKNOWN			4

#define MEMORY_FLAG_BELOW_1MB					1
#define MEMORY_FLAG_BELOW_16MB					2
#define MEMORY_FLAG_MOVABLE						4
#define MEMORY_FLAG_FIXED   					8
#define MEMORY_FLAG_ZERO_INIT					16

typedef struct _MEMORY_MAP_ENTRY
{
	UINT32 base;	//Allocation base (page size aligned)
	UINT32 size;	//Allocation size (page size aligned)
	UINT32 type;	//Allocation type
} __attribute__((packed)) MEMORY_MAP_ENTRY, *PMEMORY_MAP_ENTRY;

typedef UINT32 MEMORY_OWNER;
typedef UINT32 MEMORY_FLAGS;

typedef struct _MEMORY_DESCRIPTOR
{
	MEMORY_OWNER owner;
	UINT32 base;
	UINT32 size;
	MEMORY_FLAGS flags;
	UINT32 lockCount;
	PCHAR description;
	PVOID next;
	PVOID prev;
} __attribute__((packed)) MEMORY_DESCRIPTOR, *PMEMORY_DESCRIPTOR;

VOID MemoryManagerInit();
VOID MemoryDisplayMap();
VOID MemoryDisplayFreeList();
VOID MemoryDisplayUsedList();
HANDLE MemoryAllocate(UINT32 size, MEMORY_FLAGS flags, PCHAR pDescription);
PVOID MemoryLock(HANDLE hMemory);
BOOL MemoryUnlock(HANDLE hMemory);
BOOL MemoryFree(HANDLE hMemory);

UINT32 GetTotalPhysicalMemory();     // The size of memory installed in the device
UINT32 GetAvailablePhysicalMemory(); // The size of memory that is usable by the system (physical memory - reserved regions)
UINT32 GetFreeMemory();
UINT32 GetLargestFreeBlock();

BOOL EnumerateMemoryMapEntries(PMEMORY_MAP_ENTRY pMemoryMapEntry);

#endif /* __MEMMAN_H */