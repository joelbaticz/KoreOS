/*
  - Date: 2021-09-06
  - Author: Joel
  - Purpose: IOCTL control codes
*/

#ifndef __IOCTL_H
#define __IOCTL_H

#define IOCTL_CREATE                            0x0001
#define IOCTL_OPEN                              0x0002
#define IOCTL_CLOSE                             0x0003
#define IOCTL_READ                              0x0004
#define IOCTL_WRITE                             0x0005
#define IOCTL_GET_INFORMATION                   0x0006
#define IOCTL_SET_INFORMATION                   0x0007

#define IOCTL_VIDEO_INITIALIZE                  0x1001
#define IOCTL_VIDEO_RESET_DEVICE                0x1002
#define IOCTL_VIDEO_GET_NUMBER_OF_MODES         0x1003
#define IOCTL_VIDEO_GET_MODE_INFORMATION        0x1004
#define IOCTL_VIDEO_SET_CURRENT_MODE            0x1005
#define IOCTL_VIDEO_GET_FRAMEBUFFER_ADDRESS     0x1006
#define IOCTL_VIDEO_SET_PALETTE                 0x1007
#define IOCTL_VIDEO_GET_DEVICE_FUNCTIONS        0x1008

// Unused
#define IOCTL_VIDEO_GET_MEMORY_INFORMATION      0x1009
#define IOCTL_VIDEO_ENABLE_POINTER              0x100A
#define IOCTL_VIDEO_DISABLE_POINTER             0x100B
#define IOCTL_VIDEO_GET_POINTER_POSITION        0x100C
#define IOCTL_VIDEO_SET_POINTER_POSITION        0x100D
#define IOCTL_VIDEO_ENABLE_CURSOR               0x100E
#define IOCTL_VIDEO_DISABLE_CURSOR              0x100F
#define IOCTL_VIDEO_GET_BANK_SELECT_CODE        0x1010
#define IOCTL_VIDEO_SET_BANK_POSITION           0x1011

#define IOCTL_FILE_SYSTEM                       0x2001

#endif /* __IOCTL_H */