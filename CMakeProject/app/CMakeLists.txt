add_executable(main main.cpp util.cpp)
target_link_libraries(main PUBLIC mod1 mod2 mod3)

# C/C++test test project 'cpptest_app'
# Project will be located in ${CMAKE_SOURCE_DIR}/tests/cpptest_app
# Current directory (./) will be linked as 'app'
# Test cases will be generated into ${CMAKE_SOURCE_DIR}/tests/cpptest_app/tests
cpptest_add_executable(
    cpptest_app
    CPPTEST_PROJECT_LOC
        ${CMAKE_SOURCE_DIR}/tests/cpptest_app
    CPPTEST_PROJECT_FOLDERS
        app=${CMAKE_CURRENT_SOURCE_DIR}
    SOURCES
        main.cpp util.cpp
)
target_link_libraries(cpptest_app PRIVATE mod1 mod2 mod3)
