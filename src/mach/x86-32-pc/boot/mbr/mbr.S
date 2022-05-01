/*
  - Date: 2018-01-12
  - Author: Joel
  - Purpose: Master Boot Record loader for X86
*/

/* Public Definitions */
#include "../../../../include/asminc.S"
#include "../../def.S"

/* Private Definitions */
/* MBR */
#define X86_MBR_RELOCATION_ADDRESS			0x0600


.code16

Start:

	cli
	
	xor cx,cx
	mov ds,cx
	mov es,cx
	mov fs,cx
	mov gs,cx
	mov ss,cx

	mov sp,X86_STACK16

	cld
	mov si,X86_BOOT_START_OFFSET
	mov di,X86_MBR_RELOCATION_ADDRESS
	mov ch,1
	rep movsw

	mov ax,offset RelocatedStart

	push ax
	ret

RelocatedStart:

	sti

	mov byte ptr ds:[BootDriveID],dl	


	mov bx,offset PartitionTable
	mov cx,4
NextEntry:
	mov al,byte ptr [bx]
	cmp al,X86_ACTIVE_PARTITION_FLAG
	jz ActiveFound

	cmp al,X86_INACTIVE_PARTITION_FLAG
	jnz InvalidPartitionTable

	add bx,16
	loop NextEntry

	/* No active partition present */
	jmp InvalidPartitionTable

ActiveFound:

	/* Check rest of the partitions (the rest should be Inactive (0x0)) */

	mov word ptr ds:[PartitionOffset],bx
	mov ax,4
	sub ax,cx
	mov byte ptr ds:[BootPartitionID],al

	dec cx
NextEntry2:
	add bx,16
	mov al,[bx]
	cmp al,X86_INACTIVE_PARTITION_FLAG
	jnz InvalidPartitionTable
	loop NextEntry2
	
ContinueBooting:
	/* Partition Table Correct */
	
	mov bx,word ptr [PartitionOffset]
	mov al,[bx+4]
	cmp al,X86_PARTITION_TYPE_FAT32LARGE
	jz LoadBootSector

	cmp al,X86_PARTITION_TYPE_FAT32SMALL
	jz LoadBootSector

	jmp ErrorLoadingOS

LoadBootSector:

	mov ah,X86_BIOS_DISK_FUNCTION_RESET_DISK_SYSTEM
	mov dl,byte ptr [BootDriveID]
	int X86_BIOS_DISK_FUNCTION
	jc DiskIOError

	mov ah,X86_BIOS_DISK_FUNCTION_GET_DRIVE_PARAMETERS
	mov dl,byte ptr [BootDriveID]
	int X86_BIOS_DISK_FUNCTION
	jc DiskIOError

	and cl,X86_BIOS_MAX_DRIVE_SECTOR_COUNT
	mov byte ptr [MaxSectors],cl
	
	inc dh
	mov byte ptr [MaxHeads],dh

	mov bx,word ptr [PartitionOffset]
	mov eax,[bx+8]
	
	mov dx,ax
	shr eax,16
	xchg dx,ax

	movzx bx,byte ptr [MaxSectors]
	div bx

	inc dx

	mov cl,dl
	movzx bx,byte ptr [MaxHeads]

	xor dx,dx
	div bx

	mov ch,al
	xchg dl,dh

	mov dl,byte ptr [BootDriveID]
	mov bx,X86_BOOT_START_OFFSET
	mov ah,X86_BIOS_DISK_FUNCTION_READ_SECTOR
	mov al,1
	int X86_BIOS_DISK_FUNCTION
	jc DiskIOError

	cmp word ptr [X86_BOOT_SECTOR_SIGNATURE_OFFSET], X86_BOOT_SECTOR_SIGNATURE
	jne MissingOS

	mov dl,byte ptr [BootDriveID]
	mov dh,byte ptr [BootPartitionID]

	jmp 0x0:X86_BOOT_START_OFFSET

InvalidPartitionTable:

	mov si,offset MessageInvalidPartitionTable
	call PrintString

	jmp Halt

DiskIOError:

	mov si,offset MessageDiskIOError
	call PrintString
	
	jmp Halt

ErrorLoadingOS:

	mov si,offset MessageErrorLoadingOS
	call PrintString

	jmp Halt 

MissingOS:
	
	mov si,offset MessageMissingOS
	call PrintString

Halt:
	mov si,offset MessageHalt
	call PrintString

	sti
	jmp $

PrintString:
	push es
	push si
	push di

PrintStringLoop:
	lodsb

	or al,al
	jz PrintStringDone

	mov ah,X86_BIOS_VIDEO_FUNCION_TELETYPE_OUTPUT
	mov bh,0
	mov bl,X86_TEXT_COLOR_ATTRIBUTE_GRAY_ON_BLACK
	int X86_BIOS_VIDEO_FUNCTION

	jmp PrintStringLoop

PrintStringDone:
	pop di
	pop si
	pop es
	ret

BootDriveID:
	.byte 0
BootPartitionID:
	.byte 0
PartitionOffset:
	.word 0
MaxSectors:
	.byte 0
MaxHeads:
	.byte 0
MessageInvalidPartitionTable:
	.ascii "Invalid partition table.", CR, LF, NUL
MessageErrorLoadingOS:
	.ascii "Error loading operating system.", CR, LF, NUL
MessageDiskIOError:
	.ascii "Disk I/O error.", CR, LF, NUL
MessageMissingOS:
	.ascii "Missing operating system", CR, LF, NUL
MessageHalt:
	.ascii "Press Ctrl+Alt+Delete to reboot...", CR, LF, NUL
ZeroPadding:
	.org 446
PartitionTable:
	.byte 0x80, 0x02, 0x03, 0x00, 0x0c, 0xfc, 0x56, 0xe6, 0x80, 0x00, 0x00, 0x00, 0x00, 0x60, 0x77, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	.byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
MBRSignature:
	.word X86_BOOT_SECTOR_SIGNATURE
