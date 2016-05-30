## Find the Google-glog library.
#
# The following variables are optionally searched for defaults
#  GLog_ROOT
#
# The following are set after configuration is done:
#  GLog_FOUND
#  GLog_INCLUDE_DIRS
#  GLog_LIBRARIES

set (__headers "glog/logging.h")
set (__libs    "glog")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

