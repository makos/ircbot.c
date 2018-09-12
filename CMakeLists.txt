cmake_minimum_required(VERSION 3.0.0)

if (WIN32)
    link_libraries(ws2_32)
endif(WIN32)

if (CMAKE_COMPILER_IS_GNUCC OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"))
    add_compile_options("-Wall")
    add_compile_options("-pedantic")
endif()

include_directories(src)
file(GLOB_RECURSE SOURCES src/*.c)

add_executable(bot ${SOURCES})