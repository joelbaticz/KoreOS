/*
  - Date: 2020-11-05
  - Author: Joel
  - Purpose: FAT12 Volume Boot Record for X86
*/

// -------------------------------------
// Public Definitions
// -------------------------------------

#include "../../../../../include/asminc.S"
#include "../../../def.S"

// -------------------------------------
// Private Definitions
// VBR Uninitialized Data Section
// -------------------------------------

#define X86_VBR_DATA_SECTION_AREA               0x7b80
#define X86_VBR_DATA_SECTION_BASE		0x7c00
#define X86_VBR_OFFSET_TO_DATA_SECTION_AREA	X86_VBR_DATA_SECTION_AREA - X86_BOOT_START_OFFSET
#define X86_VBR_BOOT_DRIVE_ID			X86_VBR_OFFSET_TO_DATA_SECTION_AREA
#define X86_VBR_BOOT_PARTITION_ID		X86_VBR_OFFSET_TO_DATA_SECTION_AREA + 1
#define X86_VBR_MAX_SECTORS			X86_VBR_OFFSET_TO_DATA_SECTION_AREA + 2
#define X86_VBR_MAX_HEADS			X86_VBR_OFFSET_TO_DATA_SECTION_AREA + 3
#define X86_VBR_STARTING_SECTOR_OF_DATA_AREA	X86_VBR_OFFSET_TO_DATA_SECTION_AREA + 4
#define X86_VBR_CLUSTER_NUMBER			X86_VBR_OFFSET_TO_DATA_SECTION_AREA + 8

// -------------------------------------
// BIOS Parameter Block Descriptors
// -------------------------------------

#define X86_VBR_NUMBER_OF_COPIES_OF_FAT         NumberOfCopiesOfFAT - X86_BOOT_START_OFFSET
#define X86_VBR_SECTORS_PER_CLUSTER		SectorsPerCluster - X86_BOOT_START_OFFSET
#define X86_VBR_NUMBER_OF_SECTORS_PER_FAT	NumberOfSectorsPerFAT - X86_BOOT_START_OFFSET
#define X86_VBR_NUMBER_OF_HIDDEN_SECTORS	NumberOfHiddenSectors - X86_BOOT_START_OFFSET
#define X86_VBR_NUMBER_OF_RESERVED_SECTORS	NumberOfReservedSectors - X86_BOOT_START_OFFSET
#define X86_VBR_BYTES_PER_SECTOR		BytesPerSector - X86_BOOT_START_OFFSET
#define X86_VBR_NUMBER_OF_ROOT_DIR_ENTRIES	NoOfRootDirEntries  - X86_BOOT_START_OFFSET

// -------------------------------------
// Constants
// -------------------------------------

#define X86_VBR_DISK_BUFFER_AREA_OFFSET		0x0600
#define X86_VBR_LOADER_SIGNATURE_OFFSET		X86_LOADER_BASE + 0x03fe

// For FAT32 = 0x0FFFFFF6, FAT16 = 0x0FFF6, FAT12 = 0x0FF6
#define X86_VBR_FAT_END_OF_CLUSTER_FLAG		0x0FF6

.code16

start:
	jmp Loader
	nop

BIOSParameterBlockFAT12:
OEMName:			.ascii "MSDOS5.0"
BytesPerSector:			.word 512
SectorsPerCluster:		.byte 1
NumberOfReservedSectors:	.word 1
NumberOfCopiesOfFAT:		.byte 2
NoOfRootDirEntries:		.word 224
NoOfTotalSectors:		.word 2880
MediaDescriptor:		.byte 0xf0
NumberOfSectorsPerFAT:		.word 9
NumberOfSectorsPerTrack:	.word 18
NumberOfHeads:			.word 2
NumberOfHiddenSectors:		.long 0
NumberOfLargeSectors:		.long 0
PhysicalDriveNumberOfPartition:	.byte 0
Flags:				.byte 0
ExtendedSignature:		.byte 0x29
VolumeSerialNumber:		.byte 0x67, 0x0F, 0xEC, 0x14
VolumeLabel:			.ascii "NO NAME    "
FileSystemName:			.ascii "FAT12   "

Loader:
// xchg bx, bx
	xor ax,ax
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov sp,X86_STACK16

	mov bp,X86_VBR_DATA_SECTION_BASE

	// Save BootDriveID and BootPartitionID for later
	// Boot Drive ID is needed for VBS (DO NOT REMOVE!)
	mov byte ptr [bp+X86_VBR_BOOT_DRIVE_ID], dl

	// mov byte ptr [bp+X86_VBR_BOOT_PARTITION_ID], dh
	push dx
	
	// Reset Disk System, DL = DriveID
	int X86_BIOS_DISK_FUNCTION
	jc ErrorLoadingOS

	mov ah,X86_BIOS_DISK_FUNCTION_GET_DRIVE_PARAMETERS
	int X86_BIOS_DISK_FUNCTION
	jc ErrorLoadingOS

