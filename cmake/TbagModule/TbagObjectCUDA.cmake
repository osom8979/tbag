#/// @file   TbagObjectCUDA.cmake
#/// @brief  NVIDIA CUDA object module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT CUDA_FOUND)
    find_package (CUDA)
endif ()

if (CUDA_FOUND)
    foreach (__cuda_cusor "${TBAG_OBJECT_CONST_SOURCES}")
        cuda_compile (__cuda_object ${__cuda_cusor})
        list (APPEND TBAG_OBJECT_OBJECTS ${__cuda_object})
    endforeach ()

    list (APPEND TBAG_OBJECT_INCLUDE_DIRS ${CUDA_INCLUDE_DIRS})
    list (APPEND TBAG_OBJECT_LDFLAGS ${CUDA_LIBRARIES})
    if (USE_CUBLAS)
        list (APPEND TBAG_OBJECT_LDFLAGS ${CUDA_CUBLAS_LIBRARIES})
    endif ()
endif ()

