## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

enable_language (C)
enable_language (CXX)

# ---------------
# CMake settings.
# ---------------

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")

# User defined settings.
include (TbagCMakeConfig)
include (TbagCMakeTparty)

tbag_config_init_main_information (
        libtbag zer0 osom8979@gmail.com
        "THIRD-PARTY EXTENSION UTILITY PROJECT")
tbag_config_init_version (
        0 0 0
        0 0
        0)
tbag_tparty_init ()

# List of libraries.
tbag_config_add_library_option ("zlib"      OFF)
tbag_config_add_library_option ("gtest"     ON)
tbag_config_add_library_option ("protobuf"  OFF)
tbag_config_add_library_option ("spdlog"    ON)
tbag_config_add_library_option ("libuv"     ON)
tbag_config_add_library_option ("tinyxml2"  ON)
tbag_config_add_library_option ("ncurses"   ON)
tbag_config_add_library_option ("sqlite3"   ON)

# Continuous Integration.
option (USE_DOXYGEN "API documentation." ON)

option (BUILD_SHARED_LIBS "Create shared libraries if on." ON)

# CMake extension script.
include (TbagFindObject)
include (TbagProject)
include (TbagProjectBuild)
include (TbagFilesystem)
include (TbagInformation)
include (TbagCommon)
include (TbagStrings)
include (TbagCMakeConfig)
include (TbagFlags)

tbag_config_exists_main_information ()

list (INSERT CMAKE_PROGRAM_PATH 0 "${PROJECT_SOURCE_DIR}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${PROJECT_SOURCE_DIR}")

#set (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
#set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set  (CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")

# Extension variables.
set (ZLIB_ROOT     "${THIRD_PREFIX}")
set (GTEST_ROOT    "${THIRD_PREFIX}")
set (SPDLOG_ROOT   "${THIRD_PREFIX}")
set (UV_ROOT       "${THIRD_PREFIX}")
set (TINYXML2_ROOT "${THIRD_PREFIX}")
set (NCURSES_ROOT  "${THIRD_PREFIX}")
set (SQLITE3_ROOT  "${THIRD_PREFIX}")

# Test packages.
tbag_find_package  (USE_zlib      ZLIB)
tbag_find_package  (USE_gtest     GTest)
tbag_find_package  (USE_protobuf  Protobuf)
tbag_find_package  (USE_spdlog    Spdlog)
tbag_find_package  (USE_libuv     UV)
tbag_find_package  (USE_tinyxml2  TinyXML2)
tbag_find_package  (USE_ncurses   NCurses)
tbag_find_package  (USE_sqlite3   SQLite3)

# Continuous Integration.
tbag_find_package (DOXYGEN Doxygen)

## -------------------
## Configure settings.
## -------------------

set (TEMPLATE_PREFIX "${PROJECT_SOURCE_DIR}/template")

set (IN_CONFIG  "${TEMPLATE_PREFIX}/config.h.in")
set (OUT_CONFIG "${PROJECT_SOURCE_DIR}/${MAIN_NAME}/config.h")
configure_file (${IN_CONFIG} ${OUT_CONFIG} NEWLINE_STYLE UNIX)

set (IN_DOXYFILE  "${TEMPLATE_PREFIX}/Doxyfile.in")
set (OUT_DOXYFILE "${PROJECT_SOURCE_DIR}/Doxyfile")
configure_file (${IN_DOXYFILE} ${OUT_DOXYFILE} NEWLINE_STYLE UNIX)

## -----------
## Properties.
## -----------

tbag_flags__position_independent_code ()
tbag_flags__cpp_standard_11 ()
tbag_flags__add_debug_define ()
tbag_flags__include_and_link_for_source_dir ()
tbag_flags__print_all_compile_warning ()

# ------------
# Sub-project.
# ------------

find_project (_lib_proj_list _exe_proj_list "${PROJECT_SOURCE_DIR}")

message ("** Find library project: ${_lib_proj_list}")
message ("** Find executable project: ${_exe_proj_list}")

tbag_project_default_build_all ("${_lib_proj_list}" "${_exe_proj_list}" "${PROJECT_SOURCE_DIR}")

# -----------------------
# Continuous Integration.
# -----------------------

doxygen_generate ("${OUT_DOXYFILE}")

# -----------------
# Project settings.
# -----------------

install (DIRECTORY ${MAIN_NAME}
         DESTINATION "include"
         FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")

tbag_info__print_preview ()

