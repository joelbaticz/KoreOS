/*
  - Date: 2021-08-18
  - Author: Joel
  - Purpose: Prommable Interrupt Contoller driver
*/

#include <kldr.h>

#define PIC_PIC1_PORT       		    0x20
#define PIC_PIC2_PORT		            0xA0
#define PIC_PIC1_COMMAND_REGISTER	    PIC_PIC1_PORT
#define PIC_PIC1_DATA_REGISTER  	    (PIC_PIC1_PORT+1)
#define PIC_PIC2_COMMAND_REGISTER	    PIC_PIC2_PORT
#define PIC_PIC2_DATA_REGISTER  	    (PIC_PIC2_PORT+1)

// Interrupt Control Words (ICWs)
#define PIC_ICW1_ICW4	                0x01		/* ICW4 (not) needed */
#define PIC_ICW1_SINGLE	                0x02		/* Single (cascade) mode */
#define PIC_ICW1_INTERVAL4	            0x04		/* Call address interval 4 (8) */
#define PIC_ICW1_LEVEL	                0x08		/* Level triggered (edge) mode */
#define PIC_ICW1_INIT	                0x10		/* Initialization - required! */

#define PIC_ICW2_IRQ_VECTOR_OFFSET1     0x20        /* IRQ0-7 will be remapped to interrupts 0x20-0x27 */
#define PIC_ICW2_IRQ_VECTOR_OFFSET2     0x28        /* IRQ8-15 will be remapped to interrupts 0x28-0x2F */

#define PIC_ICW4_8086	                0x01		/* 8086/88 (MCS-80/85) mode */
#define PIC_ICW4_AUTO	                0x02		/* Auto (normal) EOI */
#define PIC_ICW4_BUF_SLAVE	            0x08		/* Buffered mode/slave */
#define PIC_ICW4_BUF_MASTER	            0x0C		/* Buffered mode/master */
#define PIC_ICW4_SFNM	                0x10		/* Special fully nested (not) */

VOID PicDisableIRQ(UINT8 IrqLine);
VOID PicEnableIRQ(UINT8 IrqLine);

static PDRIVER_OBJECT pPicDriver;
static PDEVICE_OBJECT pPicDevice;

static UINT8 PicBiosPic1Mask;
static UINT8 PicBiosPic2Mask;

VOID PicDriverInit()
{
    pPicDriver = NULL;
    pPicDevice = NULL;

    // Create Driver object (even if it's fake I guess)

    KSTATUS result = DriverCreate(
                        (PCHAR) &"",                                            // Fully qualified driver object name
                        (PCHAR) &"Programmable Interrupt Controller Driver",    // Driver name/description
                        DRIVER_BOOT,                                            // Flags
                        NULL,                                                   // Driver Init function
                        NULL,                                                   // Driver Unload function
                        NULL,                                                   // Driver Dispatch funcion
                        OUT &pPicDriver);

    if (result != STATUS_SUCCESS | pPicDriver == NULL)
    {
        ERROR("Could not initialize PIC driver (DriverCreate failed)!");
        HALT;
    }

    result = DeviceCreate(
                (PCHAR) &"\\PIC",                                               // Fully qualified device object name
                (PCHAR) &"Programmable Interrupt Controller",                   // Device name/description
                pPicDriver,                                                     // Pointer to driver object
                DEVICE_CONTROLLER,                                              // Device type
                DEVICE_CLASS_CONTROLLER,                                        // Device class
                OUT &pPicDevice);

    if (result != STATUS_SUCCESS | pPicDevice == NULL)
    {
        DriverDelete(pPicDriver);
        ERROR("Could not initialize PIC driver (DeviceCreate failed)!");
        HALT;
    }

    

    PicMapSystemIRQs();
    //PicSaveBiosIRQs();
    PicDisableIRQs();
    PicEnableSpecificIRQs();

    //MachineEnableInterrupts();

    UiDrawText("PIC Driver...................DONE");
    //HALT;
}

