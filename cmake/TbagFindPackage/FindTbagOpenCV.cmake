#/// @file   FindTbagOpenCV.cmake
#/// @brief  Find the OpenCV library.
#/// @author zer0
#/// @date   2016-05-31
#///
#/// @remarks
#///  The following variables are optionally searched for defaults:
#///    - ${OpenCV_ROOT}

if (NOT DEFINED OpenCV_ROOT)
    if (IS_DIRECTORY "${THIRD_PREFIX}")
        set (OpenCV_ROOT "${THIRD_PREFIX}")
    else ()
        set (OpenCV_ROOT "/usr/local")
    endif ()
endif ()

set (__tbag_opencv_module_dir "${OpenCV_ROOT}/share/OpenCV")
if (NOT IS_DIRECTORY "${__tbag_opencv_module_dir}")
    if (IS_DIRECTORY "/usr/local/share/OpenCV")
        set (__tbag_opencv_module_dir "/usr/local/share/OpenCV")
    else ()
        #message (WARNING "Not found OpenCV Modules directory: ${__tbag_opencv_module_dir}")
    endif ()
endif ()

if (NOT OpenCV_FOUND)
    list (FIND "${CMAKE_MODULE_PATH}" "${__tbag_opencv_module_dir}" __opencv_module_index)
    if (__opencv_module_index EQUAL -1)
        list (APPEND CMAKE_MODULE_PATH "${__tbag_opencv_module_dir}")
    endif ()

    if (TbagOpenCV_FIND_QUIETLY)
        set (__tbag_opencv_quiet QUIET)
    else ()
        set (__tbag_opencv_quiet)
    endif ()

    find_package (OpenCV ${__tbag_opencv_quiet} REQUIRED)
endif ()

if (OpenCV_FOUND)
    message (STATUS "Found OpenCV: ${OpenCV_LIBS}")
endif ()

