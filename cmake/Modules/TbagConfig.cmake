#/// @file   TbagConfig.cmake
#/// @brief  Tbag configure macros.
#/// @author zer0
#/// @date   2016-05-26

include (TbagDebug)
include (TbagUtils)

include (TbagStrings)
include (TbagInformation)
include (TbagCxxFlags)
include (TbagProject)

include (TbagCxxConfigGenerator)
include (TbagDoxygenGenerator)
include (ProtobufGenerator)

## -----------------
## Main information.
## -----------------

#/// Initialize & caching main information.
#///
#/// @param __name   [in] main project name.
#/// @param __author [in] project author.
#/// @param __email  [in] E-mail address.
#/// @param __brief  [in] Solution brief.
macro (tbag_config__init_main_information __name __author __email __brief)
    set (MAIN_NAME   "${__name}"   CACHE STRING  "Solution name string.")
    set (MAIN_AUTHOR "${__author}" CACHE STRING  "Solution author.")
    set (MAIN_EMAIL  "${__email}"  CACHE STRING  "Author E-mail address.")
    set (MAIN_BRIEF  "${__brief}"  CACHE STRING  "Solution brief string.")

    string (TOUPPER ${MAIN_NAME} MAIN_NAME_UPPER)
    string (TOLOWER ${MAIN_NAME} MAIN_NAME_LOWER)
endmacro ()

