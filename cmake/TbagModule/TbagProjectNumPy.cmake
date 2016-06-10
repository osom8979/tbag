#/// @file   TbagProjectNumPy.cmake
#/// @brief  Python NumPy project module.
#/// @author zer0
#/// @date   2016-06-03

if (NOT NumPy_FOUND)
    message (WARNING "Not found Python NumPy.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${NumPy_INCLUDE_DIRS})

