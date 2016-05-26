## Find the LevelDB library.
#
# The following variables are optionally searched for defaults
#  LEVELDB_ROOT
#
# The following are set after configuration is done:
#  LEVELDB_FOUND
#  LEVELDB_INCLUDE_DIRS
#  LEVELDB_LIBRARIES

set (__prefix   LEVELDB)
set (__headers  "leveldb/options.h")
set (__libs     "leveldb")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

