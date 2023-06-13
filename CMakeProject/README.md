#
# CMake extension for creating C/C++test Professional projects.
#

Requirements:
* CMake 3.10
* C/C++test Professional 2020.1 or newer


How-to:
* C/C++test integration package for CMake is located in <INSTALL_DIR>/integration/cmake/cpptest-project.cmake
* Be sure to include cpptest-project.cmake into your CMakeLists.txt
* Use cpptest_add_executable() to define a target that represents C/C++test Test Project
* Be sure to configure required build options and dependencies - using regular CMake functions (target_include_directories(), target_link_libraries() etc.)
* Be sure to configure C/C++test's compiler identifier - either globally in cpptest-unittest.cmake or per-target using CPPTEST_COMPILER_ID (default compiler identifier is 'gcc_9-64')
* Run CMake 'configure' with '-DCPPTEST_PROJECT=ON' and then CMake 'build' to generate C/C++test project configuration files, that can be imported into Eclipse workspace
* Supported syntax:

    cpptest_add_executable(<target_name> [CPPTEST_COMPILER_ID <compiler_id>] [CPPTEST_PROJECT_NAME <eclipse_project_name>] [CPPTEST_PROJECT_LOC <eclipse_project_location>] [CPPTEST_PROJECT_FOLDERS <name1=location1> <name2=location2> ...] SOURCES <foo.cpp> <goo.cpp> ... | TARGETS <target1> <target2> ...)

* See examples in:
   * app/CMakeLists.txt
   * modules/mod1/CMakeLists.txt
   * tests/cpptest_modules/CMakeLists.txt