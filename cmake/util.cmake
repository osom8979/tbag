## CMake common utility script.

include (TbagFilesystem)
include (TbagInformation)

#! Check the exists define.
# If not exists, it send a fatal error.
#
# \param _value [in] check value name.
function (exists_define_or_die _value)
    if (NOT DEFINED ${_value})
        message (FATAL_ERROR "Not defined ${_value} variable.")
    endif()
endfunction ()

## -----------------
## Module utilities.
## -----------------

#! Test find_package() function.
#
# \param _enable      [in] Enable variable.
# \param _module_name [in] Module name.
macro (test_find_package _enable _module_name)
    if (USE_${_enable})
        find_package (${_module_name})
    endif ()
endmacro ()

## ------------------
## String operations.
## ------------------

#! List of regex.
#
# \param _value [out] output value name.
# \param _regex [in]  Regex string.
# \param _list  [in]  Original list.
function (list_regex _value _regex _list)
    set (${_value})
    foreach (_cursor ${_list})
        string (REGEX MATCH "${_regex}" _match_cursor ${_cursor})
        list (APPEND ${_value} ${_match_cursor})
    endforeach ()
    set (${_value} ${${_value}} PARENT_SCOPE)
endfunction ()

#! Convert msys path to windows path.
#
# \param _value     [out] output value name.
# \param _path_list [in]  MSYS path list.
function (convert_msys_to_windows_path _value _path_list)
    set (${_value})
    foreach (_cursor ${_path_list})
        string (REGEX REPLACE "^/([a-zA-Z])/" "\\1:/" _replace_cursor1 ${_cursor})
        string (REGEX REPLACE "/+" "\\\\" _replace_cursor2 ${_replace_cursor1})
        list (APPEND ${_value} ${_replace_cursor2})
    endforeach ()
    set (${_value} ${${_value}} PARENT_SCOPE)
endfunction ()

