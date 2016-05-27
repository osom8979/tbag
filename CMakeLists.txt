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
include (TbagConfig)
include (TbagTparty)
include (TbagFlags)

tbag_config_init_main_information (
        libtbag zer0 osom8979@gmail.com
        "THIRD-PARTY EXTENSION UTILITY PROJECT")
tbag_config_init_version (
        0 0 0
        0 0
        0)
tbag_config_exists_main_information ()

tbag_flags__position_independent_code       ()
tbag_flags__cpp_standard_11                 ()
tbag_flags__add_debug_define                ()
tbag_flags__include_and_link_for_source_dir ()
tbag_flags__print_all_compile_warning       ()
#tbag_flags__install_rpath ()

tbag_tparty_init ()

# Extension variables.
set (ZLIB_ROOT     "${THIRD_PREFIX}")
set (GTEST_ROOT    "${THIRD_PREFIX}")
set (SPDLOG_ROOT   "${THIRD_PREFIX}")
set (UV_ROOT       "${THIRD_PREFIX}")
set (TINYXML2_ROOT "${THIRD_PREFIX}")
set (NCURSES_ROOT  "${THIRD_PREFIX}")
set (SQLITE3_ROOT  "${THIRD_PREFIX}")

# List of libraries.
tbag_config_add_library_option  (ZLIB      OFF)
tbag_config_add_library_option  (LMDB      OFF)
tbag_config_add_library_option  (GTest     ON)
tbag_config_add_library_option  (Spdlog    ON)
tbag_config_add_library_option  (UV        ON)
tbag_config_add_library_option  (TinyXML2  ON)
tbag_config_add_library_option  (NCurses   ON)
tbag_config_add_library_option  (SQLite3   ON)
tbag_config_add_library_option  (Doxygen   ON)

tbag_config_add_library_option  (Protobuf  OFF)
tbag_config_add_library_option  (GFlags    OFF)
tbag_config_add_library_option  (GLog      OFF)
tbag_config_add_library_option  (HDF5-2    OFF) # Don't use standard module. (FindHDF5.cmake)
tbag_config_add_library_option  (OpenBLAS  OFF)
tbag_config_add_library_option  (Snappy    OFF)
tbag_config_add_library_option  (LevelDB   OFF)
tbag_config_add_library_option  (PNG       OFF)
tbag_config_add_library_option  (wxWidgets OFF)
#tbag_config_add_library_option (Tbag      OFF)

# Build options.
option (BUILD_SHARED_LIBS "Create shared libraries if on." ON)

# Continuous Integration.
#tbag_find_package (DOXYGEN Doxygen)

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

