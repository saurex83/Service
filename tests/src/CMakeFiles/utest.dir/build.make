# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/saurex83/Projects/Neocore/Service

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/saurex83/Projects/Neocore/Service

# Include any dependencies generated for this target.
include tests/src/CMakeFiles/utest.dir/depend.make

# Include the progress variables for this target.
include tests/src/CMakeFiles/utest.dir/progress.make

# Include the compile flags for this target's objects.
include tests/src/CMakeFiles/utest.dir/flags.make

tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o: tests/src/CMakeFiles/utest.dir/flags.make
tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o: tests/src/SerialCom_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saurex83/Projects/Neocore/Service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utest.dir/SerialCom_test.cpp.o -c /home/saurex83/Projects/Neocore/Service/tests/src/SerialCom_test.cpp

tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utest.dir/SerialCom_test.cpp.i"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saurex83/Projects/Neocore/Service/tests/src/SerialCom_test.cpp > CMakeFiles/utest.dir/SerialCom_test.cpp.i

tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utest.dir/SerialCom_test.cpp.s"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saurex83/Projects/Neocore/Service/tests/src/SerialCom_test.cpp -o CMakeFiles/utest.dir/SerialCom_test.cpp.s

tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o.requires:

.PHONY : tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o.requires

tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o.provides: tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o.requires
	$(MAKE) -f tests/src/CMakeFiles/utest.dir/build.make tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o.provides.build
.PHONY : tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o.provides

tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o.provides.build: tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o


tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o: tests/src/CMakeFiles/utest.dir/flags.make
tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o: tests/src/ServiceConfig_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saurex83/Projects/Neocore/Service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utest.dir/ServiceConfig_test.cpp.o -c /home/saurex83/Projects/Neocore/Service/tests/src/ServiceConfig_test.cpp

tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utest.dir/ServiceConfig_test.cpp.i"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saurex83/Projects/Neocore/Service/tests/src/ServiceConfig_test.cpp > CMakeFiles/utest.dir/ServiceConfig_test.cpp.i

tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utest.dir/ServiceConfig_test.cpp.s"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saurex83/Projects/Neocore/Service/tests/src/ServiceConfig_test.cpp -o CMakeFiles/utest.dir/ServiceConfig_test.cpp.s

tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o.requires:

.PHONY : tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o.requires

tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o.provides: tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o.requires
	$(MAKE) -f tests/src/CMakeFiles/utest.dir/build.make tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o.provides.build
.PHONY : tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o.provides

tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o.provides.build: tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o


tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o: tests/src/CMakeFiles/utest.dir/flags.make
tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o: tests/src/Transiver_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saurex83/Projects/Neocore/Service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utest.dir/Transiver_test.cpp.o -c /home/saurex83/Projects/Neocore/Service/tests/src/Transiver_test.cpp

tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utest.dir/Transiver_test.cpp.i"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saurex83/Projects/Neocore/Service/tests/src/Transiver_test.cpp > CMakeFiles/utest.dir/Transiver_test.cpp.i

tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utest.dir/Transiver_test.cpp.s"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saurex83/Projects/Neocore/Service/tests/src/Transiver_test.cpp -o CMakeFiles/utest.dir/Transiver_test.cpp.s

tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o.requires:

.PHONY : tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o.requires

tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o.provides: tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o.requires
	$(MAKE) -f tests/src/CMakeFiles/utest.dir/build.make tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o.provides.build
.PHONY : tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o.provides

tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o.provides.build: tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o


tests/src/CMakeFiles/utest.dir/database_test.cpp.o: tests/src/CMakeFiles/utest.dir/flags.make
tests/src/CMakeFiles/utest.dir/database_test.cpp.o: tests/src/database_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saurex83/Projects/Neocore/Service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tests/src/CMakeFiles/utest.dir/database_test.cpp.o"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utest.dir/database_test.cpp.o -c /home/saurex83/Projects/Neocore/Service/tests/src/database_test.cpp

tests/src/CMakeFiles/utest.dir/database_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utest.dir/database_test.cpp.i"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saurex83/Projects/Neocore/Service/tests/src/database_test.cpp > CMakeFiles/utest.dir/database_test.cpp.i

tests/src/CMakeFiles/utest.dir/database_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utest.dir/database_test.cpp.s"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saurex83/Projects/Neocore/Service/tests/src/database_test.cpp -o CMakeFiles/utest.dir/database_test.cpp.s

tests/src/CMakeFiles/utest.dir/database_test.cpp.o.requires:

.PHONY : tests/src/CMakeFiles/utest.dir/database_test.cpp.o.requires

tests/src/CMakeFiles/utest.dir/database_test.cpp.o.provides: tests/src/CMakeFiles/utest.dir/database_test.cpp.o.requires
	$(MAKE) -f tests/src/CMakeFiles/utest.dir/build.make tests/src/CMakeFiles/utest.dir/database_test.cpp.o.provides.build
.PHONY : tests/src/CMakeFiles/utest.dir/database_test.cpp.o.provides

tests/src/CMakeFiles/utest.dir/database_test.cpp.o.provides.build: tests/src/CMakeFiles/utest.dir/database_test.cpp.o


