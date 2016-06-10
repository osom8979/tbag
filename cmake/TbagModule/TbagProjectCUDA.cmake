#/// @file   TbagProjectCUDA.cmake
#/// @brief  NVIDIA CUDA project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT CUDA_FOUND)
    message (WARNING "Not found CUDA.")
endif ()

cuda_include_directories (${TBAG_PROJECT_INCLUDE_DIRS})

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CUDA_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${CUDA_LIBRARIES})

