## Find the WebRTC library.
#
# The following variables are optionally searched for defaults
#  WebRTC_ROOT
#
# The following are set after configuration is done:
#  WebRTC_FOUND
#  WebRTC_INCLUDE_DIRS
#  WebRTC_LIBRARIES

if (WebRTC_FOUND)
    return ()
endif ()

set (__headers "api/rtcerror.h")
set (__libs    "webrtc")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

