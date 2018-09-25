file(GLOB_RECURSE SOURCES *.c)
# Disregard Emacs' temporary files.
list(FILTER SOURCES EXCLUDE REGEX "[.]#.*")

add_library(ircbot ${SOURCES})

if(WIN32)
  target_link_libraries(ircbot ws2_32)
endif(WIN32)

target_include_directories(ircbot PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/")
set_target_properties(ircbot PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
