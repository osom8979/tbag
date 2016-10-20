#/// @file   FindTbagOpenCV.cmake
#/// @brief  Find the OpenCV library.
#/// @author zer0
#/// @date   2016-05-31
#///
#/// @remarks
#///  The following variables are optionally searched for defaults:
#///    - ${OpenCV_ROOT}

## Find opencv-cmake-module.
if (NOT DEFINED OpenCV_DIR)
    message (STATUS "Looking for opencv-cmake-module.")
    find_path (OpenCV_DIR
               NAMES "OpenCVConfig.cmake"
               PATHS "${PROJECT_SOURCE_DIR}/cmake"
                     "${OpenCV_ROOT}"
                     "${OpenCV_ROOT}/build"
                     "${OpenCV_ROOT}/share/OpenCV"
                     "${THIRD_PREFIX}/build"
                     "${THIRD_PREFIX}/share/OpenCV"
                     "$ENV{OPENCV_HOME}"
                     "$ENV{OPENCV_HOME}/build"
                     "$ENV{OPENCV_HOME}/share/OpenCV"
                     "$ENV{TPARTY_HOME}/local/share/OpenCV"
                     "/usr/local/share/OpenCV")
endif ()

if (IS_DIRECTORY "${OpenCV_DIR}")
    message (STATUS "Find OpenCV Modules directory: ${OpenCV_DIR}")
else ()
    message (WARNING "Not found OpenCV Modules directory: ${OpenCV_DIR}")
endif ()

if (NOT OpenCV_FOUND)
    list (FIND "${CMAKE_MODULE_PATH}" "${OpenCV_DIR}" __opencv_module_index)
    if (__opencv_module_index EQUAL -1)
        list (APPEND CMAKE_MODULE_PATH "${OpenCV_DIR}")
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

