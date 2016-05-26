## CMake common utility script.

include (TbagFilesystem)
include (TbagInformation)
include (TbagCommon)


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

