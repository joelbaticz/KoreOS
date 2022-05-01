cmake_minimum_required(VERSION 3.10)
project(KORE C ASM)


#----------------------------------
# COMPILE OPTIONS
#----------------------------------
add_compile_options(-mgeneral-regs-only) # For interrupt handlers GCC can't save FPU and SSE registers
#set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

#----------------------------------
# BUILD TYPE
#----------------------------------

set(KORE_BUILD_TYPE_DEBUG "KORE_BUILD_TYPE_DEBUG")
set(KORE_BUILD_TYPE_RELEASE "KORE_BUILD_TYPE_RELEASE")

set(KORE_BUILD_TYPE ${KORE_BUILD_TYPE_DEBUG} CACHE STRING "Select Build Type")
add_definitions(-D${KORE_BUILD_TYPE})

message("BUILD_TYPE: ${KORE_BUILD_TYPE}")

#----------------------------------
# DEBUG OPTIONS
#----------------------------------

set(KORE_BUILD_DEBUG_LEVEL_ALL "KORE_BUILD_DEBUG_LEVEL_ALL")
set(KORE_BUILD_DEBUG_LEVEL_TRACE "KORE_BUILD_DEBUG_LEVEL_WARNING")
set(KORE_BUILD_DEBUG_LEVEL_ERROR "KORE_BUILD_DEBUG_LEVEL_ERROR")

set(KORE_BUILD_DEBUG_LEVEL ${KORE_BUILD_DEBUG_LEVEL_ALL} CACHE STRING "Select Debug Level")
add_definitions(-D${KORE_BUILD_DEBUG_LEVEL})

message("DEBUG_LEVEL: ${KORE_BUILD_DEBUG_LEVEL}")

#----------------------------------
# MACHINE TYPES
#----------------------------------

set(KORE_MACHINE_TYPE_X86_32_PC "KORE_MACHINE_TYPE_X86_32_PC")
set(KORE_MACHINE_TYPE_X86_64_PC "KORE_MACHINE_TYPE_X86_64_PC")
set(KORE_MACHINE_TYPE_X86_32_XBOX "KORE_MACHINE_TYPE_X86_32_XBOX")

set(KORE_MACHINE_TYPE ${KORE_MACHINE_TYPE_X86_32_PC} CACHE STRING "Select Machine Type")
add_definitions(-D${KORE_MACHINE_TYPE})

message("MACHINE_TYPE: ${KORE_MACHINE_TYPE}")

#----------------------------------
#COMMON OPTIONS
#----------------------------------

set(CMAKE_BUILD_DIRECTORY "../build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../build")
set(CMAKE_OUTPUT_FILE_FINAL "kore.sys")
#set(CMAKE_ASM_COMPILER "c:/MinGW/bin/gcc.exe")
#set(CMAKE_C_COMPILER "c:/MinGW/bin/gcc.exe")
enable_language(ASM)

#----------------------------------
# LOADER ENTRY
#----------------------------------

if(KORE_MACHINE_TYPE STREQUAL ${KORE_MACHINE_TYPE_X86_32_PC})
    set(CMAKE_ENTRY_INPUT_FILE "mach/x86-32-pc/entry/entry.S")
endif()

set(CMAKE_ENTRY_OUTPUT_FILE "entry")
set(CMAKE_ENTRY_OUTPUT_FILE_EXE "${CMAKE_ENTRY_OUTPUT_FILE}.exe")
set(CMAKE_ENTRY_OUTPUT_FILE_SYS "entry.sy_")

add_executable(${CMAKE_ENTRY_OUTPUT_FILE} ${CMAKE_ENTRY_INPUT_FILE})
set_target_properties(${CMAKE_ENTRY_OUTPUT_FILE} PROPERTIES LINK_FLAGS "-std=c99 -nostdlib -s -T../src/mach/x86-32-pc/entry/link.ld")
add_custom_target(TARGET1 ALL DEPENDS ${CMAKE_ENTRY_OUTPUT_FILE}) 

