## Find the LMDB library.
#
# The following variables are optionally searched for defaults
#  LMDB_ROOT
#
# The following are set after configuration is done:
#  LMDB_FOUND
#  LMDB_INCLUDE_DIRS
#  LMDB_LIBRARIES

if (LMDB_FOUND)
    return ()
endif ()

set (__headers "lmdb.h")
set (__libs    "lmdb")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

