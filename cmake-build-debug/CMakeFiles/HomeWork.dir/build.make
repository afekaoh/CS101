# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /cygdrive/c/Users/afeka/.CLion2019.2/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/afeka/.CLion2019.2/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/HomeWork.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HomeWork.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HomeWork.dir/flags.make

CMakeFiles/HomeWork.dir/ass4.c.o: CMakeFiles/HomeWork.dir/flags.make
CMakeFiles/HomeWork.dir/ass4.c.o: ../ass4.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/HomeWork.dir/ass4.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/HomeWork.dir/ass4.c.o   -c "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork/ass4.c"

CMakeFiles/HomeWork.dir/ass4.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/HomeWork.dir/ass4.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork/ass4.c" > CMakeFiles/HomeWork.dir/ass4.c.i

CMakeFiles/HomeWork.dir/ass4.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/HomeWork.dir/ass4.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork/ass4.c" -o CMakeFiles/HomeWork.dir/ass4.c.s

# Object files for target HomeWork
HomeWork_OBJECTS = \
"CMakeFiles/HomeWork.dir/ass4.c.o"

# External object files for target HomeWork
HomeWork_EXTERNAL_OBJECTS =

HomeWork.exe: CMakeFiles/HomeWork.dir/ass4.c.o
HomeWork.exe: CMakeFiles/HomeWork.dir/build.make
HomeWork.exe: CMakeFiles/HomeWork.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable HomeWork.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HomeWork.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HomeWork.dir/build: HomeWork.exe

.PHONY : CMakeFiles/HomeWork.dir/build

CMakeFiles/HomeWork.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HomeWork.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HomeWork.dir/clean

CMakeFiles/HomeWork.dir/depend:
	cd "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork" "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork" "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork/cmake-build-debug" "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork/cmake-build-debug" "/cygdrive/c/Users/afeka/OneDrive - Bar-Ilan University/CS101/HomeWork/cmake-build-debug/CMakeFiles/HomeWork.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/HomeWork.dir/depend

