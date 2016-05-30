## Find the speed-log library.
#
# The following variables are optionally searched for defaults
#  Spdlog_ROOT
#
# The following are set after configuration is done:
#  Spdlog_FOUND
#  Spdlog_INCLUDE_DIRS
#  Spdlog_LIBRARIES

set (__headers "spdlog/spdlog.h")
set (__libs    "")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

