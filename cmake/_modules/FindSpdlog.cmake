## Find the speed-log library.
#
# The following variables are optionally searched for defaults
#  SPDLOG_ROOT
#
# The following are set after configuration is done:
#  SPDLOG_FOUND
#  SPDLOG_INCLUDE_DIRS
#  SPDLOG_LIBRARIES

set (__prefix   SPDLOG)
set (__headers  "spdlog/spdlog.h")
set (__libs     "")

include (TbagSimpleFindLibrary)
tbag_simple_find_library ("${__prefix}" "${__headers}" "${__libs}")

