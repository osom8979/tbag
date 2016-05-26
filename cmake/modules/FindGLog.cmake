## Find the Google-glog library.
#
# The following variables are optionally searched for defaults
#  GLOG_ROOT
#
# The following are set after configuration is done:
#  GLOG_FOUND
#  GLOG_INCLUDE_DIRS
#  GLOG_LIBRARIES

set (__prefix   GLOG)
set (__headers  "glog/logging.h")
set (__libs     "glog")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

