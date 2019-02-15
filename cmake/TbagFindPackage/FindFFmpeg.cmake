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

include (TbagSimpleFindLibrary)
tbag_simple_find_library (avformat   libavformat/avformat.h     avformat)
tbag_simple_find_library (avutil     libavutil/avutil.h         avutil)
tbag_simple_find_library (avcodec    libavcodec/avcodec.h       avcodec)
tbag_simple_find_library (swscale    libswscale/swscale.h       swscale)
tbag_simple_find_library (swresample libswresample/swresample.h swresample)
tbag_simple_find_library (avfilter   libavfilter/avfilter.h     avfilter)
tbag_simple_find_library (avdevice   libavdevice/avdevice.h     avdevice)

if (avformat_FOUND OR avutil_FOUND OR avcodec_FOUND OR swscale_FOUND OR swresample_FOUND OR avfilter_FOUND OR avdevice_FOUND)
    set (FFmpeg_FOUND ON)
else ()
    set (FFmpeg_FOUND OFF)
endif ()

set (FFmpeg_INCLUDE_DIRS)
set (FFmpeg_LIBRARIES)
if (avformat_FOUND)
    list (APPEND FFmpeg_INCLUDE_DIRS ${avformat_INCLUDE_DIRS})
    list (APPEND FFmpeg_LIBRARIES ${avformat_LIBRARIES})
endif ()
if (avutil_FOUND)
    list (APPEND FFmpeg_INCLUDE_DIRS ${avutil_INCLUDE_DIRS})
    list (APPEND FFmpeg_LIBRARIES ${avutil_LIBRARIES})
endif ()
if (avcodec_FOUND)
    list (APPEND FFmpeg_INCLUDE_DIRS ${avcodec_INCLUDE_DIRS})
    list (APPEND FFmpeg_LIBRARIES ${avcodec_LIBRARIES})
endif ()
if (swscale_FOUND)
    list (APPEND FFmpeg_INCLUDE_DIRS ${swscale_INCLUDE_DIRS})
    list (APPEND FFmpeg_LIBRARIES ${swscale_LIBRARIES})
endif ()
if (swresample_FOUND)
    list (APPEND FFmpeg_INCLUDE_DIRS ${swresample_INCLUDE_DIRS})
    list (APPEND FFmpeg_LIBRARIES ${swresample_LIBRARIES})
endif ()
if (avfilter_FOUND)
    list (APPEND FFmpeg_INCLUDE_DIRS ${avfilter_INCLUDE_DIRS})
    list (APPEND FFmpeg_LIBRARIES ${avfilter_LIBRARIES})
endif ()
if (avdevice_FOUND)
    list (APPEND FFmpeg_INCLUDE_DIRS ${avdevice_INCLUDE_DIRS})
    list (APPEND FFmpeg_LIBRARIES ${avdevice_LIBRARIES})
endif ()

list (REMOVE_DUPLICATES FFmpeg_INCLUDE_DIRS)

mark_as_advanced (FFmpeg_FOUND
                  FFmpeg_LIBRARIES
                  FFmpeg_INCLUDE_DIRS)

