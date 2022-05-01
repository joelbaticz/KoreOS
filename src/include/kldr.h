/*
  - Date: 2018-01-01
  - Author: Joel
  - Purpose: General definitions for Loader
*/

#ifndef __KLDR_H
#define __KLDR_H

#define IN const
#define OUT

#include <ktypes.h>
#include <crt/string.h>
#include <crt/stdio.h>
#include <crt/stdlib.h>
#include <statuscode.h>
#include <utility.h>

#include <build.h>
#include <cmdline.h>
#include <debug.h>
#include <datetime.h>
#include <memman.h>  /* It needs to be here, because the header files below are using it */
#include <kdevices.h>
#include <kobjects.h>
#include <resman.h>
#include <ioman.h>
#include <machine.h>
#include <ioctl.h>
#include <giobjects.h>
#include <font.h>
#include <bitmap.h>
#include <icon.h>
#include <giman.h>
#include <systemlookandfeel.h>
#include <windowing.h>
#include <uidesktop.h>
#include <uiappwindow.h>
#include <uibutton.h>
#include <uicheckbox.h>
#include <uiradiobutton.h>
#include <uiscrollbar.h>
#include <uitextbox.h>
#include <messaging.h>

#ifdef KORE_MACHINE_TYPE_X86_32_PC
  #include <../mach/x86-32-pc/def.S>
  #include <../mach/x86-32-pc/include/pc.h>
  #include <../mach/x86-32-pc/include/pcbios.h>
  #include <../mach/x86-32-pc/include/pcio.h>
  #include <../mach/x86-32-pc/include/pcconsole.h>
  #include <../mach/x86-32-pc/include/pcutility.h>
  #include <../mach/x86-32-pc/include/pcmemory.h>
  //#include <pcsystem.h>
  //#include <pcdisk.h>
#endif

#ifdef KORE_MACHINE_TYPE_X86_32_XBOX
  #include <xbox.h>
  //#include <xboxio.h>
  //#include <xboxsystem.h>
  //#include <xboxdisk.h>
#endif

/* UI */
#include <ui.h>
#include <textui.h>
//#include <ui/uitext.h>
//#include <uivga.h>
//#include <uivesa.h>

/* Drivers */ 
#include <pit.h>
#include <pic.h>
#include <vga.h>
#include <keyboard.h>
#include <mouse.h>

/* File System */

/* GUI */
#include <bugcheck.h>
#include <gitest.h>
#include <welcomewindow.h>
#include <systempropertieswindow.h>
#include <aboutwindow.h>
#include <palettewindow.h>
#include <mystifywindow.h>
#include <launcherwindow.h>
#include <memorymapwindow.h>
#include <clockwindow.h>
#include <uitestwindow.h>

/* Loader */
VOID __cdecl KLoaderMain(IN PCHAR pCommandLineOptions);

#endif /* __KLDR_H */
