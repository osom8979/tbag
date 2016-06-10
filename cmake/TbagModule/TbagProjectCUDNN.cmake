#/// @file   TbagProjectCUDNN.cmake
#/// @brief  NVIDIA CUDA cuDNN project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT CUDNN_FOUND)
    message (WARNING "Not found cuDNN.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CUDNN_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${CUDNN_LIBRARIES})

