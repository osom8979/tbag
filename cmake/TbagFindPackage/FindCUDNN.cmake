## Find the cuDNN library.
#
# The following variables are optionally searched for defaults
#  CUDNN_ROOT
#
# The following are set after configuration is done:
#  CUDNN_FOUND
#  CUDNN_INCLUDE_DIRS
#  CUDNN_LIBRARIES
#  CUDNN_VERSION

if (CUDNN_FOUND)
    return ()
endif ()

set (CUDNN_ROOT_INCLUDE_PATHS "/usr/cuda/include"
                              "/usr/local/cuda/include"
                              "${CUDA_TOOLKIT_ROOT_DIR}/include"
                              ${CUDNN_ROOT_INCLUDE_PATHS})
set (CUDNN_ROOT_LIBRARY_PATHS "/usr/cuda/lib"
                              "/usr/local/cuda/lib"
                              "${CUDA_TOOLKIT_ROOT_DIR}/lib"
                              ${CUDNN_ROOT_LIBRARY_PATHS})

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    if (WIN32)
        list (INSERT CUDNN_ROOT_LIBRARY_PATHS 0 "${CUDA_TOOLKIT_ROOT_DIR}/lib/x64")
    else ()
        list (INSERT CUDNN_ROOT_LIBRARY_PATHS 0 "/usr/cuda/lib64" "/usr/local/cuda/lib64")
    endif ()
else ()
    if (WIN32)
        list (APPEND CUDNN_ROOT_LIBRARY_PATHS 0 "${CUDA_TOOLKIT_ROOT_DIR}/lib/Win32")
    endif ()
endif ()

set (__headers "cudnn.h")
set (__libs    "cudnn")

include (TbagSimpleFindLibrary)
tbag_simple_find_library_with_module_prefix ("${__headers}" "${__libs}")

if (CUDNN_FOUND)
    list (GET CUDNN_INCLUDE_DIRS 0 __cudnn_include_dir_0)
    file (READ "${__cudnn_include_dir_0}/cudnn.h" __cudnn_version_file_contents)

    string (REGEX MATCH   "define CUDNN_MAJOR * +([0-9]+)"        CUDNN_VERSION_MAJOR "${__cudnn_version_file_contents}")
    string (REGEX REPLACE "define CUDNN_MAJOR * +([0-9]+)" "\\1"  CUDNN_VERSION_MAJOR "${CUDNN_VERSION_MAJOR}")

    string (REGEX MATCH   "define CUDNN_MINOR * +([0-9]+)"        CUDNN_VERSION_MINOR "${__cudnn_version_file_contents}")
    string (REGEX REPLACE "define CUDNN_MINOR * +([0-9]+)" "\\1"  CUDNN_VERSION_MINOR "${CUDNN_VERSION_MINOR}")

    string (REGEX MATCH   "define CUDNN_PATCHLEVEL * +([0-9]+)"        CUDNN_VERSION_PATCH "${__cudnn_version_file_contents}")
    string (REGEX REPLACE "define CUDNN_PATCHLEVEL * +([0-9]+)" "\\1"  CUDNN_VERSION_PATCH "${CUDNN_VERSION_PATCH}")

    if (NOT CUDNN_VERSION_MAJOR)
        set (CUDNN_VERSION)
    else ()
        set (CUDNN_VERSION "${CUDNN_VERSION_MAJOR}.${CUDNN_VERSION_MINOR}.${CUDNN_VERSION_PATCH}")
    endif ()
    message (STATUS "cuDNN Version: ${CUDNN_VERSION}")

    # cuDNN v3 and beyond.
    string (COMPARE LESS "${CUDNN_VERSION_MAJOR}" 3 __cudnn_version_incompatible)
    if (__cuDNN_Version_Incompatible)
        message (FATAL_ERROR "cuDNN version >3 is required.")
    endif ()
endif ()

