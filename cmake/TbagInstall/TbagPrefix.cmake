## CMake policy settings.
if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()
#if (POLICY CMP0026)
#    # Disallow use of the LOCATION target property.
#    cmake_policy (SET CMP0026 NEW)
#endif ()

enable_language (C)
enable_language (CXX)

## Find tbag-cmake-module.
if (NOT TBAG_MODULE_PATH)
    message (STATUS "Looking for tbag-cmake-module.")
    find_path (TBAG_MODULE_PATH
            NAMES "TbagConfig.cmake"
            PATHS "$ENV{TBAG_HOME}/cmake"
            "/usr/local/share/libtbag/cmake")
endif ()

set (TBAG_MODULE_PATH "${TBAG_MODULE_PATH}" CACHE PATH "libtbag cmake module directory.")
if (IS_DIRECTORY "${TBAG_MODULE_PATH}")
    message (STATUS "Found tabg-cmake-module: ${TBAG_MODULE_PATH}")
else ()
    message (FATAL_ERROR "Not found tbag-cmake-module.")
endif ()

## Default tbag project.
if (IS_DIRECTORY "${TBAG_MODULE_PATH}")
    list (APPEND CMAKE_MODULE_PATH "${TBAG_MODULE_PATH}")
else ()
    message (FATAL_ERROR "Not found libtbag cmake module directory.")
endif ()

## Default tbag project.
include (TbagConfig)
tbag_config ("${PROJECT_SOURCE_DIR}/INFORMATION")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819") ## code page warning.
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4595")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996") ## declared deprecated warning.
    add_definitions (_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
endif ()

tbag_config__use_ccache ()

## Configure settings.
tbag_config__default_generate_c_header ()
tbag_config__generate_export_api_c_header (C2CORE  C2CORE   "${CMAKE_SOURCE_DIR}/libc2core/export.h"  "/* EMPTY. */")
tbag_config__generate_export_api_c_header (C2IN    C2IN     "${CMAKE_SOURCE_DIR}/libc2in/export.h"    "/* EMPTY. */")
tbag_config__generate_export_api_c_header (C2OUT   C2OUT    "${CMAKE_SOURCE_DIR}/libc2out/export.h"   "/* EMPTY. */")

## Library subproject.
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/gtest)

## Thread.
tbag_config__find_thread ()

## Tbag.
tbag_config__set_library_option (USE_TBAG Tbag Tbag_FOUND)
tbag_config__find_packages (Tbag)

## Doxygen settings.
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
