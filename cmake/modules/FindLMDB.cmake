## Find the LMDB library.
#
# The following variables are optionally searched for defaults
#  LMDB_ROOT
#
# The following are set after configuration is done:
#  LMDB_FOUND
#  LMDB_INCLUDE_DIRS
#  LMDB_LIBRARIES

set (__prefix   LMDB)
set (__headers  "lmdb.h")
set (__libs     "lmdb")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

