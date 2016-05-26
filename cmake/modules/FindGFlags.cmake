## Find the Google-gflags library.
#
# The following variables are optionally searched for defaults
#  GFLAGS_ROOT
#
# The following are set after configuration is done:
#  GFLAGS_FOUND
#  GFLAGS_INCLUDE_DIRS
#  GFLAGS_LIBRARIES

set (__prefix   GFLAGS)
set (__headers  "gflags/gflags.h")
set (__libs     "gflags")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

