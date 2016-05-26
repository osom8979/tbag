## Find the snappy library.
#
# The following variables are optionally searched for defaults
#  SNAPPY_ROOT
#
# The following are set after configuration is done:
#  SNAPPY_FOUND
#  SNAPPY_INCLUDE_DIRS
#  SNAPPY_LIBRARIES

set (__prefix   SNAPPY)
set (__headers  "snappy.h")
set (__libs     "snappy")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

