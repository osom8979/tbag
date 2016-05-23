## Find the ncurses library.
#
# The following variables are optionally searched for defaults
#  NCURSES_ROOT
#
# The following are set after configuration is done:
#  NCURSES_FOUND
#  NCURSES_INCLUDE_DIRS
#  NCURSES_LIBRARIES

include (SimpleFindLibrary)

set (_headers  "ncurses/curses.h")
set (_libs     "ncurses")

simple_find_library (NCURSES "${_headers}" "${_libs}")

