## Find the LMDB library.
#
# The following variables are optionally searched for defaults
#  LMDB_ROOT
#
# The following are set after configuration is done:
#  LMDB_FOUND
#  LMDB_INCLUDE_DIRS
#  LMDB_LIBRARIES

include (TbagSimpleFindLibrary)

set (_headers  "lmdb.h")
set (_libs     "lmdb")

tbag_simple_find_library (LMDB "${_headers}" "${_libs}")

