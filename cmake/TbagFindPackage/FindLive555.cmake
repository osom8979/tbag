## Find the Live555 (OpenRTSP) library.
#
# The following variables are optionally searched for defaults
#  Live555_ROOT
#
# The following are set after configuration is done:
#  Live555_FOUND
#  Live555_INCLUDE_DIRS
#  Live555_LIBRARIES

if (Live555_FOUND)
    return ()
endif ()

set (__headers BasicUsageEnvironment/BasicUsageEnvironment.hh
               UsageEnvironment/UsageEnvironment.hh
               groupsock/Groupsock.hh
               liveMedia/liveMedia.hh)
set (__libs    BasicUsageEnvironment
               UsageEnvironment
               groupsock
               liveMedia)

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

