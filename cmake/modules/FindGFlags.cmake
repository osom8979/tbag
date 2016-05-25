## Find the Google-gflags library.
#
# The following variables are optionally searched for defaults
#  GFLAGS_ROOT
#
# The following are set after configuration is done:
#  GFLAGS_FOUND
#  GFLAGS_INCLUDE_DIRS
#  GFLAGS_LIBRARIES

include (SimpleFindLibrary)

set (_headers  "gflags/gflags.h")
set (_libs     "gflags")

simple_find_library (GFLAGS "${_headers}" "${_libs}")