VOID PicMapSystemIRQs()
{
    UINT8 pic1Mask;
    UINT8 pic2Mask;

    // Save interrupt masks
    pic1Mask = MachineIoReadPortUint8(PIC_PIC1_DATA_REGISTER);
    pic2Mask = MachineIoReadPortUint8(PIC_PIC2_DATA_REGISTER);

    // // Start PIC initialization sequence
    MachineIoWritePortUint8(PIC_PIC1_COMMAND_REGISTER, PIC_ICW1_INIT | PIC_ICW1_ICW4);  // PIC1 initialize command with providing control words ICW1-4
    MachineIoDelay();
    MachineIoWritePortUint8(PIC_PIC2_COMMAND_REGISTER, PIC_ICW1_INIT | PIC_ICW1_ICW4);  // PIC2 initialize command with providing control words ICW1-4
    MachineIoDelay();

    // Set new interrupt vectors
    MachineIoWritePortUint8(PIC_PIC1_DATA_REGISTER, PIC_ICW2_IRQ_VECTOR_OFFSET1);   // PIC1 ICW2: interrupt vector offset
	MachineIoDelay();
	MachineIoWritePortUint8(PIC_PIC2_DATA_REGISTER, PIC_ICW2_IRQ_VECTOR_OFFSET2);   // PIC2 ICW2: interrupt vector offset
	MachineIoDelay();

    // Set PIC2 cascade to PIC1
    MachineIoWritePortUint8(PIC_PIC1_DATA_REGISTER, 4);                         // PIC1 ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	MachineIoDelay();
	MachineIoWritePortUint8(PIC_PIC2_DATA_REGISTER, 2);                         // PIC2 ICW3: tell Slave PIC its cascade identity (0000 0010)
	MachineIoDelay();    

    // Set 8086 mode
    MachineIoWritePortUint8(PIC_PIC1_DATA_REGISTER, PIC_ICW4_8086);                                    // PIC1 ICW4: 8086 mode
	MachineIoDelay();
	MachineIoWritePortUint8(PIC_PIC2_DATA_REGISTER, PIC_ICW4_8086);                                    // PIC2 ICW4: 8086 mode
	MachineIoDelay();

    // ICW1
    // MachineIoWritePortUint8(0x20, 0x11);
    // MachineIoWritePortUint8(0xA0, 0x11);

    // // ICW2
    // MachineIoWritePortUint8(0x21, 0x20);
    // MachineIoWritePortUint8(0xA1, 0x28);

    // // ICW3
    // MachineIoWritePortUint8(0x21, 0x04);
    // MachineIoWritePortUint8(0xA1, 0x02);

    // // ICW4
    // MachineIoWritePortUint8(0x21, 0x01);
    // MachineIoWritePortUint8(0xA1, 0x01);

    // // OCW1
    // MachineIoWritePortUint8(0x21, 0x0);     // Re-enable all interrupts
    // MachineIoWritePortUint8(0xA1, 0x0);     // Re-enable all interrupts

    // Restore interrupt masks
    MachineIoWritePortUint8(PIC_PIC1_DATA_REGISTER, pic1Mask);
	MachineIoWritePortUint8(PIC_PIC2_DATA_REGISTER, pic2Mask);
}