set(CMAKE_OBJCOPY_COMMAND "objcopy -O binary -j .text ${CMAKE_ENTRY_OUTPUT_FILE_EXE} ${CMAKE_ENTRY_OUTPUT_FILE_SYS}")
separate_arguments(CMAKE_OBJCOPY_COMMAND_SEPARATED WINDOWS_COMMAND ${CMAKE_OBJCOPY_COMMAND})

add_custom_command(
	WORKING_DIRECTORY	${CMAKE_BUILD_DIRECTORY}
	COMMAND			${CMAKE_OBJCOPY_COMMAND_SEPARATED}
	DEPENDS			${CMAKE_ENTRY_OUTPUT_FILE}
	OUTPUT			${CMAKE_ENTRY_OUTPUT_FILE_SYS}
	#TARGET			TARGET2
	COMMENT			"EXECUTING: [${CMAKE_OBJCOPY_COMMAND}]")

add_custom_target(TARGET2 ALL DEPENDS ${CMAKE_ENTRY_OUTPUT_FILE_SYS}) 

#----------------------------------
# LOADER MAIN
#----------------------------------

set(CMAKE_MAIN_INPUT_FILE "loader.c")
set(CMAKE_MAIN_OUTPUT_FILE "loader")
set(CMAKE_MAIN_OUTPUT_FILE_EXE "loader.exe")

list(APPEND CMAKE_LOADER_SOURCE
	"loader.c"
	
	"${CMAKE_SOURCE_DIR}/lib/crt/string.c"
	"${CMAKE_SOURCE_DIR}/lib/crt/stdio.c"
	"${CMAKE_SOURCE_DIR}/lib/crt/stdlib.c"
	"${CMAKE_SOURCE_DIR}/lib/utility.c"
	
	"${CMAKE_SOURCE_DIR}/lib/build.c"
	"${CMAKE_SOURCE_DIR}/lib/cmdline.c"
	"${CMAKE_SOURCE_DIR}/lib/debug.c"

	"${CMAKE_SOURCE_DIR}/lib/machine.c"
	"${CMAKE_SOURCE_DIR}/mach/x86-32-pc/lib/pc.c"	
	"${CMAKE_SOURCE_DIR}/mach/x86-32-pc/lib/pcbios.c"
	"${CMAKE_SOURCE_DIR}/mach/x86-32-pc/lib/pcio.c"
	"${CMAKE_SOURCE_DIR}/mach/x86-32-pc/lib/pcconsole.c"
	"${CMAKE_SOURCE_DIR}/mach/x86-32-pc/lib/pcutility.c"
	"${CMAKE_SOURCE_DIR}/mach/x86-32-pc/lib/pcmemory.c"
	
	#"${CMAKE_SOURCE_DIR}/mach/x86-32-xbox/lib/xbox.c"
	#"${CMAKE_SOURCE_DIR}/mach/x86-32-xbox/lib/xboxbios.c"
	#"${CMAKE_SOURCE_DIR}/mach/x86-32-xbox/lib/xboxio.c"
	#"${CMAKE_SOURCE_DIR}/mach/x86-32-xbox/lib/xboxconsole.c"
	#"${CMAKE_SOURCE_DIR}/mach/x86-32-xbox/lib/xboxutility.c"
	
	"${CMAKE_SOURCE_DIR}/lib/ui.c"
	"${CMAKE_SOURCE_DIR}/lib/textui.c"
	
	"${CMAKE_SOURCE_DIR}/lib/datetime.c"
	"${CMAKE_SOURCE_DIR}/lib/memman.c"
	"${CMAKE_SOURCE_DIR}/lib/resman.c"
	"${CMAKE_SOURCE_DIR}/lib/ioman.c"
	"${CMAKE_SOURCE_DIR}/lib/font.c"
	"${CMAKE_SOURCE_DIR}/lib/bitmap.c"
	"${CMAKE_SOURCE_DIR}/lib/icon.c"
	"${CMAKE_SOURCE_DIR}/lib/giman.c"
	"${CMAKE_SOURCE_DIR}/lib/systemlookandfeel.c"
	"${CMAKE_SOURCE_DIR}/lib/windowing.c"
	"${CMAKE_SOURCE_DIR}/lib/uidesktop.c"
	"${CMAKE_SOURCE_DIR}/lib/uiappwindow.c"
	"${CMAKE_SOURCE_DIR}/lib/uibutton.c"
	"${CMAKE_SOURCE_DIR}/lib/uicheckbox.c"
	"${CMAKE_SOURCE_DIR}/lib/uiradiobutton.c"
	"${CMAKE_SOURCE_DIR}/lib/uiscrollbar.c"
	"${CMAKE_SOURCE_DIR}/lib/uitextbox.c"
	"${CMAKE_SOURCE_DIR}/lib/messaging.c"
	
	"${CMAKE_SOURCE_DIR}/lib/pit.c"
	"${CMAKE_SOURCE_DIR}/lib/pic.c"
	"${CMAKE_SOURCE_DIR}/lib/vga.c"
	"${CMAKE_SOURCE_DIR}/lib/keyboard.c"
	"${CMAKE_SOURCE_DIR}/lib/mouse.c"

	"${CMAKE_SOURCE_DIR}/lib/bugcheck.c"
	"${CMAKE_SOURCE_DIR}/lib/gitest.c"
	"${CMAKE_SOURCE_DIR}/lib/welcomewindow.c"
	"${CMAKE_SOURCE_DIR}/lib/systempropertieswindow.c"
	"${CMAKE_SOURCE_DIR}/lib/aboutwindow.c"
	"${CMAKE_SOURCE_DIR}/lib/palettewindow.c"
	"${CMAKE_SOURCE_DIR}/lib/mystifywindow.c"
	"${CMAKE_SOURCE_DIR}/lib/launcherwindow.c"
	"${CMAKE_SOURCE_DIR}/lib/memorymapwindow.c"
	"${CMAKE_SOURCE_DIR}/lib/clockwindow.c"
	"${CMAKE_SOURCE_DIR}/lib/uitestwindow.c"
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${CMAKE_MAIN_OUTPUT_FILE} ${CMAKE_LOADER_SOURCE})
set_target_properties(${CMAKE_MAIN_OUTPUT_FILE} PROPERTIES LINK_FLAGS "-masm=intel -ffreestanding --entry=_KLoaderMain -nostdlib -T../src/link.ld")
add_custom_target(TARGET3 ALL DEPENDS ${CMAKE_MAIN_OUTPUT_FILE}) 

