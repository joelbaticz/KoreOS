/*
  - Date: 2018-02-22
  - Author: Joel
  - Purpose: Build info definitions
*/

#ifndef __BUILD_INFO_H
#define __BUILD_INFO_H

/* DEFINITIONS */
typedef struct _BUILD_INFO_BLOCK
{
	PCHAR Architecture;
	PCHAR Machine;
	PCHAR Type;
	PCHAR DebugLevel;
} BUILD_INFO_BLOCK, *PBUILD_INFO_BLOCK;


/* FUNCTIONS */
VOID __cdecl GetBuildInformation(OUT PBUILD_INFO_BLOCK pBuildInfoBlock);

#endif /* __BUILD_INFO_H */
