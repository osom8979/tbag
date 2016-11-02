## Find the cuDNN library.
#
# The following variables are optionally searched for defaults
#  CUDNN_ROOT
#
# The following are set after configuration is done:
#  CUDNN_FOUND
#  CUDNN_INCLUDE_DIRS
#  CUDNN_LIBRARIES

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

