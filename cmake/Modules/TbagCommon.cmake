#/// @file   TbagCommon.cmake
#/// @brief  Common cmake utilities.
#/// @author zer0
#/// @date   2016-05-26

## -------------------
## Variable utilities.
## -------------------

#/// Check the exists define.
#/// If not exists, it send a fatal error.
#///
#/// @param __value [in] check value name.
function (tbag_exists_define_or_die __value)
    if (NOT DEFINED ${__value})
        message (FATAL_ERROR "Not defined ${__value} variable.")
    endif()
endfunction ()

## -----------------
## Module utilities.
## -----------------

#/// Test find_package() macro.
#///
#/// @param __enable      [in] Enable variable.
#/// @param __module_name [in] Module name.
macro (tbag_find_package __enable __module_name)
    if (${__enable})
        find_package (${__module_name})
    endif ()
endmacro ()

