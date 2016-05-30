#/// @file   TbagConfig.cmake
#/// @brief  Tbag configure macros.
#/// @author zer0
#/// @date   2016-05-26

include (TbagDebug)
include (TbagUtils)
include (TbagConfigFile)
include (TbagStrings)
include (TbagInformation)
include (TbagCxxFlags)
include (TbagTparty)
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

## -----------
## ALL IN ONE.
## -----------

#/// Read information file and initialize.
#///
#/// @param __path [in] Information file path.
macro (tbag_config __path)
    tbag_config_file__read (__tbag_config "${__path}")
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
    tbag_tparty__default () # Setup the THIRD_PREFIX variable.

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

