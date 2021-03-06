#/// @file   TbagConfig.cmake
#/// @brief  Tbag configure macros.
#/// @author zer0
#/// @date   2016-05-26

include (TbagUtils) # Use the TBAG_CMAKE_ROOT_DIR variable.
include (TbagFlags)
include (TbagProject)
include (TbagInformation)
include (TbagPreview)

## -----------
## Environment
## -----------

#/// Build Configuration.
macro (tbag_config__check_build_configuration)
    set (TBAG_DEBUG          0)
    set (TBAG_RELEASE        0)
    set (TBAG_RELWITHDEBINFO 0)
    set (TBAG_MINSIZEREL     0)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set (TBAG_DEBUG 1)
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set (TBAG_RELEASE 1)
    elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set (TBAG_RELWITHDEBINFO 1)
    elseif (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
        set (TBAG_MINSIZEREL 1)
    endif ()
endmacro ()

#/// Check compiler.
macro (tbag_config__check_compiler)
    set (TBAG_GCC        0)
    set (TBAG_CLANG      0)
    set (TBAG_MSVC       0)
    set (TBAG_INTEL      0)
    set (TBAG_MSVC_INTEL 0)

    set (TBAG_MSVC_VERSION)

    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set (TBAG_CLANG 1)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Apple")
        set (TBAG_CLANG 1)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set (TBAG_GCC 1)
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Intel")
        if (MSVC)
            set (TBAG_MSVC_INTEL 1)
        else ()
            set (TBAG_INTEL 1)
        endif ()
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set (TBAG_MSVC 1)
        if (MSVC_VERSION EQUAL 1400)
            set (TBAG_MSVC_VERSION 8)
        elseif (MSVC_VERSION EQUAL 1500)
            set (TBAG_MSVC_VERSION 9)
        elseif (MSVC_VERSION EQUAL 1600)
            set (TBAG_MSVC_VERSION 10)
        elseif (MSVC_VERSION EQUAL 1700)
            set (TBAG_MSVC_VERSION 11)
        elseif (MSVC_VERSION EQUAL 1800)
            set (TBAG_MSVC_VERSION 12)
        elseif (MSVC_VERSION EQUAL 1900)
            set (TBAG_MSVC_VERSION 14)
        endif()
    else ()
        message (FATAL_ERROR "Compiler '${CMAKE_CXX_COMPILER_ID}' not recognized")
    endif ()
endmacro ()

#/// Check Platform.
macro (tbag_config__check_platform)
    set (TBAG_WINDOWS 0)
    set (TBAG_UNIX    0)
    set (TBAG_LINUX   0)
    set (TBAG_ANDROID 0)
    set (TBAG_FREEBSD 0)
    set (TBAG_OPENBSD 0)
    set (TBAG_IOS     0)
    set (TBAG_MACOSX  0)
    set (TBAG_CYGWIN  0)

    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set (TBAG_WINDOWS 1)
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set (TBAG_UNIX 1)
        if (ANDROID)
            set (TBAG_ANDROID 1)
        else ()
            set(TBAG_LINUX 1)
        endif ()
    elseif (CMAKE_SYSTEM_NAME MATCHES "^k?FreeBSD$")
        set (TBAG_FREEBSD 1)
    elseif (CMAKE_SYSTEM_NAME MATCHES "^OpenBSD$")
        set (TBAG_OPENBSD 1)
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        if (IOS)
            set (TBAG_IOS 1)
        else ()
            set (TBAG_MACOSX 1)
        endif ()
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
        set (TBAG_ANDROID 1)
    elseif (${CYGWIN})
        set (TBAG_CYGWIN 1)
    endif()
endmacro ()

#/// Check OpenGLES.
macro (tbag_config__check_opengles)
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux" AND ANDROID)
        set (TBAG_OPENGLES 1)
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
        set (TBAG_OPENGLES 1)
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" AND IOS)
        set (TBAG_OPENGLES 1)
    else ()
        set (TBAG_OPENGLES 0)
    endif ()
endmacro ()

#/// detect macOS version.
#/// use '/usr/bin/sw_vers -productVersion' to extract V from '10.V.x'.
macro (tbag_config__check_macos_version)
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" AND NOT IOS)
        exec_program (/usr/bin/sw_vers
                ARGS -productVersion
                OUTPUT_VARIABLE TBAG_MACOSX_VERSION)
        message (STATUS "Found macOS(Darwin) platform version: ${TBAG_MACOSX_VERSION}")
    else ()
        set (TBAG_MACOSX_VERSION)
        message (WARNING "It is not a macOS(Darwin) platform.")
    endif ()
endmacro ()

#/// Check CPU Architecture.
macro (tbag_config__check_cpu_architecture)
    set (TBAG_ARCH_32BITS 0)
    set (TBAG_ARCH_64BITS 0)

    include (CheckTypeSize)
    check_type_size (void* SIZEOF_VOID_PTR)

    if (${SIZEOF_VOID_PTR} STREQUAL "4")
        set (TBAG_ARCH_32BITS 1)
    elseif (${SIZEOF_VOID_PTR} STREQUAL "8")
        set (TBAG_ARCH_64BITS 1)
    endif()
endmacro ()

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
    set (${__value} "/usr/local") # DEFAULT SETTING.
    tbag_utils__get_args_size (__library_prefix_arg_length ${ARGN})

    if (${__library_prefix_arg_length} GREATER 0)
        foreach (__library_prefix_cursor ${ARGN})
            if (IS_DIRECTORY "${__library_prefix_cursor}")
                set (${__value} "${__library_prefix_cursor}")
                break ()
            endif ()
        endforeach ()
    elseif (DEFINED THIRD_PREFIX)
        set (${__value} "${THIRD_PREFIX}")
    endif ()

    tbag_debug_variable (tbag_config__set_library_prefix ${__value})
    set (${__value} "${${__value}}" CACHE PATH "Library prefix directory.")
endmacro ()

#/// CMake library option.
#///
#/// @param __value        [in] value name.
#/// @param __package_name [in] parameter of find_package.
#/// @param __found_value  [in] found value. (e.g. XXX_FOUND)
macro (tbag_config__set_library_option __value __package_name __found_value)
    if ((DEFINED ${__value} AND ${__value}) OR (NOT DEFINED ${__value}))
        find_package (${__package_name} QUIET)
        set (${__value} ${${__found_value}})
    else ()
        set (${__value} OFF)
    endif ()

    if (CMAKE_VERSION VERSION_LESS "3.12")
        option (${__value} "Use the ${__package_name} library if on." ${${__value}})
    else ()
        set (${__value} "${${__value}}" CACHE BOOL "Use the ${__package_name} library if on.")
    endif ()

    if (${__value})
        add_definitions (-D${__value})
    endif ()
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

    message (STATUS "THIRD_PREFIX: ${THIRD_PREFIX}")
    list (INSERT CMAKE_PROGRAM_PATH 0 ${THIRD_BIN})
    list (INSERT CMAKE_INCLUDE_PATH 0 ${THIRD_INC})
    list (INSERT CMAKE_LIBRARY_PATH 0 ${THIRD_LIB})
endmacro ()

#/// Initialize tparty variables.
macro (tbag_config__add_third_from_tparty)
    if ("$ENV{TPARTY_PREFIX}" STREQUAL "")
        string (REPLACE "\\" "/" __tparty_prefix "$ENV{TPARTY_PREFIX}")
    else ()
        string (REPLACE "\\" "/" __tparty_prefix "/usr/local/tparty")
    endif ()
    tbag_config__add_third ("${__tparty_prefix}")
    unset (__tparty_prefix)
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
    ## https://cmake.org/cmake/help/v3.2/module/FindThreads.html
    find_package (Threads)

    # CMAKE_THREAD_LIBS_INIT - the thread library

    if (CMAKE_USE_SPROC_INIT)
        set (TBAG_USE_SPROC 1)
    endif ()
    if (CMAKE_USE_WIN32_THREADS_INIT)
        set (TBAG_USE_WIN32_THREADS 1)
    endif ()
    if (CMAKE_USE_PTHREADS_INIT)
        set (TBAG_USE_PTHREADS 1)
    endif ()
    if (CMAKE_HP_PTHREADS_INIT)
        set (TBAG_USE_HP_PTHREADS 1)
    endif ()
endmacro ()

#/// Find thread package.
#///
#/// @param ... [in] message arguments.
macro (tbag_config__use_ccache)
    find_program (CCACHE_FOUND ccache)
    if (CCACHE_FOUND)
        message (STATUS "Found ccache: ${CCACHE_FOUND}")
        set_property (GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_FOUND})
        set_property (GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_FOUND})
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
    # tbag_config__add_third_from_tparty () # Setup the THIRD_PREFIX variable.
    tbag_config__add_source_dir_paths ()
    tbag_config__add_binary_dir_paths ()
    tbag_config__set_include_and_link_directories ()

    # C/C++ flags.
    tbag_flags__default_setup ()

    # Environment.
    tbag_config__check_build_configuration ()
    tbag_config__check_compiler ()
    tbag_config__check_platform ()
    tbag_config__check_opengles ()
    tbag_config__check_cpu_architecture ()
    if (TBAG_MACOSX)
        tbag_config__check_macos_version ()
    endif ()
