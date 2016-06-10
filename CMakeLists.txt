## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

enable_language (C)
enable_language (CXX)

## Find tbag module path.
set (TBAG_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake"                 CACHE PATH "libtbag cmake module directory.")
#set(TBAG_MODULE_PATH "$ENV{TPARTY_HOME}/local/share/libtbag/cmake" CACHE PATH "libtbag cmake module directory.")

## Default tbag project.
if (IS_DIRECTORY "${TBAG_MODULE_PATH}")
    list (APPEND CMAKE_MODULE_PATH "${TBAG_MODULE_PATH}")
else ()
    message (FATAL_ERROR "Not found libtbag cmake module directory.")
endif ()
include (TbagConfig)
tbag_config ("${PROJECT_SOURCE_DIR}/INFORMATION")

## Find packages.
tbag_config__set_library_prefix (GTEST_ROOT)
tbag_config__set_library_prefix (UV_ROOT)
tbag_config__set_library_prefix (Spdlog_ROOT)
tbag_config__set_library_prefix (TinyXML2_ROOT)
tbag_config__set_library_prefix (NCurses_ROOT)
tbag_config__set_library_prefix (SQLite3_ROOT)
tbag_config__set_library_prefix (Doxygen_ROOT)

tbag_config__set_option (USE_DOXYGEN)
tbag_config__set_option (USE_NCURSES)

tbag_config__find_packages (GTest UV Spdlog TinyXML2 NCurses SQLite3 TbagDoxygen)

## Configure settings.
tbag_config__default_generate_c_header ()
tbag_config__default_generate_doxyfile ()

## Run project.
tbag_project ()

## Install & external settings.
tbag_config__install_cxx_headers (${MAIN_NAME})

if (USE_DOXYGEN)
    tbag_doxygen ()
endif ()

## Print result messages.
tbag_preview ()

