cmake_minimum_required(VERSION 3.10)

project(mod1)

add_library(mod1 STATIC mod1.cpp)
target_include_directories(mod1 PUBLIC include)

# C/C++test test project 'cpptest_mod1'
# Project will be located in the current directory (./)
# Test cases will be generated into ./tests
cpptest_add_executable(cpptest_mod1
    CPPTEST_COMPILER_ID gcc_9-64
    SOURCES mod1.cpp)
target_include_directories(cpptest_mod1 PRIVATE include)
