## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

enable_language (C)
enable_language (CXX)

## Tbag project.
list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
include (TbagConfig)
tbag_config ("${PROJECT_SOURCE_DIR}/INFORMATION")

set (GTEST_ROOT "${THIRD_PREFIX}")

## Configure settings.
tbag_config__generate_c_header ("${PROJECT_SOURCE_DIR}/${MAIN_NAME}/config.h")
tbag_doxygen_generate__auto_default ()

## Run project.
tbag_project ()

tbag_module_install__assign_library (${MAIN_NAME})
tbag_info_print_preview ()

