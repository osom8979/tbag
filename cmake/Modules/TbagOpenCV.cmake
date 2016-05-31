#/// @file   TbagOpenCV.cmake
#/// @brief  tbag_opencv__find_package function prototype.
#/// @author zer0
#/// @date   2016-05-31
#///
#/// @remarks
#///  The following variables are optionally searched for defaults
#///    OpenCV_ROOT

# OpenCV settings:
macro (tbag_opencv__find_package)
    if (NOT DEFINED OpenCV_ROOT)
        set (OpenCV_ROOT "${THIRD_PREFIX}")
    endif ()

    set (__opencv_module_dir "${OpenCV_ROOT}/share/OpenCV")
    if (NOT IS_DIRECTORY "${__opencv_module_dir}")
        message (FATAL_ERROR "Not found OpenCV Modules directory: ${__opencv_module_dir}")
    endif ()

    list (APPEND CMAKE_MODULE_PATH "${__opencv_module_dir}")
    find_package (OpenCV REQUIRED)
    if (OpenCV_FOUND)
        message (STATUS "Found OpenCV: ${OpenCV_LIBS}")
    endif ()
endmacro ()

