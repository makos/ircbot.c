file(GLOB SOURCES *.c)

add_executable(mock_server ${SOURCES})

set_target_properties(mock_server PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
