# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\!PROGRAMMING\KOREOS\src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\!PROGRAMMING\KOREOS\build

# Include any dependencies generated for this target.
include CMakeFiles/entry.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/entry.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/entry.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/entry.dir/flags.make

CMakeFiles/entry.dir/mach/x86-32-pc/entry/entry.S.obj: CMakeFiles/entry.dir/flags.make
CMakeFiles/entry.dir/mach/x86-32-pc/entry/entry.S.obj: CMakeFiles/entry.dir/includes_ASM.rsp
CMakeFiles/entry.dir/mach/x86-32-pc/entry/entry.S.obj: C:/!PROGRAMMING/KOREOS/src/mach/x86-32-pc/entry/entry.S
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\!PROGRAMMING\KOREOS\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building ASM object CMakeFiles/entry.dir/mach/x86-32-pc/entry/entry.S.obj"
	C:\MinGW\bin\gcc.exe $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -o CMakeFiles\entry.dir\mach\x86-32-pc\entry\entry.S.obj -c C:\!PROGRAMMING\KOREOS\src\mach\x86-32-pc\entry\entry.S

CMakeFiles/entry.dir/mach/x86-32-pc/entry/entry.S.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing ASM source to CMakeFiles/entry.dir/mach/x86-32-pc/entry/entry.S.i"
	C:\MinGW\bin\gcc.exe $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -E C:\!PROGRAMMING\KOREOS\src\mach\x86-32-pc\entry\entry.S > CMakeFiles\entry.dir\mach\x86-32-pc\entry\entry.S.i

CMakeFiles/entry.dir/mach/x86-32-pc/entry/entry.S.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling ASM source to assembly CMakeFiles/entry.dir/mach/x86-32-pc/entry/entry.S.s"
	C:\MinGW\bin\gcc.exe $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -S C:\!PROGRAMMING\KOREOS\src\mach\x86-32-pc\entry\entry.S -o CMakeFiles\entry.dir\mach\x86-32-pc\entry\entry.S.s

# Object files for target entry
entry_OBJECTS = \
"CMakeFiles/entry.dir/mach/x86-32-pc/entry/entry.S.obj"

# External object files for target entry
entry_EXTERNAL_OBJECTS =

entry.exe: CMakeFiles/entry.dir/mach/x86-32-pc/entry/entry.S.obj
entry.exe: CMakeFiles/entry.dir/build.make
entry.exe: CMakeFiles/entry.dir/objects1.rsp
entry.exe: CMakeFiles/entry.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\!PROGRAMMING\KOREOS\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking ASM executable entry.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\entry.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/entry.dir/build: entry.exe
.PHONY : CMakeFiles/entry.dir/build

CMakeFiles/entry.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\entry.dir\cmake_clean.cmake
.PHONY : CMakeFiles/entry.dir/clean

CMakeFiles/entry.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\!PROGRAMMING\KOREOS\src C:\!PROGRAMMING\KOREOS\src C:\!PROGRAMMING\KOREOS\build C:\!PROGRAMMING\KOREOS\build C:\!PROGRAMMING\KOREOS\build\CMakeFiles\entry.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/entry.dir/depend
