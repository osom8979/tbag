#/// @file   TbagProjectNumPy.cmake
#/// @brief  Python NumPy project module.
#/// @author zer0
#/// @date   2016-06-03

if (NOT NumPy_FOUND)
    find_package (NumPy)
endif ()

if (NumPy_FOUND)
    list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_NUMPY)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${NumPy_INCLUDE_DIRS})
endif ()

