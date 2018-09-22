file(GLOB TEST_SOURCES *.c)
file(GLOB_RECURSE SOURCES ${CMAKE_HOME_DIRECTORY}/libbot/*/*.c)

list(FILTER TEST_SOURCES EXCLUDE REGEX "[.]#.*")
list(FILTER SOURCES EXCLUDE REGEX "[.]#.*")

add_subdirectory(server)

add_executable(tests ${SOURCES} ${TEST_SOURCES})

if(WIN32)
  target_link_libraries(tests ws2_32)
endif(WIN32)

target_include_directories(tests PUBLIC "${CMAKE_HOME_DIRECTORY}/libbot")
set_target_properties(tests PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)

