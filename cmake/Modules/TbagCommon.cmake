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
function (tbag_common__exists_define_or_die __value)
    if (NOT DEFINED ${__value})
        message (FATAL_ERROR "Not defined ${__value} variable.")
    endif()
endfunction ()

