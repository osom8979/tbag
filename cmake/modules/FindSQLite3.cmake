## Find the SQLite3 library.
#
# The following variables are optionally searched for defaults
#  SQLITE3_ROOT
#
# The following are set after configuration is done:
#  SQLITE3_FOUND
#  SQLITE3_INCLUDE_DIRS
#  SQLITE3_LIBRARIES

set (__prefix   SQLITE3)
set (__headers  "sqlite3.h")
set (__libs     "sqlite3")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

