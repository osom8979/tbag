## Find the OpenALPR library.
#
# The following variables are optionally searched for defaults
#  OpenALPR_ROOT
#
# The following are set after configuration is done:
#  OpenALPR_FOUND
#  OpenALPR_INCLUDE_DIRS
#  OpenALPR_LIBRARIES

if (OpenALPR_FOUND)
    return ()
endif ()

set (__headers "alpr.h")
set (__libs    "openalpr")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

