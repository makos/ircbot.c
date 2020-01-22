file(GLOB SOURCES *.c)
list(FILTER SOURCES EXCLUDE REGEX "[.]#.*")

add_executable(chatter ${SOURCES})

target_include_directories(chatter
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/"
  PUBLIC "${CMAKE_HOME_DIRECTORY}/libbot/bot")

target_link_libraries(chatter libbot)
set_target_properties(chatter
  PROPERTIES C_STANDARD 99
  C_STANDARD_REQUIRED ON)
