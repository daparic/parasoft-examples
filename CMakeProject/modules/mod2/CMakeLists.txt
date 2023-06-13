cmake_minimum_required(VERSION 3.10)

project(mod2)

add_library(mod2 STATIC mod2.cpp)
target_include_directories(mod2 PUBLIC include)