endmacro ()

## ----------
## Generator.
## ----------

#/// Write Text to String C++11 header file.
#///
#/// @param __output_path [in] Output header file path.
#/// @param __name        [in] Variable name.
#/// @param __delimiter   [in] Delimiter of C++11 string literal.
#/// @param __text        [in] Text string.
function (tbag_config__generate_text_to_cpp11_string __output_path __name __delimiter __text)
    file (WRITE ${__output_path} ""
            "\n#ifndef __INCLUDE_${MAIN_NAME_UPPER}__${MAIN_NAME_UPPER}_${__name}_TEXT_TO_CPP11_STRING_H__"
            "\n#define __INCLUDE_${MAIN_NAME_UPPER}__${MAIN_NAME_UPPER}_${__name}_TEXT_TO_CPP11_STRING_H__"
            "\n"
            "\ninline char * char get_${__name}_text()"
            "\n{"
            "\n    return R\"${__delimiter}(${__text})${__delimiter}\""
            "\n}"
            "\n"
            "\n#endif /* __INCLUDE_${MAIN_NAME_UPPER}__${MAIN_NAME_UPPER}_${__name}_TEXT_TO_CPP11_STRING_H__ */"
            "\n")
endfunction ()

#/// Write C/C++ header file.
#///
#/// @param __output_path [in] Output header file path.
function (tbag_config__generate_c_header __output_path)
    file (WRITE ${__output_path} ""
        "\n#ifndef __INCLUDE_${MAIN_NAME_UPPER}__${MAIN_NAME_UPPER}_CONFIG_H__"
        "\n#define __INCLUDE_${MAIN_NAME_UPPER}__${MAIN_NAME_UPPER}_CONFIG_H__"
        "\n"
        "\n#define ${MAIN_NAME_UPPER}_TITLE_NAME        ${MAIN_NAME}"
        "\n#define ${MAIN_NAME_UPPER}_TITLE_NAME_UPPER  ${MAIN_NAME_UPPER}"
        "\n"
        "\n#define ${MAIN_NAME_UPPER}_VERSION_MAJOR  ${VERSION_MAJOR}"
        "\n#define ${MAIN_NAME_UPPER}_VERSION_MINOR  ${VERSION_MINOR}"
        "\n#define ${MAIN_NAME_UPPER}_VERSION_PATCH  ${VERSION_PATCH}"
        "\n#define ${MAIN_NAME_UPPER}_VERSION_TWEAK  \"${VERSION_TWEAK}\""
        "\n"
        "\n#define ${MAIN_NAME_UPPER}_VERSION_PACKET_MAJOR  ${VERSION_PACKET_MAJOR}"
        "\n#define ${MAIN_NAME_UPPER}_VERSION_PACKET_MINOR  ${VERSION_PACKET_MINOR}"
        "\n#define ${MAIN_NAME_UPPER}_VERSION_RELEASE       ${VERSION_RELEASE}"
        "\n"
        "\n#ifndef NAMESPACE_${MAIN_NAME_UPPER}"
        "\n#define NAMESPACE_${MAIN_NAME_UPPER} ${MAIN_NAME_UPPER}_TITLE_NAME"
        "\n# if defined(__cplusplus)"
        "\n#  define NAMESPACE_${MAIN_NAME_UPPER}_OPEN   namespace NAMESPACE_${MAIN_NAME_UPPER} {"
        "\n#  define NAMESPACE_${MAIN_NAME_UPPER}_CLOSE  }"
        "\n#  define USING_NAMESPACE_${MAIN_NAME_UPPER}  using namespace NAMESPACE_${MAIN_NAME_UPPER};"
        "\n# else /* __cplusplus */"
        "\n#  define NAMESPACE_${MAIN_NAME_UPPER}_OPEN"
        "\n#  define NAMESPACE_${MAIN_NAME_UPPER}_CLOSE"
        "\n#  define USING_NAMESPACE_${MAIN_NAME_UPPER}"
        "\n# endif /* __cplusplus */"
        "\n#endif"
        "\n"
        "\n#if defined(__cplusplus)"
        "\nNAMESPACE_${MAIN_NAME_UPPER}_OPEN"
        "\nNAMESPACE_${MAIN_NAME_UPPER}_CLOSE"
        "\n#endif /* __cplusplus */"
        "\n"
        "\n#ifndef __TO_STRING_IMP"
        "\n#define __TO_STRING_IMP(m) #m"
        "\n#endif"
        "\n"
        "\n#ifndef __TO_STRING"
        "\n#define __TO_STRING(x) __TO_STRING_IMP(x)"
        "\n#endif"
        "\n"
        "\n#define ${MAIN_NAME_UPPER}_VERSION_STRING \\"
        "\n        __TO_STRING(${MAIN_NAME_UPPER}_VERSION_MAJOR) \".\" \\"
        "\n        __TO_STRING(${MAIN_NAME_UPPER}_VERSION_MINOR) \".\" \\"
        "\n        __TO_STRING(${MAIN_NAME_UPPER}_VERSION_PATCH)"
        "\n"
        "\n#define ${MAIN_NAME_UPPER}_TITLE_STRING  __TO_STRING(${MAIN_NAME_UPPER}_TITLE_NAME)"
        "\n#define ${MAIN_NAME_UPPER}_TITLE_PREFIX  \"[\" ${MAIN_NAME_UPPER}_TITLE_STRING \"]\""
        "\n"
        "\n#define ${MAIN_NAME_UPPER}_MAIN_TITLE \\"
        "\n        ${MAIN_NAME_UPPER}_TITLE_STRING \" v\" ${MAIN_NAME_UPPER}_VERSION_STRING"
        "\n"
        "\n#endif /* __INCLUDE_${MAIN_NAME_UPPER}__${MAIN_NAME_UPPER}_CONFIG_H__ */"
        "\n")
