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

#enable_language (C)
#enable_language (CXX)

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

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819") ## code page warning.
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4595")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996") ## declared deprecated warning.
    add_definitions (_CRT_SECURE_NO_WARNINGS)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-delete-non-virtual-dtor")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
endif ()

tbag_config__use_ccache ()
tbag_config__find_thread ()

