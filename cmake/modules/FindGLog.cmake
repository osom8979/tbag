## Find the Google-glog library.
#
# The following variables are optionally searched for defaults
#  GLOG_ROOT
#
# The following are set after configuration is done:
#  GLOG_FOUND
#  GLOG_INCLUDE_DIRS
#  GLOG_LIBRARIES

include (TbagSimpleFindLibrary)

set (_headers  "glog/logging.h")
set (_libs     "glog")

tbag_simple_find_library (GLOG "${_headers}" "${_libs}")