tests/src/CMakeFiles/utest.dir/frame_test.cpp.o: tests/src/CMakeFiles/utest.dir/flags.make
tests/src/CMakeFiles/utest.dir/frame_test.cpp.o: tests/src/frame_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saurex83/Projects/Neocore/Service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object tests/src/CMakeFiles/utest.dir/frame_test.cpp.o"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utest.dir/frame_test.cpp.o -c /home/saurex83/Projects/Neocore/Service/tests/src/frame_test.cpp

tests/src/CMakeFiles/utest.dir/frame_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utest.dir/frame_test.cpp.i"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saurex83/Projects/Neocore/Service/tests/src/frame_test.cpp > CMakeFiles/utest.dir/frame_test.cpp.i

tests/src/CMakeFiles/utest.dir/frame_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utest.dir/frame_test.cpp.s"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saurex83/Projects/Neocore/Service/tests/src/frame_test.cpp -o CMakeFiles/utest.dir/frame_test.cpp.s

tests/src/CMakeFiles/utest.dir/frame_test.cpp.o.requires:

.PHONY : tests/src/CMakeFiles/utest.dir/frame_test.cpp.o.requires

tests/src/CMakeFiles/utest.dir/frame_test.cpp.o.provides: tests/src/CMakeFiles/utest.dir/frame_test.cpp.o.requires
	$(MAKE) -f tests/src/CMakeFiles/utest.dir/build.make tests/src/CMakeFiles/utest.dir/frame_test.cpp.o.provides.build
.PHONY : tests/src/CMakeFiles/utest.dir/frame_test.cpp.o.provides

tests/src/CMakeFiles/utest.dir/frame_test.cpp.o.provides.build: tests/src/CMakeFiles/utest.dir/frame_test.cpp.o


tests/src/CMakeFiles/utest.dir/license_test.cpp.o: tests/src/CMakeFiles/utest.dir/flags.make
tests/src/CMakeFiles/utest.dir/license_test.cpp.o: tests/src/license_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saurex83/Projects/Neocore/Service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object tests/src/CMakeFiles/utest.dir/license_test.cpp.o"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utest.dir/license_test.cpp.o -c /home/saurex83/Projects/Neocore/Service/tests/src/license_test.cpp

tests/src/CMakeFiles/utest.dir/license_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utest.dir/license_test.cpp.i"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saurex83/Projects/Neocore/Service/tests/src/license_test.cpp > CMakeFiles/utest.dir/license_test.cpp.i

tests/src/CMakeFiles/utest.dir/license_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utest.dir/license_test.cpp.s"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saurex83/Projects/Neocore/Service/tests/src/license_test.cpp -o CMakeFiles/utest.dir/license_test.cpp.s

tests/src/CMakeFiles/utest.dir/license_test.cpp.o.requires:

.PHONY : tests/src/CMakeFiles/utest.dir/license_test.cpp.o.requires

tests/src/CMakeFiles/utest.dir/license_test.cpp.o.provides: tests/src/CMakeFiles/utest.dir/license_test.cpp.o.requires
	$(MAKE) -f tests/src/CMakeFiles/utest.dir/build.make tests/src/CMakeFiles/utest.dir/license_test.cpp.o.provides.build
.PHONY : tests/src/CMakeFiles/utest.dir/license_test.cpp.o.provides

tests/src/CMakeFiles/utest.dir/license_test.cpp.o.provides.build: tests/src/CMakeFiles/utest.dir/license_test.cpp.o


tests/src/CMakeFiles/utest.dir/main.cpp.o: tests/src/CMakeFiles/utest.dir/flags.make
tests/src/CMakeFiles/utest.dir/main.cpp.o: tests/src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/saurex83/Projects/Neocore/Service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object tests/src/CMakeFiles/utest.dir/main.cpp.o"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utest.dir/main.cpp.o -c /home/saurex83/Projects/Neocore/Service/tests/src/main.cpp

tests/src/CMakeFiles/utest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utest.dir/main.cpp.i"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/saurex83/Projects/Neocore/Service/tests/src/main.cpp > CMakeFiles/utest.dir/main.cpp.i

tests/src/CMakeFiles/utest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utest.dir/main.cpp.s"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/saurex83/Projects/Neocore/Service/tests/src/main.cpp -o CMakeFiles/utest.dir/main.cpp.s

tests/src/CMakeFiles/utest.dir/main.cpp.o.requires:

.PHONY : tests/src/CMakeFiles/utest.dir/main.cpp.o.requires

tests/src/CMakeFiles/utest.dir/main.cpp.o.provides: tests/src/CMakeFiles/utest.dir/main.cpp.o.requires
	$(MAKE) -f tests/src/CMakeFiles/utest.dir/build.make tests/src/CMakeFiles/utest.dir/main.cpp.o.provides.build
.PHONY : tests/src/CMakeFiles/utest.dir/main.cpp.o.provides

tests/src/CMakeFiles/utest.dir/main.cpp.o.provides.build: tests/src/CMakeFiles/utest.dir/main.cpp.o


