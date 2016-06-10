#/// @file   TbagObjectCUDA.cmake
#/// @brief  NVIDIA CUDA object module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT CUDA_FOUND)
    message (WARNING "Not found CUDA.")
endif ()

foreach (__cuda_cusor "${TBAG_OBJECT_CONST_SOURCES}")
    cuda_compile (__cuda_object ${__cuda_cusor})
    list (APPEND TBAG_OBJECT_OBJECTS ${__cuda_object})
endforeach ()

