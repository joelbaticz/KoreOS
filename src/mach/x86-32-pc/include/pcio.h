/*
  - Date: 2018-03-17
  - Author: Joel
  - Purpose: Definitions for PC Input/Output functions
*/

typedef struct _IDT_REGISTER {
	UINT16	limit;
	UINT32	base;
} __attribute__((packed)) IDT_REGISTER, *PIDT_REGISTER;

static IDT_REGISTER biosIdtRegister;
static IDT_REGISTER koreIdtRegister;

VOID PcIoWritePortUint8(UINT16 port, UINT8 data);
UINT8 PcIoReadPortUint8(UINT16 port);
VOID PcIoWritePortUint16(UINT16 port, UINT16 data);
UINT16 PcIoReadPortUint16(UINT16 port);
VOID PcIoDelay();
VOID PcIoDisableInterrupts();
VOID PcIoEnableInterrupts();
VOID PcIoAcknowledgeInterrupt(UINT32 interruptNumber);
VOID PcIoInitializeInterruptVectorTable(PISR_VTBL pIsrFunctions);

VOID PcIoGetInterruptDescriptionTableRegister(OUT PIDT_REGISTER* pIdtRegister);
VOID PcIoSetInterruptDescriptionTableRegister(PIDT_REGISTER pIdtRegister);

