/*
  - Date: 2018-01-12
  - Author: Joel
  - Purpose: FAT32 Volume Boot Record for X86
*/

/* Public Definitions */
#include "../../../../../include/asminc.S"
#include "../../../def.S"

/* Private Definitions */
/* VBR Uninitialized Data Section */
#define X86_VBR_DATA_SECTION_AREA               0x7b80
#define X86_VBR_DATA_SECTION_BASE		0x7c00
#define X86_VBR_OFFSET_TO_DATA_SECTION_AREA	X86_VBR_DATA_SECTION_AREA - X86_BOOT_START_OFFSET
#define X86_VBR_BOOT_DRIVE_ID			X86_VBR_OFFSET_TO_DATA_SECTION_AREA
#define X86_VBR_BOOT_PARTITION_ID		X86_VBR_OFFSET_TO_DATA_SECTION_AREA + 1
#define X86_VBR_MAX_SECTORS			X86_VBR_OFFSET_TO_DATA_SECTION_AREA + 2
#define X86_VBR_MAX_HEADS			X86_VBR_OFFSET_TO_DATA_SECTION_AREA + 3
#define X86_VBR_STARTING_SECTOR_OF_DATA_AREA	X86_VBR_OFFSET_TO_DATA_SECTION_AREA + 4
#define X86_VBR_CLUSTER_NUMBER			X86_VBR_OFFSET_TO_DATA_SECTION_AREA + 8

/* BIOS Parameter Block Descriptors */
#define X86_VBR_NUMBER_OF_COPIES_OF_FAT         NumberOfCopiesOfFAT - X86_BOOT_START_OFFSET
#define X86_VBR_CLUSTER_NUMBER_OF_ROOT_DIR	ClusterNoOfRootDir - X86_BOOT_START_OFFSET
#define X86_VBR_SECTORS_PER_CLUSTER		SectorsPerCluster - X86_BOOT_START_OFFSET
#define X86_VBR_NUMBER_OF_SECTORS_PER_FAT	NumberOfSectorsPerFAT - X86_BOOT_START_OFFSET
#define X86_VBR_NUMBER_OF_HIDDEN_SECTORS	NumberOfHiddenSectors - X86_BOOT_START_OFFSET
#define X86_VBR_NUMBER_OF_RESERVED_SECTORS	NumberOfReservedSectors - X86_BOOT_START_OFFSET
#define X86_VBR_BYTES_PER_SECTOR		BytesPerSector - X86_BOOT_START_OFFSET

/* Constants */
#define X86_VBR_DISK_BUFFER_AREA_OFFSET		0x0600
#define X86_VBR_LOADER_SIGNATURE_OFFSET		X86_LOADER_BASE + 0x03fe
#define X86_VBR_FAT_END_OF_CLUSTER_FLAG		0x0FFFFFF6

.code16

start:

	jmp Loader
	nop

BIOSParameterBlockFAT32:
OEMName:
	.ascii "MSDOS5.0"
BytesPerSector: 
	.word 512
SectorsPerCluster:
	.byte 8
NumberOfReservedSectors:
	.word 1134
NumberOfCopiesOfFAT:
	.byte 2
NoOfRootDirEntries:
	.word 0
NoOfSectorsInPartionSmallerThan32MB:
	.word 0
MediaDescriptor:
	.byte 0xf8
SectorsPerFATInOlderFATSystems:
	.word 0
SectorsPerTrack:
	.word 63
NumberOfHeads:
	.word 255
NumberOfHiddenSectors:
	.long 128
NumberOfSectors:
	.long 7823360
NumberOfSectorsPerFAT:
	.long 7625
Flags:
	.word 0
VersionOfFAT32:
	.word 0
ClusterNoOfRootDir:
	.long 2
SectorNoOfFileSystemInformationSector:
	.word 1
SectorNoOfBackupBootSector:
	.word 6
Reserved:
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
LogicalDriveNumberOfPartition:
	.byte 0x80
Unused:
	.byte 0
ExtendedSignature:
	.byte 0x29
SerialNumberOfPartition:
	.byte 0x7e, 0x4e, 0x43, 0x3a 
VolumeLabel:
	.ascii "NO NAME    "
