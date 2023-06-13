cmake_minimum_required(VERSION 3.10)

project(Timer)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(CPPTEST_COVERAGE)
  include(../../integration/cmake/cpptest-coverage.cmake)
endif()

add_executable(timer
 clock.c
 driver.c
 timer.c
 stdinout.c)
