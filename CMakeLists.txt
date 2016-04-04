## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

enable_language (C)
enable_language (CXX)

# ---------------
# CMake settings.
# ---------------

set (_CMAKE_BUILD_TYPE_DEBUG           "Debug")
set (_CMAKE_BUILD_TYPE_RELEASE         "Release")
set (_CMAKE_BUILD_TYPE_RELWITHDEBINFO  "RelWithDebInfo")
set (_CMAKE_BUILD_TYPE_MINSIZEREL      "MinSizeRel")

set (CMAKE_MACOSX_RPATH  1) # CMP0042

#set (CMAKE_VERBOSE_MAKEFILE ON)
 set (BUILD_SHARED_LIBS      ON)

#set (CMAKE_BUILD_TYPE "Debug")
#set (CMAKE_BUILD_TYPE "Release")

 set (CMAKE_CXX_FLAGS           "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
#set (CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}")
#set (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
#set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

# User defined settings.
include (config.cmake)

# CMake extension script.
include (cmake/compile.cmake)
include (cmake/build.cmake)
include (cmake/util.cmake)

exists_define_or_die (MAIN_NAME)

string (TOUPPER ${MAIN_NAME} MAIN_NAME_UPPER)
string (TIMESTAMP _timestamp "%Y-%m-%d_%H%M%S")
set (VERSION_TWEAK "${_timestamp}")
set (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

# Third-party libraries.
set (THIRD_BIN "${THIRD_PREFIX}/bin")
set (THIRD_INC "${THIRD_PREFIX}/include")
set (THIRD_LIB "${THIRD_PREFIX}/lib")

if (IS_DIRECTORY ${THIRD_BIN})
    set (ENV{PATH} "${THIRD_BIN}${PATH_SEPARATOR}$ENV{PATH}")
endif ()
if (IS_DIRECTORY ${THIRD_INC})
    set (ENV{CPATH} "${THIRD_INC}${PATH_SEPARATOR}$ENV{CPATH}")
endif ()
if (IS_DIRECTORY ${THIRD_LIB})
    set (ENV{LIBRARY_PATH} "${THIRD_LIB}${PATH_SEPARATOR}$ENV{LIBRARY_PATH}")
endif ()

list (INSERT CMAKE_PROGRAM_PATH 0 "${THIRD_BIN}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${THIRD_LIB}")
list (INSERT CMAKE_PROGRAM_PATH 0 "${PROJECT_SOURCE_DIR}")
list (INSERT CMAKE_LIBRARY_PATH 0 "${PROJECT_SOURCE_DIR}")

set (CMAKE_INSTALL_RPATH "." "${THIRD_LIB}")
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Extension variables.
set (ZLIB_ROOT     "${THIRD_PREFIX}")
set (GTEST_ROOT    "${THIRD_PREFIX}")
set (UV_ROOT       "${THIRD_PREFIX}")
set (TINYXML2_ROOT "${THIRD_PREFIX}")

# Test packages.
test_find_package  (zlib      ZLIB)
test_find_package  (gtest     GTest)
test_find_package  (protobuf  Protobuf)
test_find_package  (libuv     UV)
test_find_package  (tinyxml2  TinyXML2)

# Continuous Integration.
test_find_package (DOXYGEN Doxygen)

# -------------------
# Configure settings.
# -------------------

set (TEMPLATE_PREFIX "${PROJECT_SOURCE_DIR}/template")
set (MAIN_DIR "${PROJECT_SOURCE_DIR}/${MAIN_NAME}")

set (IN_PREFIX    "${TEMPLATE_PREFIX}/prefix.h.in")
set (IN_CONFIG    "${TEMPLATE_PREFIX}/config.h.in")
set (IN_DOXYFILE  "${TEMPLATE_PREFIX}/Doxyfile.in")

set (OUT_PREFIX   "${MAIN_DIR}/prefix.h")
set (OUT_CONFIG   "${MAIN_DIR}/config.h")
set (OUT_DOXYFILE "${PROJECT_SOURCE_DIR}/Doxyfile")

configure_file (${IN_PREFIX}   ${OUT_PREFIX}   NEWLINE_STYLE UNIX)
configure_file (${IN_CONFIG}   ${OUT_CONFIG}   NEWLINE_STYLE UNIX)
configure_file (${IN_DOXYFILE} ${OUT_DOXYFILE} NEWLINE_STYLE UNIX)

# -----------
# Properties.
# -----------

if (CMAKE_BUILD_TYPE MATCHES "${_CMAKE_BUILD_TYPE_DEBUG}")
    add_definitions (-DDEBUG)
else ()
    add_definitions (-DNDEBUG -DRELEASE)
endif ()

include_directories (${PROJECT_SOURCE_DIR})
if (IS_DIRECTORY ${THIRD_INC})
    include_directories (${THIRD_INC})
endif ()

link_directories (${PROJECT_SOURCE_DIR})
if (IS_DIRECTORY ${THIRD_LIB})
    link_directories (${THIRD_LIB})
endif ()

# ------------
# Sub-project.
# ------------

find_project (_lib_proj_list _test_proj_list _exe_proj_list)

message ("** Find library project: ${_lib_proj_list}")
message ("** Find test project: ${_test_proj_list}")
message ("** Find executable project: ${_exe_proj_list}")

default_build ("${_lib_proj_list}" "${_test_proj_list}" "${_exe_proj_list}")

# -----------------------
# Continuous Integration.
# -----------------------

doxygen_generate ("${OUT_DOXYFILE}")

#print_os ()
#print_compiler_id ()

