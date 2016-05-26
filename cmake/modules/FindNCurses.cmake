## Find the ncurses library.
#
# The following variables are optionally searched for defaults
#  NCURSES_ROOT
#
# The following are set after configuration is done:
#  NCURSES_FOUND
#  NCURSES_INCLUDE_DIRS
#  NCURSES_LIBRARIES

include (TbagSimpleFindLibrary)

set (_headers  "ncurses/curses.h")
set (_libs     "ncurses")

tbag_simple_find_library (NCURSES "${_headers}" "${_libs}")

