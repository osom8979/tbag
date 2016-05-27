#/// @file   TbagFilesystem.cmake
#/// @brief  File system utilities.
#/// @author zer0
#/// @date   2016-05-26

if ((WIN32) AND (NOT MSYS) AND (NOT CYGWIN))
    set (PATH_SEPARATOR ";")
else ()
    set (PATH_SEPARATOR ":")
endif ()

#/// Convert msys path to windows path.
#///
#/// @param __value [out] output value name.
#/// @param __paths [in]  MSYS path list.
function (tbag_filesystem__convert_msys_to_windows_path __value __paths)
    set (${__value})

    foreach (__path_cursor ${__paths})
        string (REGEX REPLACE "^/([a-zA-Z])/" "\\1:/" __replace_cursor1 ${__path_cursor})
        string (REGEX REPLACE "/+" "\\\\" __replace_cursor2 ${__replace_cursor1})
        list (APPEND ${__value} ${__replace_cursor2})
    endforeach ()

    # update result.
    set (${__value} ${${__value}} PARENT_SCOPE)
endfunction ()

