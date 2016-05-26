## Find the ncurses library.
#
# The following variables are optionally searched for defaults
#  NCURSES_ROOT
#
# The following are set after configuration is done:
#  NCURSES_FOUND
#  NCURSES_INCLUDE_DIRS
#  NCURSES_LIBRARIES

set (__prefix   NCURSES)
set (__headers  "ncurses/curses.h")
set (__libs     "ncurses")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

