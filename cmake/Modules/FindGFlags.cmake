## Find the Google-gflags library.
#
# The following variables are optionally searched for defaults
#  GFlags_ROOT
#
# The following are set after configuration is done:
#  GFlags_FOUND
#  GFlags_INCLUDE_DIRS
#  GFlags_LIBRARIES

set (__headers  "gflags/gflags.h")
set (__libs     "gflags")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