#----------------------------------
# LOADER MERGE PART (ENTRY + MAIN)
#----------------------------------

set(COPY_COMMAND_FULL "copy /b /y ${CMAKE_ENTRY_OUTPUT_FILE_SYS} + ${CMAKE_MAIN_OUTPUT_FILE_EXE} ${CMAKE_OUTPUT_FILE_FINAL}")
separate_arguments(COPY_COMMAND_FULL_SEPARATED WINDOWS_COMMAND ${COPY_COMMAND_FULL})

add_custom_command(
	WORKING_DIRECTORY	${CMAKE_BUILD_DIRECTORY}
	COMMAND			${COPY_COMMAND_FULL_SEPARATED} 
	DEPENDS			TARGET2 ${CMAKE_MAIN_OUTPUT_FILE_EXE}
	OUTPUT			${CMAKE_OUTPUT_FILE_FINAL}
	#TARGET			TARGET4
	COMMENT			"EXECUTING: [${COPY_COMMAND_FULL}]"
	)

add_custom_target(TARGET4 ALL DEPENDS ${CMAKE_OUTPUT_FILE_FINAL})
#add_custom_target(TARGET4 ALL DEPENDS TARGET3)

#TARGET1 = entry.exe
#TARGET2 = entry.sy_
#TARGET3 = loader.exe
#TARGET4 = kldr.sys

#add_dependencies(TARGET2 TARGET1)
#add_dependencies(TARGET4 TARGET3)
#add_dependencies(TARGET5 TARGET2)
#add_dependencies(TARGET5 TARGET4)

