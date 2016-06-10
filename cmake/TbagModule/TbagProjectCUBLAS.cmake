#/// @file   TbagProjectCUBLAS.cmake
#/// @brief  NVIDIA CUDA BLAS project module.
#/// @author zer0
#/// @date   2016-06-10

if (NOT CUDA_FOUND)
    message (WARNING "Not found CUDA.")
endif ()

list (APPEND TBAG_PROJECT_LDFLAGS ${CUDA_CUBLAS_LIBRARIES})

