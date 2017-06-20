## Find the FFmpeg library.
#
# The following variables are optionally searched for defaults
#  FFmpeg_ROOT
#
# The following are set after configuration is done:
#  FFmpeg_FOUND
#  FFmpeg_INCLUDE_DIRS
#  FFmpeg_LIBRARIES

if (FFmpeg_FOUND)
    return ()
endif ()

set (__headers "libavcodec/avcodec.h"
               "libavformat/avformat.h"
               "libswscale/swscale.h"
               "libavutil/avutil.h"
               "libavdevice/avdevice.h")
set (__libs avformat avutil avcodec swscale swresample avfilter avdevice)

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

