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

# Utility rule file for TARGET4.

# Include any custom commands dependencies for this target.
include CMakeFiles/TARGET4.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/TARGET4.dir/progress.make

CMakeFiles/TARGET4: kore.sys

kore.sys: loader.exe
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\!PROGRAMMING\KOREOS\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "EXECUTING: [copy /b /y entry.sy_ + loader.exe kore.sys]"
	copy /b /y entry.sy_ + loader.exe kore.sys

TARGET4: CMakeFiles/TARGET4
TARGET4: kore.sys
TARGET4: CMakeFiles/TARGET4.dir/build.make
.PHONY : TARGET4

# Rule to build all files generated by this target.
CMakeFiles/TARGET4.dir/build: TARGET4
.PHONY : CMakeFiles/TARGET4.dir/build

CMakeFiles/TARGET4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\TARGET4.dir\cmake_clean.cmake
.PHONY : CMakeFiles/TARGET4.dir/clean

CMakeFiles/TARGET4.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\!PROGRAMMING\KOREOS\src C:\!PROGRAMMING\KOREOS\src C:\!PROGRAMMING\KOREOS\build C:\!PROGRAMMING\KOREOS\build C:\!PROGRAMMING\KOREOS\build\CMakeFiles\TARGET4.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TARGET4.dir/depend

