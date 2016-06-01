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

set (GTEST_ROOT "${THIRD_PREFIX}")

## Configure settings.
tbag_config__generate_c_header ("${PROJECT_SOURCE_DIR}/${MAIN_NAME}/config.h")

include (TbagDoxygen)
tbag_doxygen_generate ()

## Run project.
tbag_project ()

tbag_module_install_headers (${MAIN_NAME})
tbag_preview ()

