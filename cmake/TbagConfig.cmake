#/// @file   TbagConfig.cmake
#/// @brief  Tbag configure macros.
#/// @author zer0
#/// @date   2016-05-26

include (TbagUtils) # Use the TBAG_CMAKE_ROOT_DIR variable.
include (TbagFlags)
include (TbagProject)
include (TbagInformation)
include (TbagPreview)

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

#/// Default library path.
#///
#/// @param __value [in] value name.
macro (tbag_config__set_library_prefix __value)
    if (DEFINED THIRD_PREFIX)
        set (${__value} "${THIRD_PREFIX}")
    else ()
        set (${__value} "/usr/local")
    endif ()
    set (${__value} "${${__value}}" CACHE PATH "Library prefix directory.")
endmacro ()

#/// CMake option value.
#///
#/// @param __value [in] value name.
#///
#/// @remarks
#///  If enabled, add a definitions.
macro (tbag_config__set_option __value)
    if (${ARGC} GREATER 1)
        ## Error in CMake 2.8.12.2 (Ubuntu 14.04):
        ## - list (GET ${ARGN} 0 __flag)
        ## The 'foreach' statement of the following alternatives:
        foreach (__flag ${ARGN})
            set (__default_option ${__flag})
            break ()
        endforeach ()
    else ()
        set (__default_option ON)
    endif ()

    option (${__value} "${__value} on/off option value." ${__default_option})
    if (${__value})
        add_definitions (-D${__value})
    endif ()
endmacro ()

## ----------------------
## Libraries information.
## ----------------------

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

    list (INSERT CMAKE_PROGRAM_PATH 0 ${THIRD_BIN})
    list (INSERT CMAKE_INCLUDE_PATH 0 ${THIRD_INC})
    list (INSERT CMAKE_LIBRARY_PATH 0 ${THIRD_LIB})
endmacro ()

#/// Add SOURCE PATH's.
#///
#/// @remarks
#///  - ${CMAKE_PROGRAM_PATH}
#///  - ${CMAKE_INCLUDE_PATH}
#///  - ${CMAKE_LIBRARY_PATH}
macro (tbag_config__add_source_dir_paths)
    list (INSERT CMAKE_PROGRAM_PATH 0 "${PROJECT_SOURCE_DIR}")
    list (INSERT CMAKE_INCLUDE_PATH 0 "${PROJECT_SOURCE_DIR}")
    list (INSERT CMAKE_LIBRARY_PATH 0 "${PROJECT_SOURCE_DIR}")
endmacro ()

#/// Add BINARY PATH's.
#///
#/// @remarks
#///  - ${CMAKE_PROGRAM_PATH}
#///  - ${CMAKE_INCLUDE_PATH}
#///  - ${CMAKE_LIBRARY_PATH}
macro (tbag_config__add_binary_dir_paths)
    list (INSERT CMAKE_PROGRAM_PATH 0 "${PROJECT_BINARY_DIR}")
    list (INSERT CMAKE_INCLUDE_PATH 0 "${PROJECT_BINARY_DIR}")
    list (INSERT CMAKE_LIBRARY_PATH 0 "${PROJECT_BINARY_DIR}")
endmacro ()

#/// Setup include & link directories.
macro (tbag_config__set_include_and_link_directories)
    include_directories (${CMAKE_INCLUDE_PATH})
    link_directories (${CMAKE_LIBRARY_PATH})
endmacro ()

#/// Assign soversion property.
#///
#/// @param __target_name [in] Target name.
macro (tbag_config__soversion __target_name)
    set_target_properties (${__target_name} PROPERTIES
                           VERSION   "${VERSION}"
                           SOVERSION "${SOVERSION}")
endmacro ()

#/// Install C/C++ header files.
#///
#/// @param __parent_dir [in] parent of include directory.
macro (tbag_config__install_cxx_headers __parent_dir)
    install (DIRECTORY "${__parent_dir}"
             DESTINATION include
             FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
endmacro ()

#/// Find package with list.
#///
#/// @param ... [in] message arguments.
macro (tbag_config__find_packages)
    foreach (__package_cursor ${ARGN})
        find_package (${__package_cursor})
    endforeach ()
endmacro ()

#/// Find thread package.
#///
#/// @param ... [in] message arguments.
macro (tbag_config__find_thread)
    find_package (Threads)
    if (CMAKE_THREAD_LIBS_INIT)
        message (STATUS "Thread flags: ${CMAKE_THREAD_LIBS_INIT}")
    endif ()
endmacro ()

## -----------
## ALL IN ONE.
## -----------

#/// Read information file and initialize.
#///
#/// @param __path [in] Information file path.
macro (tbag_config __path)
    # Update CMAKE_MODULE_PATH
    tbag_utils__append_find_package_path ()
    tbag_utils__append_module_path ()

    # Read INFORMATION file.
    tbag_information__read_file (__tbag_config "${__path}")
    tbag_debug_variable (tbag_config  __tbag_config_INFORMATION_MAIN_NAME)
    tbag_debug_variable (tbag_config  __tbag_config_INFORMATION_MAIN_AUTHOR)
    tbag_debug_variable (tbag_config  __tbag_config_INFORMATION_MAIN_EMAIL)
    tbag_debug_variable (tbag_config  __tbag_config_INFORMATION_MAIN_BRIEF)
    tbag_debug_variable (tbag_config  __tbag_config_INFORMATION_VERSION_MAJOR)
    tbag_debug_variable (tbag_config  __tbag_config_INFORMATION_VERSION_MINOR)
    tbag_debug_variable (tbag_config  __tbag_config_INFORMATION_VERSION_PATCH)
    tbag_debug_variable (tbag_config  __tbag_config_INFORMATION_VERSION_PACKET_MAJOR)
    tbag_debug_variable (tbag_config  __tbag_config_INFORMATION_VERSION_PACKET_MINOR)
    tbag_debug_variable (tbag_config  __tbag_config_INFORMATION_VERSION_RELEASE)

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
            "${__tbag_config_INFORMATION_VERSION_RELEASE}")

    # Exists.
    tbag_config__exists_main_information    ()
    tbag_config__exists_version_information ()

    # Configure cmake settings.
    tbag_config__add_shared_library_option ()
    # tbag_config__add_third ("$ENV{TPARTY_HOME}/local") # Setup the THIRD_PREFIX variable.
    tbag_config__add_source_dir_paths ()
    tbag_config__add_binary_dir_paths ()
    tbag_config__set_include_and_link_directories ()

    # C/C++ flags.
    tbag_flags__default_setup ()
endmacro ()

## ----------
## Generator.
## ----------

#/// Write C/C++ header file.
#///
#/// @param __output_path [in] Output header file path.
function (tbag_config__generate_c_header __output_path)
    tbag_utils__write_configure (config.h.in "${__output_path}")
endfunction ()

macro (tbag_config__default_generate_c_header)
    tbag_utils__exists_define_or_die (MAIN_NAME)
    tbag_config__generate_c_header ("${PROJECT_SOURCE_DIR}/${MAIN_NAME}/config.h")
endmacro ()

#/// Write Doxygen config file.
#///
#/// @param __output_path [in] Output Doxyfile path.
function (tbag_config__generate_doxyfile __output_path)
    tbag_utils__write_configure (Doxyfile.in "${__output_path}")
endfunction ()

macro (tbag_config__default_generate_doxyfile)
    tbag_config__generate_doxyfile ("${PROJECT_BINARY_DIR}/Doxyfile")
endmacro ()

