#/// @file   TbagProjectCURAND.cmake
#/// @brief  NVIDIA CUDA RAND project module.
#/// @author zer0
#/// @date   2016-06-10

if (NOT CUDA_FOUND)
    message (WARNING "Not found CUDA.")
endif ()

list (APPEND TBAG_PROJECT_LDFLAGS ${CUDA_curand_LIBRARY})