FileSystemName:
	.ascii "FAT32   "
	
Loader:
	xor ax,ax
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov sp,X86_STACK16

	mov bp,X86_VBR_DATA_SECTION_BASE

	/* Save BootDriveID and BootPartitionID for later */
	/* Boot Drive ID is needed for VBS (DO NOT REMOVE!) */
	mov byte ptr [bp+X86_VBR_BOOT_DRIVE_ID], dl
	/* mov byte ptr [bp+X86_VBR_BOOT_PARTITION_ID], dh */
	push dx
	
	/* Reset Disk System, DL = DriveID */
	int X86_BIOS_DISK_FUNCTION
	jc ErrorLoadingOS

	mov ah,X86_BIOS_DISK_FUNCTION_GET_DRIVE_PARAMETERS
	int X86_BIOS_DISK_FUNCTION
	jc ErrorLoadingOS
	
	/* Calculate MaxSectors */
	and cl,X86_BIOS_MAX_DRIVE_SECTOR_COUNT	
	mov byte ptr [bp+X86_VBR_MAX_SECTORS],cl

	/* Calculate MaxHeads */
	inc dh
	mov byte ptr [bp+X86_VBR_MAX_HEADS],dh

	movzx eax,byte ptr [bp+X86_VBR_NUMBER_OF_COPIES_OF_FAT]

	mov ebx,dword ptr [bp+X86_VBR_NUMBER_OF_SECTORS_PER_FAT]
	mul ebx

	movzx ebx,word ptr [bp+X86_VBR_NUMBER_OF_RESERVED_SECTORS]
	add eax,ebx

	add eax,dword ptr [bp+X86_VBR_NUMBER_OF_HIDDEN_SECTORS] 

	mov dword ptr [bp+X86_VBR_STARTING_SECTOR_OF_DATA_AREA],eax

	mov eax,dword ptr [bp+X86_VBR_CLUSTER_NUMBER_OF_ROOT_DIR]
NextCluster:

	sub eax,2
	cmp eax,X86_VBR_FAT_END_OF_CLUSTER_FLAG
	jae ErrorLoadingOS 

	mov dword ptr [bp+X86_VBR_CLUSTER_NUMBER],eax

	movzx ebx,byte ptr [bp+X86_VBR_SECTORS_PER_CLUSTER]
	mul ebx

	/* EDX = A counter for counting how many sectors we still have in the cluster */
	/* mov edx,ebx */  /* XCHG WOULD IT BE SMALLER? WE TRASH BX ANYWAY */
	xchg edx,ebx

	add eax,dword ptr [bp+X86_VBR_STARTING_SECTOR_OF_DATA_AREA]
	
NextSectorInCluster:
	mov ebx,X86_VBR_DISK_BUFFER_AREA_OFFSET
	mov di,bx

	call LoadSector
	jc ErrorLoadingOS
	
	add bx,512

NextDirectoryEntry:
	mov cx,11
	mov si,offset LoaderString
	repz cmpsb
	jz LoaderFound

	add di,cx
	add di,21
	cmp di,bx
	jb NextDirectoryEntry

	dec edx
	jnz NextSectorInCluster

	call FindNextCluster
	jc ErrorLoadingOS

	jmp NextCluster

LoaderFound:

	mov ax,word ptr [di+9]
	shl eax,16
	mov ax,word ptr [di+15]

CheckCluster:
	mov dword ptr [bp+X86_VBR_CLUSTER_NUMBER],eax
	
	sub eax,2
	cmp eax,X86_VBR_FAT_END_OF_CLUSTER_FLAG
	jae ErrorLoadingOS

	/* After searching the Root Dir and finding the Loader file BX points to 0800h already */

	mov ebx,X86_LOADER_BASE
ProcessNextCluster:
	
	movzx ecx, byte ptr [bp + X86_VBR_SECTORS_PER_CLUSTER]
	mul ecx

	add eax,dword ptr [bp + X86_VBR_STARTING_SECTOR_OF_DATA_AREA]

