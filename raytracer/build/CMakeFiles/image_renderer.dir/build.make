# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/build

# Include any dependencies generated for this target.
include CMakeFiles/image_renderer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/image_renderer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/image_renderer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/image_renderer.dir/flags.make

CMakeFiles/image_renderer.dir/codegen:
.PHONY : CMakeFiles/image_renderer.dir/codegen

CMakeFiles/image_renderer.dir/main.cpp.o: CMakeFiles/image_renderer.dir/flags.make
CMakeFiles/image_renderer.dir/main.cpp.o: /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/main.cpp
CMakeFiles/image_renderer.dir/main.cpp.o: CMakeFiles/image_renderer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/image_renderer.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/image_renderer.dir/main.cpp.o -MF CMakeFiles/image_renderer.dir/main.cpp.o.d -o CMakeFiles/image_renderer.dir/main.cpp.o -c /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/main.cpp

CMakeFiles/image_renderer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/image_renderer.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/main.cpp > CMakeFiles/image_renderer.dir/main.cpp.i

CMakeFiles/image_renderer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/image_renderer.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/main.cpp -o CMakeFiles/image_renderer.dir/main.cpp.s

# Object files for target image_renderer
image_renderer_OBJECTS = \
"CMakeFiles/image_renderer.dir/main.cpp.o"

# External object files for target image_renderer
image_renderer_EXTERNAL_OBJECTS =

image_renderer: CMakeFiles/image_renderer.dir/main.cpp.o
image_renderer: CMakeFiles/image_renderer.dir/build.make
image_renderer: CMakeFiles/image_renderer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable image_renderer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/image_renderer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/image_renderer.dir/build: image_renderer
.PHONY : CMakeFiles/image_renderer.dir/build

CMakeFiles/image_renderer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/image_renderer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/image_renderer.dir/clean

CMakeFiles/image_renderer.dir/depend:
	cd /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/build /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/build /Users/christianwittwer/UtahTech/SeniorProject/distributed-raytracing/raytracer/build/CMakeFiles/image_renderer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/image_renderer.dir/depend

