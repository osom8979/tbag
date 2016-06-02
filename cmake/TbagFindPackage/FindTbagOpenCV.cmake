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
    message (FATAL_ERROR "Not found OpenCV Modules directory: ${__tbag_opencv_module_dir}")
endif ()

list (APPEND CMAKE_MODULE_PATH "${__tbag_opencv_module_dir}")
find_package (OpenCV REQUIRED)

if (OpenCV_FOUND)
    message (STATUS "Found OpenCV: ${OpenCV_LIBS}")
endif ()

