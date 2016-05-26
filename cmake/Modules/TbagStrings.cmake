#/// @file   TbagStrings.cmake
#/// @brief  String & List utilities.
#/// @author zer0
#/// @date   2016-05-25

#/// Arguments to string.
#///
#/// @param __result    [out] Output value name.
#/// @param __delimiter [in]  Delimiter string.
#/// @param ...         [in]  List of arguments.
function (tabg_arguments_to_string __result __delimiter)
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

    unset (__args_cursor)
    unset (__is_first_setting)

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// List of regex.
#///
#/// @param __result  [out] output value name.
#/// @param __regex   [in]  Regex string.
#/// @param __source  [in]  Original list.
function (tbag_list_regex __result __regex __source)
    set (${__result})

    foreach (__list_cursor ${__source})
        string (REGEX MATCH "${__regex}" __match_cursor ${__list_cursor})
        list (APPEND ${__result} ${__match_cursor})
    endforeach ()

    unset (__list_cursor)
    unset (__match_cursor)

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

