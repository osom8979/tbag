## Find the ncurses library.
#
# The following variables are optionally searched for defaults
#  NCurses_ROOT
#
# The following are set after configuration is done:
#  NCurses_FOUND
#  NCurses_INCLUDE_DIRS
#  NCurses_LIBRARIES

set (__headers "ncurses/curses.h")
set (__libs    "ncurses")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

