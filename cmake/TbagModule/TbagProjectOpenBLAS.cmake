#/// @file   TbagProjectOpenBLAS.cmake
#/// @brief  OpenBLAS project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT OpenBLAS_FOUND)
    find_package (OpenBLAS)
endif ()

if (OpenBLAS_FOUND)
    list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_OPENBLAS)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${OpenBLAS_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${OpenBLAS_LIBRARIES})
endif ()

