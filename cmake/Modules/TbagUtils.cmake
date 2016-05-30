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

if (NOT DEFINED TBAG_CONFIG_TEMPLATE_DIR)
    set (TBAG_CONFIG_TEMPLATE_DIR "${CMAKE_CURRENT_LIST_DIR}/TbagTemplate")
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

## ----------
## Configure.
## ----------

#/// Write configure file.
#///
#/// @param __output_path [in] Output configure file path.
function (tbag_utils__write_configure __template_name __output_path)
    configure_file ("${TBAG_CONFIG_TEMPLATE_DIR}/${__template_name}" "${__output_path}" NEWLINE_STYLE UNIX)

    if (EXISTS "${__output_path}")
        message (STATUS "Write a configure file: ${__output_path}")
    else ()
        message (FATAL_ERROR "Not found configure file: ${__output_path}")
    endif ()
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
function (tabg_utils__get_filename_and_not_extension __result __filename)
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

    tabg_utils__get_filename_and_not_extension (__temp_name "${__filename}")
    string (REGEX REPLACE "^${__prefix}" "" "${__result}" "${__temp_name}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

## --------------
## String & List.
## --------------

#/// Arguments to string.
#///
#/// @param __result    [out] Output value name.
#/// @param __delimiter [in]  Delimiter string.
#/// @param ...         [in]  List of arguments.
function (tabg_utils__list_to_string __result __delimiter)
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
function (tbag_utils__list_regex __result __regex __source)
    set (${__result})

    foreach (__list_cursor ${__source})
        string (REGEX MATCH "${__regex}" __match_cursor ${__list_cursor})
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

## ----------
## Debugging.
## ----------

#/// Logging-like debugging message function.
#///
#/// @param __tag [in] tag name.
#/// @param ...   [in] message arguments.
#///
#/// @remarks
#///  Check the global variables:
#///  - TBAG_DEBUG_TAG: Regex conditions of print.
function (tbag_debug __tag)
    if (TBAG_DEBUG_TAG AND ("${__tag}" MATCHES "${TBAG_DEBUG_TAG}"))
        message (STATUS "@ [${__tag}] " ${ARGN})
    endif ()
endfunction ()

#/// Print variable.
#///
#/// @param __tag      [in] tag name.
#/// @param __variable [in] Variable name.
function (tbag_debug_variable __tag __variable)
    tbag_debug ("${__tag}" "${__variable}: ${${__variable}}")
endfunction ()

#/// Print debug message with list.
#///
#/// @param __tag [in] tag name.
#/// @param ...   [in] list arguments.
function (tbag_debug_list __tag)
    set (__list_index 0)
    foreach (__list_cursor ${ARGN})
        tbag_debug ("${__tag}" "#${__list_index} ${__list_cursor}")
        math (EXPR __list_index "${__list_index} + 1")
    endforeach ()
endfunction ()

## --------------------
## Information Preview.
## --------------------

#/// Print information.
#///
#/// @param ... [in] message arguments.
function (tbag_info_print)
    message ("## " ${ARGN})
endfunction ()

#/// Print variable.
#///
#/// @param __value [in] value name.
function (tbag_info_print_variable __value)
    tbag_info_print ("${__value}: " ${${__value}})
endfunction ()

#/// Print os information.
#///
#/// @remarks
#///  - ${WIN32}: Prior to 2.8.4 this included CygWin.
#///  - ${UNIX}: including Apple OS X and CygWin.
function (tbag_info_print_os)
    if (WIN32)
        if (CYGWIN)
            tbag_info_print ("Windows OS and Cygwin.")
        elseif (MSYS)
            tbag_info_print ("Windows OS and MSYS.")
        else ()
            tbag_info_print ("Windows OS.")
        endif ()
    elseif (UNIX)
        if (APPLE)
            tbag_info_print ("Apple systems.")
        else ()
            tbag_info_print ("Unix-like OS's.")
        endif ()
    else ()
        tbag_info_print ("Unknown OS.")
    endif ()
endfunction ()

#/// Print compiler information.
function (tbag_info_print_compiler)
    # Compiler & IDE:
    # CLANG GNU
    # MINGW MSYS CYGWIN
    # BORLAND WATCOM
    # MSVC MSVC_IDE MSVC60 MSVC70 MSVC71 MSVC80 CMAKE_COMPILER_2005 MSVC90 MSVC10
    message (WARNING "Not implement.")
endfunction ()

#/// Print your compiler.
function (tbag_info_print_compiler_id)
    tbag_info_print ("Compiler id: ${CMAKE_CXX_COMPILER_ID}")
endfunction ()

#/// Preview information.
function (tbag_info_print_preview)
    message ("")
    tbag_info_print ("#################### PREVIEW INFORMATION ####################")
    tbag_info_print ("")

    tbag_info_print_os ()
    tbag_info_print_compiler_id ()
    tbag_info_print ("")

    tbag_info_print_variable (CMAKE_BUILD_TYPE)
    tbag_info_print_variable (BUILD_SHARED_LIBS)
    tbag_info_print ("")

    tbag_info_print_variable (CMAKE_CXX_FLAGS)
    tbag_info_print_variable (CMAKE_EXE_LINKER_FLAGS)
    tbag_info_print_variable (CMAKE_MODULE_LINKER_FLAGS)
    tbag_info_print_variable (CMAKE_SHARED_LINKER_FLAGS)
    tbag_info_print ("")

    tbag_info_print_variable (PROJECT_SOURCE_DIR)
    tbag_info_print_variable (PROJECT_BINARY_DIR)
    tbag_info_print_variable (CMAKE_INSTALL_PREFIX)
    message ("")
endfunction ()

