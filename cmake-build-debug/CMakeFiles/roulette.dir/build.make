# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/john/Documents/comp262serial

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/john/Documents/comp262serial/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/roulette.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/roulette.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/roulette.dir/flags.make

CMakeFiles/roulette.dir/roulette.c.o: CMakeFiles/roulette.dir/flags.make
CMakeFiles/roulette.dir/roulette.c.o: ../roulette.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/john/Documents/comp262serial/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/roulette.dir/roulette.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/roulette.dir/roulette.c.o   -c /home/john/Documents/comp262serial/roulette.c

CMakeFiles/roulette.dir/roulette.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/roulette.dir/roulette.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/john/Documents/comp262serial/roulette.c > CMakeFiles/roulette.dir/roulette.c.i

CMakeFiles/roulette.dir/roulette.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/roulette.dir/roulette.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/john/Documents/comp262serial/roulette.c -o CMakeFiles/roulette.dir/roulette.c.s

# Object files for target roulette
roulette_OBJECTS = \
"CMakeFiles/roulette.dir/roulette.c.o"

# External object files for target roulette
roulette_EXTERNAL_OBJECTS =

roulette: CMakeFiles/roulette.dir/roulette.c.o
roulette: CMakeFiles/roulette.dir/build.make
roulette: CMakeFiles/roulette.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/john/Documents/comp262serial/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable roulette"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/roulette.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/roulette.dir/build: roulette

.PHONY : CMakeFiles/roulette.dir/build

CMakeFiles/roulette.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/roulette.dir/cmake_clean.cmake
.PHONY : CMakeFiles/roulette.dir/clean

CMakeFiles/roulette.dir/depend:
	cd /home/john/Documents/comp262serial/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/john/Documents/comp262serial /home/john/Documents/comp262serial /home/john/Documents/comp262serial/cmake-build-debug /home/john/Documents/comp262serial/cmake-build-debug /home/john/Documents/comp262serial/cmake-build-debug/CMakeFiles/roulette.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/roulette.dir/depend

