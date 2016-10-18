## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
#if (POLICY CMP0054)
#    cmake_policy (SET CMP0054 NEW)
#endif ()
#if (POLICY CMP0026)
#    # Disallow use of the LOCATION target property.
#    cmake_policy (SET CMP0026 NEW)
#endif ()

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

if (WIN32 AND MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4595")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996") ## declared deprecated warning.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Setup the THIRD_PREFIX variable.
tbag_config__add_third ("$ENV{TPARTY_HOME}/local")

## Configure settings.
tbag_config__default_generate_c_header ()

## Sub-project.
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/lua)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/uv)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/icu)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/gtest)

## Thread settings.
tbag_config__find_thread ()

## SFML settings.
tbag_config__set_library_option (USE_SFML TbagSFML SFML_FOUND)

## Doxygen settings.
tbag_config__set_library_prefix (Doxygen_ROOT)
tbag_config__set_library_option (USE_DOXYGEN Doxygen DOXYGEN_FOUND)

if (USE_DOXYGEN)
    find_package (TbagDoxygen)
    if (DOXYGEN_FOUND)
        set (DOXYFILE_INPUT   "${PROJECT_SOURCE_DIR}/template/Doxyfile.in")
        set (DOXYFILE_OUTPUT  "${PROJECT_BINARY_DIR}/Doxyfile")

        configure_file ("${DOXYFILE_INPUT}" "${DOXYFILE_OUTPUT}")
        tbag_doxygen__generate_index ("${PROJECT_BINARY_DIR}/doxygen.html")
        tbag_doxygen__target ("${DOXYFILE_OUTPUT}" "${PROJECT_BINARY_DIR}")
    endif ()
endif ()

## Header checker.
include (CheckIncludeFile)
## - ${CMAKE_REQUIRED_FLAGS}: string of compile command line flags
## - ${CMAKE_REQUIRED_DEFINITIONS}: list of macros to define (-DFOO=bar)
## - ${CMAKE_REQUIRED_INCLUDES}: list of include directories
check_include_file (sys/syscall.h  HAVE_SYS_SYSCALL_H)
check_include_file (syscall.h      HAVE_SYSCALL_H)

set (CONFIG_EX_INPUT  "${PROJECT_SOURCE_DIR}/template/config-ex.h.in")
set (CONFIG_EX_OUTPUT "${PROJECT_SOURCE_DIR}/${MAIN_NAME}/config-ex.h")
configure_file ("${CONFIG_EX_INPUT}" "${CONFIG_EX_OUTPUT}")

## Run project.
tbag_project ()

## Install & external settings.
tbag_config__install_cxx_headers (${MAIN_NAME})
tbag_config__soversion (tbag)

## libtbag-cmake build packages.
install (DIRECTORY "${PROJECT_SOURCE_DIR}/cmake"
         DESTINATION "share/libtbag")

## Print result messages.
tbag_preview ()

