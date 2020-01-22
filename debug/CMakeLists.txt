file(GLOB SOURCES *.c)

add_library(libdebug ${SOURCES})

target_include_directories(libdebug PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")
set_target_properties(libdebug PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
