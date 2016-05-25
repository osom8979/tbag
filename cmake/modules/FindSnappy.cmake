## Find the snappy library.
#
# The following variables are optionally searched for defaults
#  SNAPPY_ROOT
#
# The following are set after configuration is done:
#  SNAPPY_FOUND
#  SNAPPY_INCLUDE_DIRS
#  SNAPPY_LIBRARIES

include (SimpleFindLibrary)

set (_headers  "snappy.h")
set (_libs     "snappy")

simple_find_library (SNAPPY "${_headers}" "${_libs}")

