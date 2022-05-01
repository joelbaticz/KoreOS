/*
  - Date: 2018-04-04
  - Author: Joel
  - Purpose: Definitions for PC Memory functions
*/

BOOL PcMemoryGetMemoryMap(OUT PMEMORY_MAP_ENTRY pMemoryMap, OUT PUINT32 pMemoryMapEntryCount);
UINT32 PcMemoryGetPageSize();
UINT32 PcMemoryGetFreeListBase();
UINT32 PcMemoryGetUsedListBase();
UINT32 PcMemoryGetFreeListSize();
UINT32 PcMemoryGetUsedListSize();