endfunction ()

#/// Write Export API C header file.
#///
#/// @param __name        [in] Project name.
#/// @param __prefix      [in] Prefix of APIs.
#/// @param __output_path [in] Output header file path.
#/// @param __more_body   [in] More c header code.
function (tbag_config__generate_export_api_c_header __name __prefix __output_path __more_body)
    set (__prefix_export_api "${__prefix}_EXPORT_API")
    set (__prefix_api        "${__prefix}_API")

    file (WRITE ${__output_path} ""
            "\n#ifndef __INCLUDE_${__name}__${__prefix}_EXPORT_API_H__"
            "\n#define __INCLUDE_${__name}__${__prefix}_EXPORT_API_H__"
            "\n"
            "\n#if defined(__GNUC__)"
            "\n#define __${__prefix}_GNUC_FULLVERSION \\"
            "\n    ( (__GNUC__            * 10000) \\"
            "\n    + (__GNUC_MINOR__      *   100) \\"
            "\n    + (__GNUC_PATCHLEVEL__ *     1) )"
            "\n#else"
            "\n#define __${__prefix}_GNUC_FULLVERSION 0"
            "\n#endif"
            "\n"
            "\n#if defined(WIN32) || defined(_WIN32) || \\"
            "\n    defined(WIN64) || defined(_WIN64) || \\"
            "\n    defined(WINCE) || defined(_WINCE) || \\"
            "\n    defined(__TOS_WIN__) || \\"
            "\n    defined(__WIN32__) || defined(__TOS_WIN__) || \\"
            "\n    defined(__WINDOWS__)"
            "\n#define __${__prefix}_PLATFORM_WINDOWS"
            "\n#endif"
            "\n"
            "\n#if defined(${__prefix_export_api})"
            "\n# if defined(__${__prefix}_PLATFORM_WINDOWS)"
            "\n#  define ${__prefix_api} __declspec(dllexport)"
            "\n# elif __${__prefix}_GNUC_FULLVERSION >= 40000"
            "\n#  define ${__prefix_api} __attribute__((visibility(\"default\")))"
            "\n# else"
            "\n#  define ${__prefix_api}"
            "\n# endif"
            "\n#else /* defined(${__prefix_export_api}) */"
            "\n# if defined(__${__prefix}_PLATFORM_WINDOWS)"
            "\n#  define ${__prefix_api} __declspec(dllimport)"
            "\n# else"
            "\n#  define ${__prefix_api}"
            "\n# endif"
            "\n#endif /* defined(${__prefix_export_api}) */"
            "\n"
            "\n#if defined(__cplusplus)"
            "\nextern \"C\" {"
            "\n#endif"
            "\n"
            "\n${__more_body}"
            "\n"
            "\n#if defined(__cplusplus)"
            "\n} /* extern \"C\" */"
            "\n#endif"
            "\n"
            "\n#endif /* __INCLUDE_${__name}__${__prefix}_EXPORT_API_H__ */"
            "\n")

    #message (STATUS "[${__name}/${__prefix}] Export API macro: ${__prefix_export_api}")
    #message (STATUS "[${__name}/${__prefix}] API macro: ${__prefix_api}")
