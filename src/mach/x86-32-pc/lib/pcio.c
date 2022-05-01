/*
  - Date: 2018-03-10
  - Author: Joel
  - Purpose: PC Input/Output implementation:
			 Machine specific port I/O functions.
*/

#include <kldr.h>

typedef struct _IDT_DESCRIPTOR /* Interrupt Descriptior Table Entry */
{
    UINT16  isrLow;     // Lower 16bits of the ISR's address
    UINT16  kernelCS;   // The GDT segment selector that the CPU will load into CS before calling the ISR
    UINT8   reserved;   // Reserved, set to 0
    UINT8   attributes; // Type and attributes flags
    UINT16  isrHigh;    // Higher 16bits of the ISR's address
} __attribute__((packed)) IDT_DESCRIPTOR, *PIDT_DESCRIPTOR;

__attribute__((aligned(0x10)))
IDT_DESCRIPTOR idtEntries[256] = { 0 };

VOID SetInterruptVector(UINT8 vector, UINT32 interruptServiceRoutine, UINT8 flags);

VOID PcIoWritePortUint8(UINT16 port, UINT8 data)
{
	//asm volatile("xchg %bx, %bx");

	// asm
	// (
	// 	"mov %0, %%al;"
	// 	"mov %1, %%dx;"
	// 	"outb %%al, %%dx;"
	// 	 : /* No output operand. */
	// 	 : "r"(data), "r"(port)
	// );

	asm volatile("outb %0, %1" : /* No output operand. */ : "a"(data), "Nd"(port));
}

UINT8 PcIoReadPortUint8(UINT16 port)
{
	UINT8 result;

	asm volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
	
	return result;
}

VOID PcIoWritePortUint16(UINT16 port, UINT16 data)
{
	NOT_IMPLEMENTED;
}

UINT16 PcIoReadPortUint16(UINT16 port)
{
	NOT_IMPLEMENTED;
}

VOID PcIoDelay()
{
	// Performs a delay between reading/writing ports - required on older machines
	PcIoWritePortUint8(0x80, 0);
}

VOID PcIoDisableInterrupts(UINT32 interruptNumber)
{
	asm volatile("cli");
}

VOID PcIoEnableInterrupts(UINT32 interruptNumber)
{
	asm volatile("sti");
}

VOID PcIoAcknowledgeInterrupt(UINT32 interruptNumber)
{
	// Master PIC
	
	PcIoWritePortUint8(0x20, 0x20);

	if (interruptNumber >= 8)
	{
		// Slave PIC
		PcIoWritePortUint8(0xA0, 0x20);
	}
}

VOID PcIoGetInterruptDescriptionTableRegister(OUT PIDT_REGISTER* pIdtRegister)
{
	IDT_REGISTER reg;

	asm volatile ("sidt %0" : "=m"(reg));

	(*pIdtRegister)[0] = reg;
}

VOID PcIoSetInterruptDescriptionTableRegister(PIDT_REGISTER pIdtRegister)
{
	IDT_REGISTER reg = *pIdtRegister;

	asm volatile ("lidt %0" :: "m"(reg));
}

VOID PcIoInitializeInterruptVectorTable(PISR_VTBL pIsrFunctions)
{
	//PcIoGetInterruptDescriptionTableRegister(&biosIdtRegister);
	//asm volatile ("sidt %0" : "=m" (biosIdtRegister));

	//return;

	//TRACE("Init IVT\0");

	// Set Ist functions in the IDTEntries
	// Load the idt table

	// Set the IDT register to the beginning of the IDT table
 	koreIdtRegister.base = (UINT32) &idtEntries[0];
    koreIdtRegister.limit = (UINT16) (pIsrFunctions->isrCount * sizeof(IDT_DESCRIPTOR) - 1);	

	// Set the interrupt table entries
	for(UINT32 i = 0; i < pIsrFunctions->isrCount; i++)
	{
		UiDrawText("SettingInterrupt................DONE");
		SetInterruptVector(i, pIsrFunctions->isr[i], 0x8E);
	}

	// Load a different interrupt table in the IDT register

	// Ezt csak ugy fogom tudni debugolni, ha debug pointoot allitok fel itt
	// Lehetseges okok:
	// - a register nincs align-olva
	// - a stack belelog valamibe es felulirjuk
	// - vagy valami garbage van az IsrFunction-okben
	//asm volatile ("lidt %0" : /* No output operand. */ : "memory"(idtRegister));

	//asm volatile("lidt (%0)" : : "r" (&idtRegister));
	asm volatile ("lidt %0" :: "m" (koreIdtRegister));
	//PcIoSetInterruptDescriptionTableRegister(&idtRegister);

	// TRACE <- Shouldn't call trace as it call the BIOS and freezes as the interrupts are mingle now
}

VOID SetInterruptVector(UINT8 vector, UINT32 interruptServiceRoutine, UINT8 flags)
{
	PIDT_DESCRIPTOR descriptor = &idtEntries[vector];

    descriptor->isrLow       = (UINT16) (interruptServiceRoutine & 0xFFFF);
    descriptor->isrHigh      = (UINT16) (interruptServiceRoutine >> 16);	
    descriptor->kernelCS     = 0x08; // This value represents the offset of the kernel code selector in the GDT
    descriptor->attributes   = flags;
    descriptor->reserved     = 0;
}