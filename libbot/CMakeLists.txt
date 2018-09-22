file(GLOB_RECURSE SOURCES *.c)
# Disregard Emacs' temporary files.
list(FILTER SOURCES EXCLUDE REGEX "[.]#.*")

add_library(bot ${SOURCES})

set_target_properties(bot PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
