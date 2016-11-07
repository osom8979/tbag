## Find the LevelDB library.
#
# The following variables are optionally searched for defaults
#  LevelDB_ROOT
#
# The following are set after configuration is done:
#  LevelDB_FOUND
#  LevelDB_INCLUDE_DIRS
#  LevelDB_LIBRARIES

if (LevelDB_FOUND)
    return ()
endif ()

set (__headers "leveldb/options.h")
set (__libs    "leveldb")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