VOID PicMapBiosIRQs()
{
    UINT8 pic1Mask;
    UINT8 pic2Mask;

    // Save interrupt masks
    pic1Mask = MachineIoReadPortUint8(PIC_PIC1_DATA_REGISTER);
    pic2Mask = MachineIoReadPortUint8(PIC_PIC2_DATA_REGISTER);

    // // Start PIC initialization sequence
    MachineIoWritePortUint8(PIC_PIC1_COMMAND_REGISTER, PIC_ICW1_INIT | PIC_ICW1_ICW4);  // PIC1 initialize command with providing control words ICW1-4
    MachineIoDelay();
    MachineIoWritePortUint8(PIC_PIC2_COMMAND_REGISTER, PIC_ICW1_INIT | PIC_ICW1_ICW4);  // PIC2 initialize command with providing control words ICW1-4
    MachineIoDelay();

    // Set new interrupt vectors
    MachineIoWritePortUint8(PIC_PIC1_DATA_REGISTER, 0);   // PIC1 ICW2: interrupt vector offset
	MachineIoDelay();
	MachineIoWritePortUint8(PIC_PIC2_DATA_REGISTER, 8);   // PIC2 ICW2: interrupt vector offset
	MachineIoDelay();

    // Set PIC2 cascade to PIC1
    MachineIoWritePortUint8(PIC_PIC1_DATA_REGISTER, 4);                         // PIC1 ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	MachineIoDelay();
	MachineIoWritePortUint8(PIC_PIC2_DATA_REGISTER, 2);                         // PIC2 ICW3: tell Slave PIC its cascade identity (0000 0010)
	MachineIoDelay();    

    // Set 8086 mode
    MachineIoWritePortUint8(PIC_PIC1_DATA_REGISTER, PIC_ICW4_8086);                                    // PIC1 ICW4: 8086 mode
	MachineIoDelay();
	MachineIoWritePortUint8(PIC_PIC2_DATA_REGISTER, PIC_ICW4_8086);                                    // PIC2 ICW4: 8086 mode
	MachineIoDelay();

    // ICW1
    // MachineIoWritePortUint8(0x20, 0x11);
    // MachineIoWritePortUint8(0xA0, 0x11);

    // // ICW2
    // MachineIoWritePortUint8(0x21, 0x20);
    // MachineIoWritePortUint8(0xA1, 0x28);

    // // ICW3
    // MachineIoWritePortUint8(0x21, 0x04);
    // MachineIoWritePortUint8(0xA1, 0x02);

    // // ICW4
    // MachineIoWritePortUint8(0x21, 0x01);
    // MachineIoWritePortUint8(0xA1, 0x01);

    // // OCW1
    // MachineIoWritePortUint8(0x21, 0x0);     // Re-enable all interrupts
    // MachineIoWritePortUint8(0xA1, 0x0);     // Re-enable all interrupts

    // Restore interrupt masks
    MachineIoWritePortUint8(PIC_PIC1_DATA_REGISTER, pic1Mask);
	MachineIoWritePortUint8(PIC_PIC2_DATA_REGISTER, pic2Mask);
}

VOID PicSaveBiosIRQs()
{
    PicBiosPic1Mask = MachineIoReadPortUint8(PIC_PIC1_DATA_REGISTER);
    PicBiosPic2Mask = MachineIoReadPortUint8(PIC_PIC2_DATA_REGISTER);
}

VOID PicRestoreBiosIRQs()
{
    MachineIoWritePortUint8(PIC_PIC1_DATA_REGISTER, PicBiosPic1Mask);
	MachineIoWritePortUint8(PIC_PIC2_DATA_REGISTER, PicBiosPic2Mask);
}

VOID PicDisableIRQs()
{
    for(UINT8 i = 0; i < 16; i++)
    {
        PicDisableIRQ(i);
    }
}

VOID PicEnableSpecificIRQs()
{
    PicEnableIRQ(0);    // System Timer
    PicEnableIRQ(1);    // Keyboard
    PicEnableIRQ(2);    // Cascade from PIC2
    //PicEnableIRQ(3);    // Serial Port 2 and 4
    //PicEnableIRQ(4);    // Serial Port 1 and 3
    //PicEnableIRQ(5);    // Parallel Port 3 or Sound Card
    //PicEnableIRQ(6);    // Floppy Disk Controller
    //PicEnableIRQ(7);    // Parallel Port 1 and 2

    //PicEnableIRQ(8);  // Real-Time Clock
    //PicEnableIRQ(9);  // ACPI
    //PicEnableIRQ(10); // Available
    //PicEnableIRQ(11); // Available
    PicEnableIRQ(12);   // Mouse
    //PicEnableIRQ(13); // Co-processor or FPU
    //PicEnableIRQ(14); // Primary ATA
    //PicEnableIRQ(15); // Secondary ATA
}

VOID PicDisableIRQ(UINT8 IrqLine)
{
    UINT16 port;
    UINT8 value;
 
    if(IrqLine < 8)
    {
        port = PIC_PIC1_DATA_REGISTER;
    }
    else
    {
        port = PIC_PIC2_DATA_REGISTER;
        IrqLine -= 8;
    }

    value = MachineIoReadPortUint8(port);
    value |= (1 << IrqLine);

    MachineIoWritePortUint8(port, value);        
}
 
VOID PicEnableIRQ(UINT8 IrqLine)
{
    UINT16 port;
    UINT8 value;
 
    if(IrqLine < 8)
    {
        port = PIC_PIC1_DATA_REGISTER;
    }
    else
    {
        port = PIC_PIC2_DATA_REGISTER;
        IrqLine -= 8;
    }

    value = MachineIoReadPortUint8(port);
    value &= ~(1 << IrqLine);

    MachineIoWritePortUint8(port, value);        
}
