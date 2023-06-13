# C/C++test test project 'cpptest_modules'
# Project will be located in the current directory (./)
# Test cases will be generated into ./tests
# mod1 and mod2 source directories will be linked
cpptest_add_executable(
    cpptest_modules
    CPPTEST_PROJECT_FOLDERS
        mod2=${CMAKE_SOURCE_DIR}/modules/mod2
        mod3=${CMAKE_SOURCE_DIR}/modules/mod3
    TARGETS mod2 mod3)
file(GLOB modules_INCLUDES "${CMAKE_SOURCE_DIR}/modules/**/include")
target_include_directories(cpptest_modules PRIVATE ${modules_INCLUDES})
target_link_libraries(cpptest_modules PRIVATE mod1)