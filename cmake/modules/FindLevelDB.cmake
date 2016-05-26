## Find the LevelDB library.
#
# The following variables are optionally searched for defaults
#  LEVELDB_ROOT
#
# The following are set after configuration is done:
#  LEVELDB_FOUND
#  LEVELDB_INCLUDE_DIRS
#  LEVELDB_LIBRARIES

include (TbagSimpleFindLibrary)

set (_headers  "leveldb/options.h")
set (_libs     "leveldb")

tbag_simple_find_library (LEVELDB "${_headers}" "${_libs}")

