#/// @file   TbagStrings.cmake
#/// @brief  String & List utilities.
#/// @author zer0
#/// @date   2016-05-25

#/// Arguments to string.
#///
#/// @param __result    [out] Output value name.
#/// @param __delimiter [in]  Delimiter string.
#/// @param ...         [in]  List of arguments.
function (tabg_strings__arguments_to_string __result __delimiter)
    set (${__result})
    set (__is_first_setting NO)

    foreach (__args_cursor ${ARGN})
        if (NOT __is_first_setting)
            set (__is_first_setting YES)
            set (${__result} "${__args_cursor}")
        else ()
            set (${__result} "${${__result}}${__delimiter}${__args_cursor}")
        endif ()
    endforeach ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// List of regex.
#///
#/// @param __result  [out] output value name.
#/// @param __regex   [in]  Regex string.
#/// @param __source  [in]  Original list.
function (tbag_strings__list_regex __result __regex __source)
    set (${__result})

    foreach (__list_cursor ${__source})
        string (REGEX MATCH "${__regex}" __match_cursor ${__list_cursor})
        list (APPEND ${__result} ${__match_cursor})
    endforeach ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Obtain file name. (not contain extension)
#///
#/// @param __result   [out] output value name.
#/// @param __filename [in]  File name.
#///
#/// @warning
#///  Don't use regex in string() cmake method.
function (tabg_strings__get_filename_and_not_extension __result __filename)
    set (${__result})

    get_filename_component (__temp_name      "${__filename}"  NAME)
    get_filename_component (__temp_extension "${__filename}"  EXT)

    string (LENGTH "${__temp_name}"      __temp_name_length)
    string (LENGTH "${__temp_extension}" __temp_extension_length)

    math (EXPR __temp_substring_length "${__temp_name_length} - ${__temp_extension_length}")
    string (SUBSTRING "${__temp_name}" 0 ${__temp_substring_length} "${__result}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Obtain current cmake script file name.
#///
#/// @param __result [out] output value name.
#/// @param __prefix [in]  File name.
#/// @param __prefix [in]  Module prefix (e.g. Find)
function (tbag_strings__current_module_name __result __filename __prefix)
    set (${__result})

    tabg_strings__get_filename_and_not_extension (__temp_name "${__filename}")
    string (REGEX REPLACE "^${__prefix}" "" "${__result}" "${__temp_name}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

