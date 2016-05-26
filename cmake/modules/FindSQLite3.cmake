## Find the SQLite3 library.
#
# The following variables are optionally searched for defaults
#  SQLITE3_ROOT
#
# The following are set after configuration is done:
#  SQLITE3_FOUND
#  SQLITE3_INCLUDE_DIRS
#  SQLITE3_LIBRARIES

include (TbagSimpleFindLibrary)

set (_headers  "sqlite3.h")
set (_libs     "sqlite3")

tbag_simple_find_library (SQLITE3 "${_headers}" "${_libs}")

