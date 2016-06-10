#/// @file   TbagProjectOpenBLAS.cmake
#/// @brief  OpenBLAS project module.
#/// @author zer0
#/// @date   2016-06-02

if (NOT OpenBLAS_FOUND)
    message (WARNING "Not found OpenBLAS.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${OpenBLAS_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${OpenBLAS_LIBRARIES})

