## Find the SQLite3 library.
#
# The following variables are optionally searched for defaults
#  SQLite3_ROOT
#
# The following are set after configuration is done:
#  SQLite3_FOUND
#  SQLite3_INCLUDE_DIRS
#  SQLite3_LIBRARIES

set (__headers  "sqlite3.h")
set (__libs     "sqlite3")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

