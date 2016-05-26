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

# -------------------
# Configure settings.
# -------------------

set (TEMPLATE_PREFIX "${PROJECT_SOURCE_DIR}/template")
set (MAIN_DIR "${PROJECT_SOURCE_DIR}/${MAIN_NAME}")

set (IN_CONFIG    "${TEMPLATE_PREFIX}/config.h.in")
set (IN_DOXYFILE  "${TEMPLATE_PREFIX}/Doxyfile.in")

set (OUT_CONFIG   "${MAIN_DIR}/config.h")
set (OUT_DOXYFILE "${PROJECT_SOURCE_DIR}/Doxyfile")

configure_file (${IN_CONFIG}   ${OUT_CONFIG}   NEWLINE_STYLE UNIX)
configure_file (${IN_DOXYFILE} ${OUT_DOXYFILE} NEWLINE_STYLE UNIX)

# -----------
# Properties.
# -----------

#set (BUILD_SHARED_LIBS        ON) # User's variable.
#set (CMAKE_MACOSX_RPATH        1) # CMP0042
#set (CMAKE_BUILD_TYPE    Release) # User's variable.

# Position independent code settings.
if (CMAKE_VERSION VERSION_LESS "3.0.2")
    set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
else ()
    set (CMAKE_POSITION_INDEPENDENT_CODE  ON) # -fPIC
endif ()

# C++11 settings.
if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CUDA_FOUND)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
    else ()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif ()
    if (CUDA_FOUND)
        set (CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++11")
    endif ()
else ()
    set (CMAKE_CXX_EXTENSIONS  OFF) # Change: -std=gnu++11 -> -std=c++11
    set (CMAKE_CXX_STANDARD     11) # C++ standard 11
endif ()

set  (CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} -Wall")
#set (CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}")
#set (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
#set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

# Build type: Debug, Release, RelWithDebInfo, MinSizeRel
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions (-DDEBUG)
else ()
    add_definitions (-DNDEBUG -DRELEASE)
endif ()

include_directories (${PROJECT_SOURCE_DIR})
link_directories    (${PROJECT_SOURCE_DIR})

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