ProcessNextSector:
	
	call LoadSector
	jc ErrorLoadingOS

	add ebx,512
	loop ProcessNextSector
	
	call FindNextCluster
	jc ErrorLoadingOS

	
	mov dword ptr [bp + X86_VBR_CLUSTER_NUMBER],eax
	sub eax,2
	cmp eax,X86_VBR_FAT_END_OF_CLUSTER_FLAG
	jb ProcessNextCluster	

	/* All sectors loaded */

	cmp word ptr [X86_VBR_LOADER_SIGNATURE_OFFSET], X86_LOADER_SIGNATURE
	jnz ErrorLoadingOS

	/* Transfer Execution */
	
	/*
	mov dl,byte ptr [bp + X86_VBR_BOOT_DRIVE_ID]
	mov dh,byte ptr [bp + X86_VBR_BOOT_PARTITION_ID]
	*/
	
	/* Wouldn't it be better with push at the start? */
	/* We would gain 8 bytes */
	pop dx	

	jmp 0x0:X86_LOADER_BASE

ErrorLoadingOS:	

	mov si,offset MessageErrorLoadingOS
	call PrintString

	sti
	jmp $

FindNextCluster:

	push ebx
	/* push edx - NOT BEING USED IN CALLER PROCESSES */ 

	xor edx,edx
	mov eax,dword ptr [bp+X86_VBR_CLUSTER_NUMBER]

	// Real cluster numbers start from 2
	add eax,2

	shl eax,2

	movzx ebx,word ptr [bp+X86_VBR_BYTES_PER_SECTOR]
	div ebx

	add eax,dword ptr [bp+X86_VBR_NUMBER_OF_HIDDEN_SECTORS]
	
	movzx ebx,word ptr [bp+X86_VBR_NUMBER_OF_RESERVED_SECTORS]
	add eax,ebx

	mov ebx,X86_VBR_DISK_BUFFER_AREA_OFFSET
	call LoadSector
	/* jc FindNextClusterDone - NO NEED TO CHECK HERE - WE CHECK AT THE CALLER */

	/* sub bx,512  */ /* WHY DO WE DO THIS??? IF LOADSECTOR WOULDN'T INCREMENT WE WOULD BE BETTER OFF */
	add bx,dx
	
	mov eax,dword ptr [bx]

/* FindNextClusterDone: */

	/* pop edx */
	pop ebx
	
	ret


LoadSector:
	/*
	push eax
	push ecx
	push edx
	push si
	push ebx
	*/

	push es
	pushad

	shr ebx,4
	mov es,bx

	/* Why don't we use XOR EDX,EDX MOVZX EBX,BYTE... DIV EBX --> STUFF IN EAX, EDX anyway ???
	IT WOULD GIVE ME 2 BYTES EXTRA
	xor edx,edx
	movzx ebx, byte ptr [bp+X86_VBR_MAX_SECTORS]
	div ebx
	*/
	
	mov dx,ax
	shr eax,16
	xchg dx,ax

	movzx bx,byte ptr [bp+X86_VBR_MAX_SECTORS]
	div bx

	inc dx
	mov cl,dl

	movzx bx,byte ptr [bp+X86_VBR_MAX_HEADS]

	xor dx,dx
	div bx

	mov ch,al
	xchg dl,dh

	/* pop ebx */


	
	/*
	mov eax,ebx
	shr eax,4
	mov es,ax
	shl eax,4
	sub ebx,eax
	xor bx,bx
	*/

	xor bx,bx
	mov dl,byte ptr [bp + X86_VBR_BOOT_DRIVE_ID]
	mov ax,0x0201
	

	
	int X86_BIOS_DISK_FUNCTION

 /* xchg bx,bx */
	
	
	/*
	pop si
	pop edx
	pop ecx
	pop eax
	*/

	popad
	pop es

	/* add ebx,512 */
	inc eax

	ret	

PrintString:
			
	lodsb
	
	or al,al
	jz PrintStringDone

	mov ah,0x0e
	mov bx,0x0007
	int 0x10	

	jmp PrintString

PrintStringDone:

	ret

LoaderString:
	.ascii "KLDR    SYS"
MessageErrorLoadingOS:
	.ascii "Error Loading OS.", NUL
ZeroPadding:
	 .org 510
BootSignature:
	.word X86_BOOT_SECTOR_SIGNATURE