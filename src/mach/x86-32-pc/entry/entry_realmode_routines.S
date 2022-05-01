/*
  - Date: 2018-01-17
  - Author: Joel
  - Purpose: Real Mode Loader common helper functions
*/

#define X86_KEYBOARD_CTRL_COMMAND_PORT 0x64
#define X86_KEYBOARD_CTRL_DATA_PORT 0x60

.code16

/* ---=== PRINT ===--- */

PrintString:
	lodsb

	or al,al
	jz PrintStringDone

	mov ah,X86_BIOS_VIDEO_FUNCION_TELETYPE_OUTPUT
	mov bh,0
	mov bl,X86_TEXT_COLOR_ATTRIBUTE_GRAY_ON_BLACK
	int X86_BIOS_VIDEO_FUNCTION

	jmp PrintString
PrintStringDone:
	ret

/* ---=== HALT ===--- */

Halt:
	cli
	jmp $

/* ---=== INITIALIZE LOADER ===--- */

InitializeLoader:
	/* Save Drive ID to x86 Info Block */
	xor eax,eax
	mov al,dl
	mov dword ptr [X86_BOOT_DRIVE_ID],eax
	
	/* Save Partition ID to x86 Info Block */
	xor eax,eax
	mov al,dh
	mov dword ptr [X86_BOOT_PARTITION_ID],eax

	/* Save Real Mode Handler's address to the x86 Info Block */
	mov eax,offset EnterRealMode
	mov dword ptr [X86_REAL_MODE_HANDLER_ADDRESS],eax

	/* Save Protected Mode Continue address to the x86 Info Block */
	mov eax,offset ProtectedModeContinueAddress
	mov dword ptr [X86_PROTECTED_MODE_CONTINUATION_ADDRESS],eax

	/* Save Address of Interrupt Number to the x86 Info Block */
	mov eax,offset RealModeInterruptNumberAddress
 	mov dword ptr [X86_INTERRUPT_NUMBER_ADDRESS],eax

	/* Set up default Protected Mode Stack */
	mov eax,X86_STACK32
	mov dword ptr [X86_STACK32_STORED],eax
	
	ret
	
/* ---=== A20 ENABLE ===--- */

EnableA20:
	pusha

	call WaitForKeyboardController
	
	/* Write to command port */
	mov al,0xd1
	out X86_KEYBOARD_CTRL_COMMAND_PORT,al

	call WaitForKeyboardController

	/* Enable A20 line */
	mov al,0xdf
	out X86_KEYBOARD_CTRL_DATA_PORT,al	

	call WaitForKeyboardController

	popa

	ret

WaitForKeyboardController:

PollController:
	
	in al,X86_KEYBOARD_CTRL_COMMAND_PORT
	cmp al,0xff
	jz ControllerReady

	/* 0x02 flag is set if controller is NOT ready */
	test al,2
	jnz PollController
ControllerReady:
	ret

/* ---=== VERIFY PE IMAGE ===--- */

VerifyPEImage:
	
	/* Load Loader PE Executable Start to Segment Register */
	push es
	mov ax,X86_LOADER_MZ_BASE / 16
	mov es,ax

	/* Check MZ signature */
	mov ebx,0
	cmp word ptr es:[ebx],X86_LOADER_MZ_SIGNATURE
	jnz ErrorCorruptImage
	
	/* Locate PE Image start */
	mov ebx,es:[ebx+0x3c]	

	/* Check PE signature */
	cmp dword ptr es:[ebx],X86_LOADER_PE_SIGNATURE
	jnz ErrorCorruptImage	

	/* Check Machine Flag (0x14c = i386) */
	cmp word ptr es:[ebx+0x04],X86_LOADER_MACHINE_FLAG_I386
	jnz ErrorCorruptImage	

	/* Check Optional Header signature */
	cmp word ptr es:[ebx+0x18],X86_LOADER_OPTIONAL_HEADER_SIGNATURE
	jnz ErrorCorruptImage

	/* Loader Protected Mode Entry point = AddressOfEntryPoint + ImageBase */ 
	mov eax,es:[ebx+0x28]
	add eax,es:[ebx+0x34]
	mov dword ptr ds:[X86_PROTECTED_MODE_LOADER_ENTRY_POINT],eax

	pop es

	ret

/* ---=== REBOOT ===--- */

FunctionReboot:

//xchg bx,bx

	cli

	/* Set word at location 0x40:0x72 to 0 (specify cold reboot) */
	mov word ptr ds:[0x0472], 0

	/* Jump to the Power On Reset Vector location at 0xf000:0xfff0 to perform reboot */
	jmp 0xf000:0xfff0

/* ---=== CALL BIOS INTERRUPT ===--- */
FunctionCallBiosInterrupt:

//xchg bx,bx

	//call LoadRegisters
	jmp LoadRegisters

LoadRegistersAfter:	

	.byte 0xcd	/* Interrupt opcode */
RealModeInterruptNumberAddress:
	.byte 0xff	/* Interrupt number */
	//.byte 0x10	/* Interrupt number */

	cli

	//call SaveRegisters
	jmp SaveRegisters

SaveRegistersAfter:	

	ret


	
	
	
LoadRegisters:

	/* Using only the lower 16-bit of the FLAGS register */
	mov eax,dword ptr cs:[X86_REGISTER_EFLAGS]
	push ax
	popf

	mov eax,dword ptr cs:[X86_REGISTER_DS]
	mov ds,ax
	mov eax,dword ptr cs:[X86_REGISTER_ES]
	mov es,ax
	mov eax,dword ptr cs:[X86_REGISTER_FS]
	mov fs,ax
	mov eax,dword ptr cs:[X86_REGISTER_GS]
	mov gs,ax
	
	mov eax,dword ptr cs:[X86_REGISTER_EAX]
	mov ebx,dword ptr cs:[X86_REGISTER_EBX]
	mov ecx,dword ptr cs:[X86_REGISTER_ECX]
	mov edx,dword ptr cs:[X86_REGISTER_EDX]
	mov esi,dword ptr cs:[X86_REGISTER_ESI]
	mov edi,dword ptr cs:[X86_REGISTER_EDI]
	mov ebp,dword ptr cs:[X86_REGISTER_EBP]
	
	//ret
	jmp LoadRegistersAfter
	
SaveRegisters:
	
	mov dword ptr cs:[X86_REGISTER_EAX],eax
	mov dword ptr cs:[X86_REGISTER_EBX],ebx
	mov dword ptr cs:[X86_REGISTER_ECX],ecx
	mov dword ptr cs:[X86_REGISTER_EDX],edx
	mov dword ptr cs:[X86_REGISTER_ESI],esi
	mov dword ptr cs:[X86_REGISTER_EDI],edi
	mov dword ptr cs:[X86_REGISTER_EBP],ebp	
	
	mov eax,0
	mov ax,ds
	mov dword ptr cs:[X86_REGISTER_DS],eax
	mov eax,0
	mov ax,es
	mov dword ptr cs:[X86_REGISTER_ES],eax
	mov eax,0
	mov ax,fs
	mov dword ptr cs:[X86_REGISTER_FS],eax
	mov eax,0
	mov ax,gs
	mov dword ptr cs:[X86_REGISTER_GS],eax

	/* Using only the lower 16-bit of the FLAGS register */
	mov eax,0
	pushf
	pop ax
	mov dword ptr cs:[X86_REGISTER_EFLAGS],eax	
	
	//ret
	jmp SaveRegistersAfter

	
	