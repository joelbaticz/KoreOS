/*
  - Date: 2018-01-17
  - Author: Joel
  - Purpose: Real Mode Loader entry for x86-32
*/

#include "../include/asminc.S"
#include "../mach/x86-32-pc/def.S"

/* This file is being included in the middle of the file */
/* 	#include "../mach/x86-32-pc/entry/entry_realmode_routines.S" */

.code16

start:

	/* Bochs pause */

    xchg bx,bx

	/* Disable Interrupts */
	cli

	/* Set-up Segment registers */
	xor ax,ax
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax

	/* Set-up Real Mode stack */
	mov sp,X86_STACK16

	/* Loader Starting message */
	mov si,offset MessageLoaderStarting	
	call PrintString

	/* Save info passed by Boot Sector, set-up Real Mode Reentry address */
	call InitializeLoader

	/* Enable A20 line */
	call EnableA20

	/* Verify PE Image and Calculate PE Entry Address */
	call VerifyPEImage

	/* Getting ready for Protected Mode - we want to initialize these things once */
	lgdt GlobalDescriptorTable
	
	jmp EnterProtectedMode
	
ErrorCorruptImage:

	/* If we get here something went wrong */
	mov si,offset MessageCorruptImage	
	call PrintString

	mov si,offset MessageSystemHalted	
	call PrintString

	jmp Halt

EnterRealMode:
	
/* */
.code32	
	
	/* Fix Segment registers to contain valid 16 bit Selectors */
	mov ax,X86_DATA16_SELECTOR
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	
	jmp X86_CODE16_SELECTOR:EnterRealModeContinue1
EnterRealModeContinue1:
/* */
.code16		
	
	/* Switch back to Real Mode */
	mov eax,cr0
	and eax,X86_CR0_PE_CLEAR
	mov cr0,eax
	
	/* Fix Segment registers so they contain Real Mode values */
	mov ax,0
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax

	/* Flush cache */
	jmp 0x0:EnterRealModeContinue2
EnterRealModeContinue2:

	/* Save 32bit Stack Pointer */
	mov dword ptr [X86_STACK32_STORED],esp
	

RealModeRequest:
	/* Set 16bit Stack Pointer */
	mov eax,dword ptr [X86_STACK16_STORED]
	mov esp,eax

	/* Call requested Real Mode routine */
	mov ebx,dword ptr [X86_FUNCTION_NUMBER]
	shl ebx,1
	add ebx,offset RealModeFunctionTable

	call word ptr [bx]

EnterProtectedMode:

	cli

	/* Save current Stack Pointer */
	mov eax,0
	mov ax,sp
	mov dword ptr [X86_STACK16_STORED],eax
	
	/* Set PE bit */
	mov eax,cr0
	or eax,X86_CR0_PE_SET
	mov cr0,eax

	/* Flush cache */
	jmp X86_CODE32_SELECTOR:EnterProtectedModeContinue

.code32
EnterProtectedModeContinue:
	/* Fix Segment registers to contain valid Selectors */
	mov ax,X86_DATA32_SELECTOR
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax

	/* Set-up Protected Mode stack */
	mov esp,dword ptr [X86_STACK32_STORED]

	/* Load Interrupt Descriptor Table */
	/* lidt idtptr */

	jmp dword ptr [X86_PROTECTED_MODE_CONTINUATION_ADDRESS]

ProtectedModeContinueAddress:
	
	/* Pass X86_LOADER_INFO_BLOCK to Main */
	mov eax,offset LoaderCommandLineOptions
	push eax

///!!!!!!!!!!!!!!!!!///

	call dword ptr ds:[X86_PROTECTED_MODE_LOADER_ENTRY_POINT]
	
	/* Hang if we ever get back here */
	jmp $

	/* Common Real Mode Helper functions */ 
	#include "../mach/x86-32-pc/entry/entry_realmode_routines.S"

MessageLoaderStarting:
	.ascii "Loader starting up...", CR, LF, NUL
MessageSystemHalted:
	.ascii "System halted.",CR, LF, NUL
MessageRealModeTerminate:
	.ascii "System Terminated after switching back to Real Mode.",CR, LF, NUL 
MessageCorruptImage:
	.ascii "Corrupt Loader image.",CR ,LF, NUL
