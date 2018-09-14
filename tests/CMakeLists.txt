file(GLOB TEST_SOURCES *.c)
file(GLOB_RECURSE SOURCES ${CMAKE_HOME_DIRECTORY}/src/*/*.c)

add_executable(tests ${SOURCES} ${TEST_SOURCES})

target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
