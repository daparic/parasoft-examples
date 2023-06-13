cmake_minimum_required(VERSION 3.10)

project(mod3)

add_library(mod3 STATIC mod3.cpp)
target_link_libraries(mod3 PUBLIC mod1 mod2)
target_include_directories(mod3 PUBLIC include)