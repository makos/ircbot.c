file(GLOB SOURCES *.c)

add_library(ircdebug ${SOURCES})

target_include_directories(ircdebug PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")
set_target_properties(ircdebug PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
