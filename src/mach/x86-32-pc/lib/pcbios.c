/*
  - Date:		2018-03-11
  - Author:		Joel
  - Purpose:	PC Bios implementation:
				Machine specific implementation of Reboot, Bios interrupt calls etc.
*/

/* INCLUDES */
#include <kldr.h>

/* PRIVATE FUNCTION PROTOTYPES */
VOID CallRealModeFunction(PREGISTERS pRegsIn, PREGISTERS pRegsOut);

/* FUNCTIONS */	
VOID _cdecl PcBiosReboot()
{
	PUINT16 pFunctionNumber = (PUINT16)(X86_FUNCTION_NUMBER);
	
	*pFunctionNumber = 0; /* 0 = Reboot */
	
	CallRealModeFunction(NULL, NULL);
}

KSTATUS _cdecl PcBiosCallInterrupt(UINT8 interruptNumber, PREGISTERS pRegsIn, PREGISTERS pRegsOut)
{
	KSTATUS result = STATUS_ERROR_UNKNOWN;

	PUINT16 pFunctionNumber = (PUINT16)(X86_FUNCTION_NUMBER);
	*pFunctionNumber = 1; /* 1 = CallBiosInterrupt */
		
	//X86_INTERRUPT_NUMBER_ADDRESS is a pointer that points to a byte, which holds the interrupt number
	PUINT8* pInterruptNumber = (PUINT8*)(X86_INTERRUPT_NUMBER_ADDRESS);
		
	//TRACE("Pointer to Interrupt Number: %x", *pInterruptNumber);
	//DebugPause();

	UiDrawText("PcBiosCallInterrupt()");    
    //HALT;

	
	**pInterruptNumber = interruptNumber;	

	biosIdtRegister.base = 0;
	biosIdtRegister.limit = 1023; 

	PcIoDisableInterrupts();

	asm volatile ("lidt %0" :: "m" (biosIdtRegister));

	CallRealModeFunction(pRegsIn, pRegsOut);

	asm volatile ("lidt %0" :: "m" (koreIdtRegister));

	PcIoDisableInterrupts();
	//PcIoEnableInterrupts();
		
	result = STATUS_SUCCESS;
	return result;
}

VOID CallRealModeFunction(PREGISTERS pRegsIn, PREGISTERS pRegsOut)
{
	/* 
		While calling Real Mode Functions you have to make sure that the registers hold 16-bit
		SEGMENT:OFFSET addresses. Furthermore only the lower 16-bit part of the FLAGS and the
		segment registers are usable.
	*/
		
	if (pRegsIn)
	{
		/* Copy registers to x86 Info Block */
		int32_t* registers = (int32_t*)(X86_REGISTER_EAX);//(0x800 + 32); // 880??? Parenthesis???
		*registers = (int32_t) pRegsIn->EAX;
		registers++;
		*registers = (int32_t) pRegsIn->EBX;
		registers++;
		*registers = (int32_t) pRegsIn->ECX;
		registers++;
		*registers = (int32_t) pRegsIn->EDX;
		registers++;
		*registers = (int32_t) pRegsIn->ESI;
		registers++;
		*registers = (int32_t) pRegsIn->EDI;
		registers++;
		*registers = (int32_t) pRegsIn->EBP;
		registers++;
		
		*registers = (int32_t) pRegsIn->DS;
		registers++;
		*registers = (int32_t) pRegsIn->ES;
		registers++;
		*registers = (int32_t) pRegsIn->FS;
		registers++;
		*registers = (int32_t) pRegsIn->GS;
		registers++;
		
		*registers = (int32_t) pRegsIn->EFLAGS;
	}
	
	/* A pointer to the RealModeHandler is stored at address X86_REAL_MODE_HANDLER_ADDRESS */
	int32_t* RealModeHandlerAddress = (int32_t*)(X86_REAL_MODE_HANDLER_ADDRESS);
	//VOID (*RealModeHandler)() = (void*)(*RealModeHandlerAddress);	
	
	/* Set the continuation address (return address) */
	int32_t* RealModeReturnAddress = (int32_t*)(X86_PROTECTED_MODE_CONTINUATION_ADDRESS);	
	*RealModeReturnAddress = (int32_t)(&&ReturnAddress);
	
	// It looks like something is wrong when the RealMode funcitons returns
	// In some cases the stack segment / pointer looks totally off
	//DebugPause();
DebugPause();	
	
	/* Save actual values of processor registers */
	asm volatile(
		"push %eax;"	
		"push %ebx;"	
		"push %ecx;"	
		"push %edx;"	
		"push %esi;"	
		"push %edi;"	
		"push %ebp;"	
		"pushf;"
		"push %ds;"
		"push %es;"
		"push %fs;"
		"push %gs;"
	);
	
	/* Calling the Real Mode Handler */
	asm(
		"jmp *%0;"
		: /* No output operand. */
		: "r" (*RealModeHandlerAddress)
	);
		
ReturnAddress:
	
	//asm("xchg %bx, %bx;");
	
	/* Restore original processor registers */
	asm volatile(
		"pop %gs;"
		"pop %fs;"
		"pop %es;"
		"pop %ds;"
		"popf;"
		"pop %ebp;"
		"pop %edi;"
		"pop %esi;"
		"pop %edx;"
		"pop %ecx;"
		"pop %ebx;"
		"pop %eax;"
	);

DebugPause();

	if (pRegsOut)
	{
		/* Copy registers back from x86 Info Block */
		int32_t* registers = (int32_t*)(X86_REGISTER_EAX);//(0x800 + 32); // 880??? Parenthesis???
		pRegsIn->EAX = *registers;
		registers++;
		pRegsIn->EBX = *registers;
		registers++;
		pRegsIn->ECX = *registers;
		registers++;
		pRegsIn->EDX = *registers;
		registers++;
		pRegsIn->ESI = *registers;
		registers++;
		pRegsIn->EDI = *registers;
		registers++;
		pRegsIn->EBP = *registers;
		registers++;
		
		pRegsIn->DS = *registers;
		registers++;
		pRegsIn->ES = *registers;
		registers++;
		pRegsIn->FS = *registers;
		registers++;
		pRegsIn->GS = *registers;
		registers++;
		
		pRegsIn->EFLAGS = *registers;
	}

	return;
}