// xchg bx, bx

	// Get Drive Parameters will set ES:DI = pointer to drive parameter table (for floppies only)
	// Need to reset ES, because later the string comparision will fail if we don't
	xor ax,ax
	mov es,ax
	
	// Calculate MaxSectors
	and cl,X86_BIOS_MAX_DRIVE_SECTOR_COUNT	
	mov byte ptr [bp+X86_VBR_MAX_SECTORS],cl

	// Calculate MaxHeads
	inc dh
	mov byte ptr [bp+X86_VBR_MAX_HEADS],dh

	// Caculate Starting Sector of Data Area / RootDir
	movzx eax,byte ptr [bp+X86_VBR_NUMBER_OF_COPIES_OF_FAT]			// EAX = 2

	movzx ebx,word ptr [bp+X86_VBR_NUMBER_OF_SECTORS_PER_FAT]       // EBX = 9
	mul ebx															// EAX = 18

	movzx ebx,word ptr [bp+X86_VBR_NUMBER_OF_RESERVED_SECTORS]      // EBX = 1
	add eax,ebx													    // EAX = 19

	add eax,dword ptr [bp+X86_VBR_NUMBER_OF_HIDDEN_SECTORS] 	    // EAX = 19 - OK

	mov dword ptr [bp+X86_VBR_STARTING_SECTOR_OF_DATA_AREA],eax

	// Calculate Sectors covered by RootDirEntries
	movzx edx, word ptr [bp+ X86_VBR_NUMBER_OF_ROOT_DIR_ENTRIES]
	shr edx,4 // One record is 32 bytes long and one sector is 512 byte ( << 6 >> 10 = >> 4) 
	          // EDX = 224 * 32 / 512 = 224 >> 4 = 14 - OK

LoadNextRootDirSector:
	mov ebx,X86_VBR_DISK_BUFFER_AREA_OFFSET
	mov di,bx

	call LoadSector
// xchg bx, bx

	add bx,512

	// Perform searching Directory Entries
NextDirectoryEntry:
	mov cx,11
	mov si,offset LoaderString
	repz cmpsb
	jz LoaderFound

	add di,cx
	add di,21
	cmp di,bx
	jb NextDirectoryEntry

	// EDX = Remaining sectors, zero flag will be set when it hits 0
	dec edx 
	jnz LoadNextRootDirSector

	// All Directory Entry sectors have been checked and loader was not found
	jmp ErrorLoadingOS

LoaderFound:

// xchg bx, bx
	// EBX = target buffer
	mov ebx,X86_LOADER_BASE

	// Get starting Cluster Number (it's 32bits and stored in 2x16bit offsets)
	mov ax,word ptr [di+9]
	shl eax,16
	mov ax,word ptr [di+15]
CheckCluster:
	mov dword ptr [bp+X86_VBR_CLUSTER_NUMBER],eax
	sub eax,2
	cmp eax,X86_VBR_FAT_END_OF_CLUSTER_FLAG
	jae ErrorLoadingOS

ProcessNextCluster:
	movzx ecx, byte ptr [bp + X86_VBR_SECTORS_PER_CLUSTER]
	mul ecx

	add eax,dword ptr [bp + X86_VBR_STARTING_SECTOR_OF_DATA_AREA]

	// Add sectors occupied by RootDir entries
	movzx edx, word ptr [bp+ X86_VBR_NUMBER_OF_ROOT_DIR_ENTRIES]
	// One record is 32 bytes long and one sector is 512 byte ( << 6 >> 10 = >> 4)
	shr edx,4 
	add eax,edx

ProcessNextSector:
// ------------
	pushad
	mov si, offset MessageDot
	call PrintString
	popad

// ------------
	call LoadSector
// xchg bx, bx

	add ebx,512
	loop ProcessNextSector
	
	call FindNextCluster


// DO WE NEED THIS ERROR CHECK???
// FindNextCluster already aborted if there was an error with loading the sector!

	jc ErrorLoadingOS

// xchg bx, bx
	
	// EAX - contains next cluster
	mov dword ptr [bp + X86_VBR_CLUSTER_NUMBER],eax
	sub eax,2
	cmp eax,0x0FF6 //X86_VBR_FAT_END_OF_CLUSTER_FLAG
	jb ProcessNextCluster	

	/* All sectors loaded */