endfunction ()

macro (tbag_config__default_generate_c_header)
    tbag_utils__exists_define_or_die (MAIN_NAME)
    tbag_config__generate_c_header ("${PROJECT_SOURCE_DIR}/${MAIN_NAME}/config.h")
endmacro ()

#/// Write Doxygen config file.
#///
#/// @param __output_path [in] Output Doxyfile path.
function (tbag_config__generate_doxyfile __output_path)
    file (WRITE ${__output_path} ""
        "\nDOXYFILE_ENCODING     = UTF-8"
        "\nPROJECT_NAME          = \"${MAIN_NAME}\""
        "\nPROJECT_NUMBER        = \"${VERSION}\""
        "\nPROJECT_BRIEF         = \"${MAIN_BRIEF}\""
        "\nOUTPUT_DIRECTORY      = \"${PROJECT_BINARY_DIR}\""
        "\nOUTPUT_LANGUAGE       = Korean-en"
        "\nALIASES               = translate{2}=\"<dl class='section remarks'><dt>Translate-\\1</dt><dd>\\2</dd></dl>\""
        "\nMARKDOWN_SUPPORT      = NO"
        "\nWARN_IF_UNDOCUMENTED  = NO"
        "\nINPUT                 = \"${PROJECT_SOURCE_DIR}\""
        "\nFILE_PATTERNS         = *.c *.cc *.cxx *.cpp *.c++ \\"
        "\n                        *.inl *.h *.hh *.hxx *.hpp *.h++ *.idl *.odl *.inc"
        "\nRECURSIVE             = YES"
        "\nEXCLUDE_PATTERNS      = *build/* *dep/* *tmp/* *temp/*"
        "\nHTML_TIMESTAMP        = YES"
        "\nHTML_OUTPUT           = doxygen"
        "\nGENERATE_LATEX        = NO"
        "\nLATEX_OUTPUT          = latex"
        "\nMACRO_EXPANSION       = YES"
        "\nINCLUDE_PATH          = \"${PROJECT_SOURCE_DIR}\""
        "\nINCLUDE_FILE_PATTERNS = *.h *.hh *.hxx *.hpp *.h++"
        "\nUSE_MATHJAX           = YES"
        "\nMATHJAX_RELPATH       = https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"
        "\nQUIET                 = YES"
        "\n")
endfunction ()

macro (tbag_config__default_generate_doxyfile)
    tbag_config__generate_doxyfile ("${PROJECT_BINARY_DIR}/Doxyfile")
endmacro ()

