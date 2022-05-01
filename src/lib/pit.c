/*
  - Date: 2022-01-16
  - Author: Joel
  - Purpose: Prommable Interval Timer driver
*/

#include <kldr.h>

#define PIT_REG_COUNTER_ZERO     0x40
#define PIT_REG_COUNTER_ONE      0x41
#define PIT_REG_COUNTER_TWO      0x42
#define PIT_REG_COMMAND          0x43

#define PIT_MODE_MASK            0x0E
#define PIT_RL_MASK              0x30
#define PIT_COUTNER_MASK         0xC0

#define PIT_MODE_SQUAREWAVE      0x06
#define PIT_RL_DATA              0x30

#define PIT_COUNTER_ZERO         0x00
#define PIT_COUNTER_ONE          0x40
#define PIT_COUNTER_TWO          0x80

static PDRIVER_OBJECT pPitDriver;
static PDEVICE_OBJECT pPitDevice;

VOID ConfigurePIT(UINT16 frequency);

VOID PitDriverInit()
{
    pPitDriver = NULL;
    pPitDevice = NULL;

    // Create Driver object

    KSTATUS result = DriverCreate(
                        (PCHAR) &"",                                            // Fully qualified driver object name
                        (PCHAR) &"Programmable Interval Timer Driver",          // Driver name/description
                        DRIVER_BOOT,                                            // Flags
                        NULL,                                                   // Driver Init function
                        NULL,                                                   // Driver Unload function
                        NULL,                                                   // Driver Dispatch funcion
                        OUT &pPitDriver);

    if (result != STATUS_SUCCESS | pPitDriver == NULL)
    {
        ERROR("Could not initialize PIT driver (DriverCreate failed)!");
        HALT;
    }

    result = DeviceCreate(
                (PCHAR) &"\\PIT",                                               // Fully qualified device object name
                (PCHAR) &"Programmable Interval Timer",                         // Device name/description
                pPitDriver,                                                     // Pointer to driver object
                DEVICE_CONTROLLER,                                              // Device type
                DEVICE_CLASS_CONTROLLER,                                        // Device class
                OUT &pPitDevice);

    if (result != STATUS_SUCCESS | pPitDevice == NULL)
    {
        DriverDelete(pPitDriver);
        ERROR("Could not initialize PIC driver (DeviceCreate failed)!");
        HALT;
    }

    ConfigurePIT(200);

    UiDrawText("PIT Driver...................DONE");
}

VOID ConfigurePIT(UINT16 frequency)
{
    // Interrupts should be disabled at this point

    UINT16 divisor = 1193181 / frequency;

    // Send Operational Command Words
    UINT8 command = 0;

    command = (command & ~PIT_MODE_MASK) | PIT_MODE_SQUAREWAVE;
    command = (command & ~PIT_RL_MASK) | PIT_RL_DATA;
    command = (command & ~PIT_COUTNER_MASK) | PIT_COUNTER_ZERO;
    outportb(PIT_REG_COMMAND, command);

    // Set frequency rate
    outportb(PIT_REG_COUNTER_ZERO, divisor & 0xFF);
    outportb(PIT_REG_COUNTER_ZERO, (divisor >> 8) & 0xFF);
}