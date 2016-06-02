#/// @file   TbagProjectCUDA.cmake
#/// @brief  NVIDIA CUDA project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT CUDA_FOUND)
    find_package (CUDA)
endif ()

if (CUDA_FOUND)
    cuda_include_directories (${TBAG_PROJECT_INCLUDE_DIRS})
    if (CMAKE_CXX_STANDARD EQUAL 11)
        set (CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++11")
    endif ()
endif ()

