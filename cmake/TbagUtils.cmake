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

set (TBAG_CMAKE_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
if (NOT IS_DIRECTORY "${TBAG_CMAKE_ROOT_DIR}")
    message (FATAL_ERROR "Not found directory: ${TBAG_CMAKE_ROOT_DIR}")
endif()

if (NOT DEFINED TBAG_FIND_PACKAGE_DIR)
    set (TBAG_FIND_PACKAGE_DIR "${TBAG_CMAKE_ROOT_DIR}/TbagFindPackage")
endif ()

## ------------
## Path macros.
## ------------

#/// Append TbagFindPackage directory.
macro (tbag_utils__append_find_package_path)
    if (NOT IS_DIRECTORY "${TBAG_FIND_PACKAGE_DIR}")
        message (FATAL_ERROR "Not found directory: ${TBAG_FIND_PACKAGE_DIR}")
    endif()
    list (APPEND CMAKE_MODULE_PATH "${TBAG_FIND_PACKAGE_DIR}")
endmacro ()

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

#/// Obtain file name. (not contain extension)
#///
#/// @param __result   [out] output value name.
#/// @param __filename [in]  File name.
#///
#/// @warning
#///  Don't use regex in string() cmake method.
function (tbag_utils__get_filename_and_not_extension __result __filename)
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
function (tbag_utils__current_module_name __result __filename __prefix)
    set (${__result})

    tbag_utils__get_filename_and_not_extension (__temp_name "${__filename}")
    string (REGEX REPLACE "^${__prefix}" "" "${__result}" "${__temp_name}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

## --------------
## String & List.
## --------------

#/// size of arguments.
#///
#/// @param __result [out] Output value name.
#/// @param ...      [in]  List of arguments.
function (tbag_utils__get_args_size __result)
    set (${__result} 0)

    foreach (__args_cursor ${ARGN})
        math (EXPR ${__result} "${${__result}} + 1")
    endforeach ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// obtain arguments[index].
#///
#/// @param __result [out] Output value name.
#/// @param __index  [in]  Argument index.
#/// @param ...      [in]  List of arguments.
function (tbag_utils__get_args_index __result __index)
    set (${__result})
    set (__index_count 0)

    foreach (__args_cursor ${ARGN})
        if (${__index_count} EQUAL ${__index})
            set (${__result} ${__args_cursor})
            break ()
        endif ()
        math (EXPR __index_count "${__index_count} + 1")
    endforeach ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Arguments to string.
#///
#/// @param __result    [out] Output value name.
#/// @param __delimiter [in]  Delimiter string.
#/// @param ...         [in]  List of arguments.
function (tbag_utils__list_to_string __result __delimiter)
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
#/// @param ...       [in]  Original list.
function (tbag_utils__list_of_regex __result __regex)
    set (${__result})

    foreach (__argument_cursor ${ARGN})
        string (REGEX MATCH "${__regex}" __match_cursor ${__argument_cursor})
        list (APPEND ${__result} ${__match_cursor})
    endforeach ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Remove quoring.
#///
#/// @param __result [out] output value name.
#/// @param __string [in]  string.
function (tbag_utils__remove_quoting __result __string)
    set (${__result})

    #if ("${__string}" MATCHES "^\".*\"$")
    #    string (REGEX REPLACE "^\"" "" __temp_string ${${__result}})
    #    set (${__result} ${__temp_string})
    #    string (REGEX REPLACE "\"$" "" __temp_string ${${__result}})
    #    set (${__result} ${__temp_string})
    #endif ()
    #if ("${__string}" MATCHES "^'.*'$")
    #    string (REGEX REPLACE "^'" "" __temp_string ${${__result}})
    #    set (${__result} ${__temp_string})
    #    string (REGEX REPLACE "'$" "" __temp_string ${${__result}})
    #    set (${__result} ${__temp_string})
    #endif ()

    if ("${__string}" MATCHES "^['\"](.*)['\"]$")
        set (${__result} "${CMAKE_MATCH_1}")
    else ()
        set (${__result} "${__string}")
    endif ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

