file(GLOB_RECURSE SOURCES *.c)

add_executable(bot ${SOURCES})

set_target_properties(bot PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
