## Find the OpenH264 library.
#
# The following variables are optionally searched for defaults
#  OpenH264_ROOT
#
# The following are set after configuration is done:
#  OpenH264_FOUND
#  OpenH264_INCLUDE_DIRS
#  OpenH264_LIBRARIES

if (OpenH264_FOUND)
    return ()
endif ()

set (__headers "wels/codec_api.h")
set (__libs    "openh264")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

