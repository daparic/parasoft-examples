cmake_minimum_required(VERSION 3.10)

project(cpptest-unittest-cmake)

include(../../integration/cmake/cpptest-project.cmake)

add_subdirectory(app)
add_subdirectory(modules)
add_subdirectory(tests)