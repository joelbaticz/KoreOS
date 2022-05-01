/*
  - Date: 2021-08-22
  - Author: Joel
  - Purpose: Kernel device definitions
*/

#ifndef __KDEVICES_H
#define __KDEVICES_H

typedef UINT32 DEVICE_TYPE;

#define DEVICE_BATTERY                  0x0001
#define DEVICE_BEEP                     0x0002
#define DEVICE_CD_ROM                   0x0003
#define DEVICE_CD_ROM_FILE_SYSTEM       0x0004
#define DEVICE_CONTROLLER               0x0005
#define DEVICE_DISK                     0x0006
#define DEVICE_DISK_FILE_SYSTEM         0x0007
#define DEVICE_DVD                      0x0008
#define DEVICE_FILE_SYSTEM              0x0009
#define DEVICE_INPORT_PORT              0x000A
#define DEVICE_KEYBOARD                 0x000B
#define DEVICE_MAILSLOT                 0x000C
#define DEVICE_MASS_STORAGE             0x000D
#define DEVICE_MIDI_IN                  0x000E
#define DEVICE_MIDI_OUT                 0x000F
#define DEVICE_MODEM                    0x0010
#define DEVICE_MOUSE                    0x0011
#define DEVICE_NAMED_PIPE               0x0012
#define DEVICE_NETWORK                  0x0013
#define DEVICE_NETWORK_FILE_SYSTEM      0x0014
#define DEVICE_NULL                     0x0015
#define DEVICE_PARALLEL_PORT            0x0016
#define DEVICE_PRINTER                  0x0017
#define DEVICE_SCANNER                  0x0018
#define DEVICE_SCREEN                   0x0019
#define DEVICE_SERIAL_MOUSE_PORT        0x001A
#define DEVICE_SERIAL_PORT              0x001B
#define DEVICE_SOUND                    0x001C
#define DEVICE_UNKNOWN                  0x001D
#define DEVICE_VIDEO                    0x001E
#define DEVICE_VIRTUAL_DISK             0x001F
#define DEVICE_WAVE_IN                  0x0020
#define DEVICE_WAVE_OUT                 0x0021

typedef UINT32 DEVICE_CLASS;

#define DEVICE_CLASS_UNKOWN             0x0001
#define DEVICE_CLASS_DISK               0x0002  // Like file system, volume, disk, dvd, cd-rom, mass-storage
#define DEVICE_CLASS_HUMAN_INTERFACE    0x0003  // Like mouse, keyboard, controller
#define DEVICE_CLASS_COMMUNICATION      0x0004  // Like serial, parallel port, network, modem, pipe, mailslot
#define DEVICE_CLASS_OUTPUT             0x0005  // Like display/screen, video, printer
#define DEVICE_CLASS_MISC               0x0006  // Everything else, like battery, sound
#define DEVICE_CLASS_CONTROLLER         0x0007

typedef UINT32 DEVICE_FLAGS;
#define DEVICE_NONE                     0x0000

typedef UINT32 DRIVER_FLAGS;

#define DRIVER_NONE                     0x0000
#define DRIVER_BOOT                     0x0001  // Designates that the driver is a boot driver
#define DRIVER_FALL_BACK                0x0002  // Designates that the driver is a fall-back driver, which needs to be replaced as soon as an appropriate driver is found
#define DRIVER_PNP                      0x0003  // Designates that the driver is a Plug&Play driver

#endif /* __KDEVICES_H */