## Find the cuDNN library.
#
# The following variables are optionally searched for defaults
#  CUDNN_ROOT
#
# The following are set after configuration is done:
#  CUDNN_FOUND
#  CUDNN_INCLUDE_DIRS
#  CUDNN_LIBRARIES

if (NOT CUDA_FOUND)
    message ("** Find cuDNN: Not found CUDA.")
    return ()
endif ()

set (CUDNN_ROOT_INCLUDE_PATHS "/usr/cuda/include"
                              "/usr/local/cuda/include"
                              ${CUDNN_ROOT_INCLUDE_PATHS})
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set (CUDNN_ROOT_LIBRARY_PATHS "/usr/cuda/lib"
                                  "/usr/cuda/lib64"
                                  "/usr/local/cuda/lib"
                                  "/usr/local/cuda/lib64"
                                  ${CUDNN_ROOT_LIBRARY_PATHS})
else ()
    set (CUDNN_ROOT_LIBRARY_PATHS "/usr/cuda/lib"
                                  "/usr/local/cuda/lib"
                                  ${CUDNN_ROOT_LIBRARY_PATHS})
endif ()

include (SimpleFindLibrary)

set (_headers  "cudnn.h")
set (_libs     "cudnn")

simple_find_library (CUDNN "${_headers}" "${_libs}")

