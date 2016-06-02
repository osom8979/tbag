#/// @file   TbagProjectCUDNN.cmake
#/// @brief  NVIDIA CUDA cuDNN project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT CUDNN_FOUND)
    find_package (CUDNN)
endif ()

if (CUDNN_FOUND)
    list (APPEND TBAG_PROJECT_DEFINITIONS   -DUSE_CUDNN)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CUDNN_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS       ${CUDNN_LIBRARIES})
endif ()

