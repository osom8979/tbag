#/// @file   TbagUtils.cmake
#/// @brief  Common cmake utilities.
#/// @author zer0
#/// @date   2016-05-26

## -----------------
## Global variables.
## -----------------

if (NOT DEFINED TBAG_PATH_SEPARATOR)
    if ((WIN32) AND (NOT MSYS) AND (NOT CYGWIN))
        set (TBAG_PATH_SEPARATOR ";")
    else ()
        set (TBAG_PATH_SEPARATOR ":")
    endif ()
endif ()

## -------------------
## Variable utilities.
## -------------------

#/// Check the exists define.
#/// If not exists, it send a fatal error.
#///
#/// @param __value [in] check value name.
function (tbag_utils__exists_define_or_die __value)
    if (NOT DEFINED ${__value})
        message (FATAL_ERROR "Not defined ${__value} variable.")
    endif()
endfunction ()

## -----------
## Filesystem.
## -----------

#/// Convert msys path to windows path.
#///
#/// @param __value [out] output value name.
#/// @param __paths [in]  MSYS path list.
function (tbag_utils__convert_msys_to_windows_path __value __paths)
    set (${__value})

    foreach (__path_cursor ${__paths})
        string (REGEX REPLACE "^/([a-zA-Z])/" "\\1:/" __replace_cursor1 ${__path_cursor})
        string (REGEX REPLACE "/+" "\\\\" __replace_cursor2 ${__replace_cursor1})
        list (APPEND ${__value} ${__replace_cursor2})
    endforeach ()

    # update result.
    set (${__value} ${${__value}} PARENT_SCOPE)
endfunction ()

