## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

enable_language (C)
enable_language (CXX)

## Tbag startup script:
#set (TBAG_CMAKE_MODULE "$ENV{TPARTY_HOME}/local/share/libtbag/cmake" CACHE PATH "libtbag cmake module directory.")
#if (IS_DIRECTORY "${TBAG_CMAKE_MODULE}")
#    list (APPEND CMAKE_MODULE_PATH "${TBAG_CMAKE_MODULE}")
#else ()
#    message (FATAL_ERROR "Not found libtbag cmake directory.")
#endif ()
#include (TbagConfig)
#tbag_config ("${PROJECT_SOURCE_DIR}/INFORMATION")

## Tbag project.
list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include (TbagConfig)
tbag_config ("${PROJECT_SOURCE_DIR}/INFORMATION")

tbag_config__set_library_prefix (GTEST_ROOT)
tbag_config__set_library_prefix (UV_ROOT)
tbag_config__set_library_prefix (Spdlog_ROOT)
tbag_config__set_library_prefix (TinyXML2_ROOT)
tbag_config__set_library_prefix (NCurses_ROOT)
tbag_config__set_library_prefix (SQLite3_ROOT)
tbag_config__set_library_prefix (Doxygen_ROOT)

tbag_config__set_option (USE_DOXYGEN)

## Configure settings.
tbag_config__generate_c_header ("${PROJECT_SOURCE_DIR}/${MAIN_NAME}/config.h")

if (USE_DOXYGEN)
    find_package (TbagDoxygen)
    tbag_doxygen ()
endif ()

## Run project.
tbag_project ()

tbag_config__install_cxx_headers (${MAIN_NAME})
tbag_preview ()

