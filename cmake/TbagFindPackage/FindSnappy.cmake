## Find the Snappy library.
#
# The following variables are optionally searched for defaults
#  Snappy_ROOT
#
# The following are set after configuration is done:
#  Snappy_FOUND
#  Snappy_INCLUDE_DIRS
#  Snappy_LIBRARIES

if (Snappy_FOUND)
    return ()
endif ()

set (__headers "snappy.h")
set (__libs    "snappy")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