// -- CHECK --
CheckHalt:
	//jmp CheckHalt

// -- CHECK-END --


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

	// DX = Boot Drive Id (BIOS Hdd Id) passed to KLDR.SYS

	jmp 0x0:X86_LOADER_BASE

	// We never come back here as we transferred execution with jump!

FindNextCluster:

// xchg bx,bx

	push ebx
	// push edx - NOT BEING USED IN CALLER PROCESSES

	xor edx,edx
	mov eax,dword ptr [bp+X86_VBR_CLUSTER_NUMBER]
	
	// KLDR.SYS starts from Cluster 2. This is the ClusterNumber that's in EAX now
	
	// For FAT32 - Shift by 2 = each cluster number is 4 bytes -> 32 bits
	// For FAT16 - Shift by 1 = each cluster number is 2 bytes -> 16 bits
	// shl eax,2

	// For FAT12 - Multiply by 3 and shift right 1 since each cluster number is 1.5bytes long
	// Carry flag will be set if cluster offset is ODD
	mov ebx, 3
	mul ebx         // EAX = 6
	shr eax,1       // EAX = 3
	
	// EAX is the offset from 0x0600 pointing to the FAT entry that needs to be investigated

	// In the Flags we store whether the cluster offset was ODD
	// LoadSector overwrites the Flags, so we save it
	pushf

	// Calculate which FAT sector the cluster info will be found 
	movzx ebx,word ptr [bp+X86_VBR_BYTES_PER_SECTOR]
	div ebx

	// EDX = reminder = 3 (offset in sector to FAT entry)
	// EAX = sector number = 0

	add eax,dword ptr [bp+X86_VBR_NUMBER_OF_HIDDEN_SECTORS]
	
	movzx ebx,word ptr [bp+X86_VBR_NUMBER_OF_RESERVED_SECTORS]
	add eax,ebx

    // HOW ABOUT ROOTDIR SECTORS???
	// RootDir sectors are after the FATs, so don't need to add them!

	// Make sure EBX points to Buffer = 0x600
	mov ebx,X86_VBR_DISK_BUFFER_AREA_OFFSET
	
	// LoadSector keeps all general registers intact, but EAX will be incremented
	call LoadSector
	
// xchg bx,bx

	// LoadSector already handled if the sector could not be loaded!

	// sub bx,512 - WHY DO WE DO THIS??? IF LOADSECTOR WOULDN'T INCREMENT WE WOULD BE BETTER OFF
	add bx,dx
	
	// Make sure only the needed part is copied!
	movzx eax,word ptr [bx]

	// Only restore Carry-Flag now (instructions before could overwrite it!)
	popf

	jnc ClusterEven
ClusterOdd:
	shr eax, 4
ClusterEven:
	and eax, 0xFFF
ErrorLoadingSector:

xchg bx,bx

	// pop edx
	pop ebx
	
	ret


LoadSector:

	// ES and the general register will be touched by the BIOS load sector function
	push es
	pushad

	// EBX = Memory address where the routine should load the sector
	// ES = Convert address to Segments
	shr ebx,4
	mov es,bx
	
	// DX:AX = Stores the 32bit LBA of the sector to load
	mov dx,ax
	shr eax,16
	xchg dx,ax

	// Divide DX:AX by MaxSectors
	movzx bx,byte ptr [bp+X86_VBR_MAX_SECTORS]
	div bx

	// Calculate number of Heads
	inc dx
	mov cl,dl

	movzx bx,byte ptr [bp+X86_VBR_MAX_HEADS]

	xor dx,dx
	div bx

	// Calculate Cylinders and Sectors
	mov ch,al
	xchg dl,dh

	// DS:BX = Segment:Offset pair where to load data
	xor bx,bx
	mov dl,byte ptr [bp + X86_VBR_BOOT_DRIVE_ID]
	mov ax,0x0202 // AH = Read Sectors Command, AL = number of sectors to read (it's 2 to handle overlapped entries in FAT12 table)
	
	int X86_BIOS_DISK_FUNCTION
	
	// Restore registers even when there is a problem
	popad
	pop es
	
	//jc ErrorLoadingOS

	// Set EAX to next sector
	inc eax

	ret	
	
ErrorLoadingOS:

	mov si,offset MessageErrorLoadingOS
	call PrintString

	sti
	jmp $

BootingOS:
	mov si,offset MessageLoadingOS

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
MessageLoadingOS:
	//.ascii "Loading...", NUL
// ------------
MessageDot:
    .ascii ".", NUL
// ------------
ZeroPadding:
	 .org 510
BootSignature:
	.word X86_BOOT_SECTOR_SIGNATURE