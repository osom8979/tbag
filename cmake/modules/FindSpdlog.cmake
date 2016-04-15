## Find the speed-log library.
#
# The following variables are optionally searched for defaults
#  SPDLOG_ROOT
#
# The following are set after configuration is done:
#  SPDLOG_FOUND
#  SPDLOG_INCLUDE_DIRS
#  SPDLOG_LIBRARIES

include (SimpleFindLibrary)

set (_headers  "spdlog/spdlog.h")

simple_find_library (SPDLOG "${_headers}" "")

