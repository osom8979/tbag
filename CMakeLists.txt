## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

enable_language (C)
enable_language (CXX)

## ---------------
## CMake settings.
## ---------------

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
include (TbagConfig)

tbag_config__init_main_information (
        libtbag zer0 osom8979@gmail.com
        "THIRD-PARTY EXTENSION UTILITY PROJECT")
tbag_config__init_version (
        0 0 0
        0 0
        0)

tbag_config__exists_main_information ()
tbag_config__caching_shared_library_option ()
tbag_flags__default ()
tbag_tparty__default ()

# Library extension variables.
set (GTEST_ROOT    "${THIRD_PREFIX}")
set (Spdlog_ROOT   "${THIRD_PREFIX}")
set (UV_ROOT       "${THIRD_PREFIX}")
set (TinyXML2_ROOT "${THIRD_PREFIX}")
set (NCurses_ROOT  "${THIRD_PREFIX}")
set (SQLite3_ROOT  "${THIRD_PREFIX}")

tbag_config__add_library_list (GTest UV Spdlog TinyXML2 NCurses SQLite3 Doxygen)

## -------------------
## Configure settings.
## -------------------

set (TEMPLATE_PREFIX "${PROJECT_SOURCE_DIR}/template")

set (IN_CONFIG  "${TEMPLATE_PREFIX}/config.h.in")
set (OUT_CONFIG "${PROJECT_SOURCE_DIR}/${MAIN_NAME}/config.h")
configure_file (${IN_CONFIG} ${OUT_CONFIG} NEWLINE_STYLE UNIX)

set (IN_DOXYFILE  "${TEMPLATE_PREFIX}/Doxyfile.in")
set (OUT_DOXYFILE "${PROJECT_BINARY_DIR}/Doxyfile")
configure_file (${IN_DOXYFILE} ${OUT_DOXYFILE} NEWLINE_STYLE UNIX)

set (IN_DOXY_INDEX  "${TEMPLATE_PREFIX}/doxygen.html.in")
set (OUT_DOXY_INDEX "${PROJECT_BINARY_DIR}/doxygen.html")
configure_file (${IN_DOXY_INDEX} ${OUT_DOXY_INDEX} COPYONLY)

## -----------------
## Project settings.
## -----------------

tbag_project__default ()

doxygen_generate ("${OUT_DOXYFILE}" "${PROJECT_BINARY_DIR}")
tbag_module_install__assign_library (${MAIN_NAME})
tbag_info__print_preview ()