# Object files for target utest
utest_OBJECTS = \
"CMakeFiles/utest.dir/SerialCom_test.cpp.o" \
"CMakeFiles/utest.dir/ServiceConfig_test.cpp.o" \
"CMakeFiles/utest.dir/Transiver_test.cpp.o" \
"CMakeFiles/utest.dir/database_test.cpp.o" \
"CMakeFiles/utest.dir/frame_test.cpp.o" \
"CMakeFiles/utest.dir/license_test.cpp.o" \
"CMakeFiles/utest.dir/main.cpp.o"

# External object files for target utest
utest_EXTERNAL_OBJECTS =

bin/utest: tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o
bin/utest: tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o
bin/utest: tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o
bin/utest: tests/src/CMakeFiles/utest.dir/database_test.cpp.o
bin/utest: tests/src/CMakeFiles/utest.dir/frame_test.cpp.o
bin/utest: tests/src/CMakeFiles/utest.dir/license_test.cpp.o
bin/utest: tests/src/CMakeFiles/utest.dir/main.cpp.o
bin/utest: tests/src/CMakeFiles/utest.dir/build.make
bin/utest: /usr/lib/libPocoNetSSL.so.50
bin/utest: src/libService_lib.a
bin/utest: tests/gtest/src/gtest-build/lib/libgtest.a
bin/utest: tests/gtest/src/gtest-build/lib/libgmock.a
bin/utest: /usr/lib/x86_64-linux-gnu/libpq.so
bin/utest: /usr/lib/x86_64-linux-gnu/libboost_log.a
bin/utest: /usr/lib/x86_64-linux-gnu/libboost_system.a
bin/utest: /usr/lib/x86_64-linux-gnu/libboost_date_time.a
bin/utest: /usr/lib/x86_64-linux-gnu/libboost_log_setup.a
bin/utest: /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
bin/utest: /usr/lib/x86_64-linux-gnu/libboost_thread.a
bin/utest: /usr/lib/x86_64-linux-gnu/libboost_regex.a
bin/utest: /usr/lib/x86_64-linux-gnu/libboost_chrono.a
bin/utest: /usr/lib/x86_64-linux-gnu/libboost_atomic.a
bin/utest: /usr/lib/x86_64-linux-gnu/libpthread.so
bin/utest: /usr/lib/libPocoUtil.so.50
bin/utest: /usr/lib/libPocoJSON.so.50
bin/utest: /usr/lib/libPocoXML.so.50
bin/utest: /usr/lib/x86_64-linux-gnu/libexpat.so
bin/utest: /usr/lib/libPocoNet.so.50
bin/utest: /usr/lib/libPocoCrypto.so.50
bin/utest: /usr/lib/libPocoFoundation.so.50
bin/utest: /usr/lib/x86_64-linux-gnu/libpcre.so
bin/utest: /usr/lib/x86_64-linux-gnu/libz.so
bin/utest: /usr/lib/x86_64-linux-gnu/libssl.so
bin/utest: /usr/lib/x86_64-linux-gnu/libcrypto.so
bin/utest: tests/src/CMakeFiles/utest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/saurex83/Projects/Neocore/Service/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable ../../bin/utest"
	cd /home/saurex83/Projects/Neocore/Service/tests/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/utest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/src/CMakeFiles/utest.dir/build: bin/utest

.PHONY : tests/src/CMakeFiles/utest.dir/build

tests/src/CMakeFiles/utest.dir/requires: tests/src/CMakeFiles/utest.dir/SerialCom_test.cpp.o.requires
tests/src/CMakeFiles/utest.dir/requires: tests/src/CMakeFiles/utest.dir/ServiceConfig_test.cpp.o.requires
tests/src/CMakeFiles/utest.dir/requires: tests/src/CMakeFiles/utest.dir/Transiver_test.cpp.o.requires
tests/src/CMakeFiles/utest.dir/requires: tests/src/CMakeFiles/utest.dir/database_test.cpp.o.requires
tests/src/CMakeFiles/utest.dir/requires: tests/src/CMakeFiles/utest.dir/frame_test.cpp.o.requires
tests/src/CMakeFiles/utest.dir/requires: tests/src/CMakeFiles/utest.dir/license_test.cpp.o.requires
tests/src/CMakeFiles/utest.dir/requires: tests/src/CMakeFiles/utest.dir/main.cpp.o.requires

.PHONY : tests/src/CMakeFiles/utest.dir/requires

tests/src/CMakeFiles/utest.dir/clean:
	cd /home/saurex83/Projects/Neocore/Service/tests/src && $(CMAKE_COMMAND) -P CMakeFiles/utest.dir/cmake_clean.cmake
.PHONY : tests/src/CMakeFiles/utest.dir/clean

tests/src/CMakeFiles/utest.dir/depend:
	cd /home/saurex83/Projects/Neocore/Service && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/saurex83/Projects/Neocore/Service /home/saurex83/Projects/Neocore/Service/tests/src /home/saurex83/Projects/Neocore/Service /home/saurex83/Projects/Neocore/Service/tests/src /home/saurex83/Projects/Neocore/Service/tests/src/CMakeFiles/utest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/src/CMakeFiles/utest.dir/depend