#/// Initialize version information.
#/// @param __major         [in] major version (0 <  X <= 65535)
#/// @param __minor         [in] minor version (0 <= Y <=   255)
#/// @param __patch         [in] patch version (0 <= Z <=   255)
#/// @param __packet_major  [in] Packet major version.
#/// @param __packet_minor  [in] Packet minor version.
#/// @param __release       [in] Release number.
macro (tbag_config__init_version_information __major __minor __patch __packet_major __packet_minor __release)
    # Main version.
    set (VERSION_MAJOR ${__major})
    set (VERSION_MINOR ${__minor})
    set (VERSION_PATCH ${__patch})

    # Tweak version.
    string (TIMESTAMP __timestamp "%Y-%m-%d_%H%M%S")
    set (VERSION_TWEAK  "${__timestamp}")
    unset (__timestamp)

    # Extension: Packet version.
    set (VERSION_PACKET_MAJOR ${__packet_major})
    set (VERSION_PACKET_MINOR ${__packet_minor})

    # Extension: release version number.
    set (VERSION_RELEASE ${__release})

    mark_as_advanced (VERSION_MAJOR VERSION_MINOR VERSION_PATCH
                      VERSION_PACKET_MAJOR VERSION_PACKET_MINOR
                      VERSION_RELEASE)

    set (VERSION   "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
    set (SOVERSION "${VERSION_MAJOR}")
endmacro ()

#/// Exists main information.
macro (tbag_config__exists_main_information)
    tbag_utils__exists_define_or_die (MAIN_NAME)
    tbag_utils__exists_define_or_die (MAIN_AUTHOR)
    tbag_utils__exists_define_or_die (MAIN_EMAIL)
    tbag_utils__exists_define_or_die (MAIN_BRIEF)
endmacro ()

#/// Exists version information.
macro (tbag_config__exists_version_information)
    tbag_utils__exists_define_or_die (VERSION_MAJOR)
    tbag_utils__exists_define_or_die (VERSION_MINOR)
    tbag_utils__exists_define_or_die (VERSION_PATCH)
    tbag_utils__exists_define_or_die (VERSION_PACKET_MAJOR)
    tbag_utils__exists_define_or_die (VERSION_PACKET_MINOR)
    tbag_utils__exists_define_or_die (VERSION_RELEASE)
endmacro ()

## --------------
## CMake options.
## --------------

#/// The caching shared library options.
macro (tbag_config__add_shared_library_option)
    option (BUILD_SHARED_LIBS "Create shared libraries if on." ON)
endmacro ()

## ----------------------
## Libraries information.
## ----------------------

#/// Create & cacheing library option.
#///
#/// @remarks
#///  - USE_${_name}: library variable.
macro (tbag_config__add_library_option __name __enable)
    set (USE_${__name} ${__enable} CACHE BOOL "Use the ${__name} library.")
endmacro ()

#/// Create & cacheing library list.
#///
#/// @param __default_root  [in] Default root directory of libraries.
#/// @param ...             [in] list of library name.
macro (tbag_config__add_libraries __default_root)
    foreach (__list_cursor ${ARGN})
        tbag_debug (tbag_config__add_libraries "Add ${__list_cursor} library.")

        set ("${__list_cursor}_ROOT" "${__default_root}" CACHE PATH "${__list_cursor} library root directory.")
        tbag_config__add_library_option (${__list_cursor} ON)
    endforeach ()
endmacro ()

#/// Initialize 3rd-party library variables.
#///
#/// @param __root_dir [in] Root directory of libraries.
macro (tbag_config__add_third __root_dir)
    set (THIRD_PREFIX  "${__root_dir}"           CACHE PATH "3rd-party prefix directory.")
    set (THIRD_BIN     "${THIRD_PREFIX}/bin"     CACHE PATH "3rd-party binary directory.")
    set (THIRD_INC     "${THIRD_PREFIX}/include" CACHE PATH "3rd-party include directory.")
    set (THIRD_LIB     "${THIRD_PREFIX}/lib"     CACHE PATH "3rd-party library directory.")

    if (IS_DIRECTORY ${THIRD_BIN})
        set (ENV{PATH} "${THIRD_BIN}${TBAG_PATH_SEPARATOR}$ENV{PATH}")
    endif ()
    if (IS_DIRECTORY ${THIRD_INC})
        set (ENV{CPATH} "${THIRD_INC}${TBAG_PATH_SEPARATOR}$ENV{CPATH}")
    endif ()
    if (IS_DIRECTORY ${THIRD_LIB})
        set (ENV{LIBRARY_PATH} "${THIRD_LIB}${TBAG_PATH_SEPARATOR}$ENV{LIBRARY_PATH}")
    endif ()

    list (INSERT CMAKE_PROGRAM_PATH 0 "${THIRD_BIN}")
    list (INSERT CMAKE_LIBRARY_PATH 0 "${THIRD_LIB}")

    include_directories (${THIRD_INC})
    link_directories (${THIRD_LIB})
endmacro ()

## ------------------------
## INFORMATION File reader.
## ------------------------

#/// Read variable.
#///
#/// @param __result  [out] value name of output result.
#/// @param __key     [in]  Key name.
#/// @param __content [in]  Content string.
#///
#/// @remarks
#///  Example content format: <code>KEY=VALUE</code>
function (tbag_config__read_value __result __key __content)
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
    tbag_strings__remove_quoting (${__result} "${__strip_variable}")

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
function (tbag_config__parse_libs __result __content)
    set (${__result})

    string (REGEX MATCHALL "[a-zA-Z0-9]+" ${__result} "${__content}")

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
function (tbag_config__parse_versions __result __content)
    set (${__result})

    string (REGEX MATCHALL "[0-9]+" ${__result} "${__content}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Read version from version-list.
#///
#/// @param __result       [out] value name of output result.
#/// @param __version_list [in]  Version list.
#/// @param __index        [in]  Index of version list.
function (tbag_config__read_version __result __version_list __index)
    set (${__result})

    list (LENGTH "${__version_list}" __version_list_length)
    if (__version_list_length GREATER ${__index})
        list (GET ${__version_list} ${__index} ${__result})
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
#///  - ${__prefix}_INFORMATION_LIBRARIES
function (tbag_config__read_information_file __prefix __path)
    if (NOT EXISTS "${__path}")
        message (FATAL_ERROR "Not found ${__path}")
    endif ()

    file (READ "${__path}" __information_content)

    tbag_config__read_value (__information_name     NAME    ${__information_content})
    tbag_config__read_value (__information_author   AUTHOR  ${__information_content})
    tbag_config__read_value (__information_email    EMAIL   ${__information_content})
    tbag_config__read_value (__information_brief    BRIEF   ${__information_content})
    tbag_config__read_value (__information_version  VERSION ${__information_content})
    tbag_config__read_value (__information_libs     LIBS    ${__information_content})

    tbag_debug (tbag_config__read_information_file "NAME: ${__information_name}")
    tbag_debug (tbag_config__read_information_file "AUTHOR: ${__information_author}")
    tbag_debug (tbag_config__read_information_file "EMAIL: ${__information_email}")
    tbag_debug (tbag_config__read_information_file "BRIEF: ${__information_brief}")
    tbag_debug (tbag_config__read_information_file "VERSION: ${__information_version}")
    tbag_debug (tbag_config__read_information_file "LIBS: ${__information_libs}")

    tbag_config__parse_versions (__version_list "${__information_version}")
    tbag_config__parse_libs     (__lib_list     "${__information_libs}")

    # Update main information.
    set (${__prefix}_INFORMATION_MAIN_NAME   "${__information_name}"   PARENT_SCOPE)
    set (${__prefix}_INFORMATION_MAIN_AUTHOR "${__information_author}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_MAIN_EMAIL  "${__information_email}"  PARENT_SCOPE)
    set (${__prefix}_INFORMATION_MAIN_BRIEF  "${__information_brief}"  PARENT_SCOPE)

    # Update version.
    tbag_config__read_version (__version_0 "${__version_list}" 0)
    tbag_config__read_version (__version_1 "${__version_list}" 1)
    tbag_config__read_version (__version_2 "${__version_list}" 2)
    tbag_config__read_version (__version_3 "${__version_list}" 3)
    tbag_config__read_version (__version_4 "${__version_list}" 4)
    tbag_config__read_version (__version_5 "${__version_list}" 5)
    set (${__prefix}_INFORMATION_VERSION_MAJOR         "${__version_0}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_VERSION_MINOR         "${__version_1}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_VERSION_PATCH         "${__version_2}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_VERSION_PACKET_MAJOR  "${__version_3}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_VERSION_PACKET_MINOR  "${__version_4}" PARENT_SCOPE)
    set (${__prefix}_INFORMATION_VERSION_RELEASE       "${__version_5}" PARENT_SCOPE)

    # Update libraries.
    set (${__prefix}_INFORMATION_LIBRARIES "${__lib_list}" PARENT_SCOPE)
endfunction ()

## -----------
## ALL IN ONE.
## -----------

#/// Read information file and initialize.
#///
#/// @param __path [in] Information file path.
macro (tbag_config __path)
    tbag_config__read_information_file (__tbag_config "${__path}")
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_MAIN_NAME)
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_MAIN_AUTHOR)
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_MAIN_EMAIL)
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_MAIN_BRIEF)
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_VERSION_MAJOR)
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_VERSION_MINOR)
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_VERSION_PATCH)
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_VERSION_PACKET_MAJOR)
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_VERSION_PACKET_MINOR)
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_VERSION_RELEASE)
    tbag_debug__variable (tbag_config  __tbag_config_INFORMATION_LIBRARIES)

    # Main information.
    tbag_config__init_main_information (
            "${__tbag_config_INFORMATION_MAIN_NAME}"
            "${__tbag_config_INFORMATION_MAIN_AUTHOR}"
            "${__tbag_config_INFORMATION_MAIN_EMAIL}"
            "${__tbag_config_INFORMATION_MAIN_BRIEF}")
    tbag_config__init_version_information (
            "${__tbag_config_INFORMATION_VERSION_MAJOR}"
            "${__tbag_config_INFORMATION_VERSION_MINOR}"
            "${__tbag_config_INFORMATION_VERSION_PATCH}"
            "${__tbag_config_INFORMATION_VERSION_PACKET_MAJOR}"
            "${__tbag_config_INFORMATION_VERSION_PACKET_MINOR}"
            "${__tbag_config_INFORMATION_VERSION_RELEASE}"
            "${__tbag_config_INFORMATION_LIBRARIES}")

    # Exists.
    tbag_config__exists_main_information    ()
    tbag_config__exists_version_information ()

    # Options.
    tbag_config__add_shared_library_option ()

    # Default settings.
    tbag_flags__default  ()
    tbag_config__add_third ("$ENV{TPARTY_HOME}/local") # Setup the THIRD_PREFIX variable.

    # Libraryies.
    tbag_utils__exists_define_or_die (THIRD_PREFIX)
    tbag_config__add_libraries ("${THIRD_PREFIX}" ${__tbag_config_INFORMATION_LIBRARIES})

    unset (__tbag_config_INFORMATION_MAIN_NAME)
    unset (__tbag_config_INFORMATION_MAIN_AUTHOR)
    unset (__tbag_config_INFORMATION_MAIN_EMAIL)
    unset (__tbag_config_INFORMATION_MAIN_BRIEF)
    unset (__tbag_config_INFORMATION_VERSION_MAJOR)
    unset (__tbag_config_INFORMATION_VERSION_MINOR)
    unset (__tbag_config_INFORMATION_VERSION_PATCH)
    unset (__tbag_config_INFORMATION_VERSION_PACKET_MAJOR)
    unset (__tbag_config_INFORMATION_VERSION_PACKET_MINOR)
    unset (__tbag_config_INFORMATION_VERSION_RELEASE)
    unset (__tbag_config_INFORMATION_LIBRARIES)
endmacro ()

