#/// @file   TbagInformation.cmake
#/// @brief  Information file utilities.
#/// @author zer0
#/// @date   2016-06-01

include (TbagUtils)
include (TbagDebug)

#/// Read variable.
#///
#/// @param __result  [out] value name of output result.
#/// @param __key     [in]  Key name.
#/// @param __content [in]  Content string.
#///
#/// @remarks
#///  Example content format: <code>KEY=VALUE</code>
function (tbag_information__read_value __result __key __content)
    set (${__result})

    # Find key & value.
    string (REGEX MATCH "${__key}[ \t]*=([^\n]+|$)" __match_content "${__content}")
    if ("${__match_content}" STREQUAL "")
        set (${__result} "" PARENT_SCOPE)
        return ()
    endif ()

    # Remove key name & strip.
    string (REGEX REPLACE "^${__key}[ \t]*=" "" __match_variable "${__match_content}")
    string (STRIP "${__match_variable}" __strip_variable)

    # Remove quoting.
    tbag_utils__remove_quoting (${__result} "${__strip_variable}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Parse version list.
#///
#/// @param __result  [out] value name of output result.
#/// @param __content [in]  Content string.
#///
#/// @remarks
#///  Example content format: <code>0.1.2-3.4-5</code>
function (tbag_information__parse_versions __result __content)
    set (${__result})

    string (REGEX MATCHALL "[0-9]+" ${__result} "${__content}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Parse library list.
#///
#/// @param __result  [out] value name of output result.
#/// @param __content [in]  Content string.
#///
#/// @remarks
#///  Example content format: <code>GTest UV Spdlog</code>
function (tbag_information__parse_libs __result __content)
    set (${__result})

    string (REGEX MATCHALL "[a-zA-Z0-9]+" ${__result} "${__content}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Read version from version-list.
#///
#/// @param __result       [out] value name of output result.
#/// @param __version_list [in]  Version list.
#/// @param __index        [in]  Index of version list.
function (tbag_information__read_version __result __version_list __index)
    set (${__result})

    # list (LENGTH "${__version_list}" __list_length) # [BUG] Not working!!
    tbag_utils__get_args_size (__list_length ${__version_list})

    if (__list_length GREATER ${__index})
        # list (GET ${__version_list} ${__index} ${__result}) # [BUG] Not working!!
        tbag_utils__get_args_index (${__result} ${__index} ${__version_list})
    else ()
        # Default version number: 0
        set (${__result} 0)
    endif ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Read information file.
#///
#/// @param __prefix [in] Result variable prefix.
#/// @param __path   [in] Information file path.
#///
#/// @remarks
#///  - ${__prefix}_INFORMATION_MAIN_NAME
#///  - ${__prefix}_INFORMATION_MAIN_AUTHOR
#///  - ${__prefix}_INFORMATION_MAIN_EMAIL
#///  - ${__prefix}_INFORMATION_MAIN_BRIEF
#///  - ${__prefix}_INFORMATION_VERSION_MAJOR
#///  - ${__prefix}_INFORMATION_VERSION_MINOR
#///  - ${__prefix}_INFORMATION_VERSION_PATCH
#///  - ${__prefix}_INFORMATION_VERSION_PACKET_MAJOR
#///  - ${__prefix}_INFORMATION_VERSION_PACKET_MINOR
#///  - ${__prefix}_INFORMATION_VERSION_RELEASE
function (tbag_information__read_file __prefix __path)
    if (NOT EXISTS "${__path}")
        message (FATAL_ERROR "Not found ${__path}")
    endif ()

    file (READ "${__path}" __information_content)

    tbag_information__read_value (__information_name    NAME    ${__information_content})
    tbag_information__read_value (__information_author  AUTHOR  ${__information_content})
    tbag_information__read_value (__information_email   EMAIL   ${__information_content})
    tbag_information__read_value (__information_brief   BRIEF   ${__information_content})
    tbag_information__read_value (__information_version VERSION ${__information_content})

    tbag_debug (tbag_information__read_file "NAME: ${__information_name}")
    tbag_debug (tbag_information__read_file "AUTHOR: ${__information_author}")
    tbag_debug (tbag_information__read_file "EMAIL: ${__information_email}")
    tbag_debug (tbag_information__read_file "BRIEF: ${__information_brief}")
    tbag_debug (tbag_information__read_file "VERSION: ${__information_version}")

    # Update main information.
    set (${__prefix}_INFORMATION_MAIN_NAME   "${__information_name}"   PARENT_SCOPE)
    set (${__prefix}_INFORMATION_MAIN_AUTHOR "${__information_author}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_MAIN_EMAIL  "${__information_email}"  PARENT_SCOPE)
    set (${__prefix}_INFORMATION_MAIN_BRIEF  "${__information_brief}"  PARENT_SCOPE)

    # Update version.
    tbag_information__parse_versions (__version_list "${__information_version}")
    tbag_information__read_version (__version_0 "${__version_list}" 0)
    tbag_information__read_version (__version_1 "${__version_list}" 1)
    tbag_information__read_version (__version_2 "${__version_list}" 2)
    tbag_information__read_version (__version_3 "${__version_list}" 3)
    tbag_information__read_version (__version_4 "${__version_list}" 4)
    tbag_information__read_version (__version_5 "${__version_list}" 5)
    set (${__prefix}_INFORMATION_VERSION_MAJOR         "${__version_0}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_VERSION_MINOR         "${__version_1}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_VERSION_PATCH         "${__version_2}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_VERSION_PACKET_MAJOR  "${__version_3}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_VERSION_PACKET_MINOR  "${__version_4}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_VERSION_RELEASE       "${__version_5}" PARENT_SCOPE)
endfunction ()

