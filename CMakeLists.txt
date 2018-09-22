cmake_minimum_required(VERSION 3.0.0)

if (WIN32)
    link_libraries(ws2_32)
endif(WIN32)

if (CMAKE_COMPILER_IS_GNUCC OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"))
    add_compile_options("-Wall")
    add_compile_options("-pedantic")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options("-g")
    endif()
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include_directories(src)
add_subdirectory(libbot)
add_subdirectory(tests)
