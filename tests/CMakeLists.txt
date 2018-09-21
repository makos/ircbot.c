file(GLOB TEST_SOURCES *.c)
file(GLOB_RECURSE SOURCES ${CMAKE_HOME_DIRECTORY}/src/*/*.c)

add_subdirectory(server)

add_executable(tests ${SOURCES} ${TEST_SOURCES})

set_target_properties(tests PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)

target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
