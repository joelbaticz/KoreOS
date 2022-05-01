/*
  - Date: 2021-11-11
  - Author: Joel
  - Purpose: PS/2 mouse driver
*/

#ifndef __MOUSE_H
#define __MOUSE_H

BOOL mouseServiceInstalled;

VOID MouseDriverInit();
VOID MouseHandler();

#endif // __MOUSE_H