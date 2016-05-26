#/// @file   TbagCMakeConfig.cmake
#/// @brief  Default tbag cmake configure.
#/// @author zer0
#/// @date   2016-05-26

include (TbagCommon)

#/// Initialize & caching main information.
#///
#/// @param __name   [in] main project name.
#/// @param __author [in] project author.
#/// @param __email  [in] E-mail address.
#/// @param __brief  [in] Solution brief.
macro (tbag_config_init_main_information __name __author __email __brief)
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
macro (tbag_config_init_version __major __minor __patch __packet_major __packet_minor __release)
    # Main version.
    set (VERSION_MAJOR __major)
    set (VERSION_MINOR __minor)
    set (VERSION_PATCH __patch)

    # Tweak version.
    string (TIMESTAMP __timestamp "%Y-%m-%d_%H%M%S")
    set (VERSION_TWEAK  "${__timestamp}")
    unset (__timestamp)

    # Extension: Packet version.
    set (VERSION_PACKET_MAJOR __packet_major)
    set (VERSION_PACKET_MINOR __packet_minor)

    # Extension: release version number.
    set (VERSION_RELEASE __release)

    set (VERSION   "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
    set (SOVERSION "${VERSION_MAJOR}")
endmacro ()

#/// Exists main information.
macro (tbag_config_exists_main_information)
    # Main information.
    tbag_exists_define_or_die (MAIN_NAME)
    tbag_exists_define_or_die (MAIN_AUTHOR)
    tbag_exists_define_or_die (MAIN_EMAIL)
    tbag_exists_define_or_die (MAIN_BRIEF)

    # Main version.
    tbag_exists_define_or_die (VERSION_MAJOR)
    tbag_exists_define_or_die (VERSION_MINOR)
    tbag_exists_define_or_die (VERSION_PATCH)

    # Packet version.
    tbag_exists_define_or_die (VERSION_PACKET_MAJOR)
    tbag_exists_define_or_die (VERSION_PACKET_MINOR)

    # Extension version number.
    tbag_exists_define_or_die (VERSION_RELEASE)
endmacro ()

#/// Create & cacheing library option.
#///
#/// @remarks
#///  - USE_${_name}: library variable.
macro (tbag_config_add_library_option _name _enable)
    set (USE_${_name} ${_enable} CACHE BOOL "Use the ${_name} library.")
endmacro ()