LoaderCommandLineOptions:
	.ascii "DEBUG=trace", NUL
RealModeFunctionTable:
	.word FunctionReboot
	.word FunctionCallBiosInterrupt


.align 4

GlobalDescriptorTableData:
NullDescriptor:
	.long 0
	.long 0
Code32Descriptor:
	.word 0xffff		//Bits 0-15 - LimitLow
	.word 0			//Bits 16-31 - Base Low
	.byte 0			//Bits 32-39 - Base Mid
	.byte 0b10011010	//Bits 40-47 - Access Flag
				//Access bit = 0
				//Read/Write bit = 1 (Read and Execute)
				//Expansion Direction bit = 0
				//Code/Data bit = 1 = Code
				//System bit = 1
				//Priviledge level bits = 00b -> Ring 0
				//In memory bit = 1
	.byte 0b11001111	//Bits 48-55 - Granularity
				//Limit High bits = 1111b ->	Limit - 4GB
				//Reserved bit = 0
				//Reserved bit = 0
				//Segment Type (16/32bits) = 1 -> 32bits
				//Granularity = 1 -> 4KB
	.byte 0			//Bits 56-63 - Base High
Data32Descriptor:
	.word 0xffff		//Bits 0-15 - LimitLow 
	.word 0			//Bits 16-31 - Base Low
	.byte 0			//Bits 32-39 - Base Mid
	.byte 0b10010010	//Bits 40-47 - Access Flag
				//Access bit = 0
				//Read/Write bit = 1 (Read and Write)
				//Expansion Direction bit = 0
				//Code/Data bit = 0 = Data
				//System bit = 1
				//Priviledge level bits = 00b -> Ring 0
				//In memory bit = 1
	.byte 0b11001111	//Bits 48-55 - Granularity
				//Limit High bits = 1111b ->	Limit - 4GB
				//Reserved bit = 0
				//Reserved bit = 0
				//Segment Type (16/32bits) = 1 -> 32bits
				//Granularity = 1 -> 4KB
	.byte 0			//Bits 56-63 - Base High
Code16Descriptor:
	.word 0xffff		//Bits 0-15 - LimitLow 
	.word 0			//Bits 16-31 - Base Low
	.byte 0			//Bits 32-39 - Base Mid
	.byte 0b10011010	//Bits 40-47 - Access Flag
				//Access bit = 0
				//Read/Write bit = 1 (Read and Execute)
				//Expansion Direction bit = 0
				//Code/Data bit = 1 = Code
				//System bit = 1
				//Priviledge level bits = 00b -> Ring 0
				//In memory bit = 1
	.byte 0b10001111	//Bits 48-55 - Granularity
				//Limit High bits = 1111b ->	Limit - 4GB
				//Reserved bit = 0
				//Reserved bit = 0
				//Segment Type (16/32bits) = 0 -> 16bits
				//Granularity = 1 -> 4KB
	.byte 0			//Bits 56-63 - Base High
Data16Descriptor:
	.word 0xffff		//Bits 0-15 - LimitLow 
	.word 0			//Bits 16-31 - Base Low
	.byte 0			//Bits 32-39 - Base Mid
	.byte 0b10010010	//Bits 40-47 - Access Flag
				//Access bit = 0
				//Read/Write bit = 1 (Read and Write)
				//Expansion Direction bit = 0
				//Code/Data bit = 0 = Data
				//System bit = 1
				//Priviledge level bits = 00b -> Ring 0
				//In memory bit = 1
	.byte 0b10001111	//Bits 48-55 - Granularity
				//Limit High bits = 1111b ->	Limit - 4GB
				//Reserved bit = 0
				//Reserved bit = 0
				//Segment Type (16/32bits) = 0 -> 16bits
				//Granularity = 1 -> 4KB
	.byte 0			//Bits 56-63 - Base High
	
GlobalDescriptorTableDataEnd:

GlobalDescriptorTable:	
	.word GlobalDescriptorTableDataEnd - GlobalDescriptorTableData - 1	//Length of Global Descriptor Table
	.long GlobalDescriptorTableData						//Pointer to Global Descriptor Table

ZeroPadding:
	.org 1022
LoaderSignature:
	.word X86_LOADER_SIGNATURE

