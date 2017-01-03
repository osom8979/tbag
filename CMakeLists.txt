## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

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

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4146") ## unary minus operator applied to unsigned type, result still unsigned.
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244") ## conversion from 'type1' to 'type2', possible loss of data.
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251") ## class 'type' needs to have dll-interface to be used by clients of class 'type2'.
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267") ## conversion from 'size_t' to 'type', possible loss of data.
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4333") ## right shift by too large amount, data loss.
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4595") ## non-member operator new or delete functions may not be declared inline.
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819") ## code page warning.
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996") ## declared deprecated warning.
    add_definitions (-D_CRT_SECURE_NO_WARNINGS)
endif()

# Setup the THIRD_PREFIX variable.
string (REPLACE "\\" "/" __tparty_home_path "$ENV{TPARTY_HOME}/local")
if (IS_DIRECTORY "${__tparty_home_path}")
    tbag_config__add_third ("${__tparty_home_path}")
else ()
    message (STATUS "Not found tparty local directory: ${__tparty_home_path}")
endif ()


## Debugging logger settings.
option (USE_TBAG_DEBUGGING_LOGGER "Enable tbag debugging logger." ON)

## Configure settings.
tbag_config__default_generate_c_header ()

## Library subproject.
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/flatbuffers)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/gtest)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/icu)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/lmdb)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/lzma)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/minizip)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/sqlite3)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/uv)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/zlib)

## Lua settings.
option (USE_LUA "Don't use the LUA library." OFF)
if (USE_LUA)
    add_subdirectory (${CMAKE_SOURCE_DIR}/dep/lua)
endif ()

## Test subproject.
add_subdirectory (${CMAKE_SOURCE_DIR}/test/tbproc)
add_subdirectory (${CMAKE_SOURCE_DIR}/test/tbshare)

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

## Compile checker.
include (CheckCSourceCompiles)
include (CheckCXXSourceCompiles)
## - ${CMAKE_REQUIRED_FLAGS}: string of compile command line flags
## - ${CMAKE_REQUIRED_DEFINITIONS}: list of macros to define (-DFOO=bar)
## - ${CMAKE_REQUIRED_INCLUDES}: list of include directories
## - ${CMAKE_REQUIRED_LIBRARIES}: list of libraries to link
check_c_source_compiles ("
    #include <cstring>
    #include <windows.h>
    #include \"Dbgeng.h\"
    #pragma comment(lib, \"ole32.lib\")
    #pragma comment(lib, \"Dbgeng.lib\")
    int main() { CoInitializeEx(0, COINIT_MULTITHREADED); return 0; }
    " HAVE_WINDBG)
check_c_source_compiles ("
    #include <dlfcn.h>
    #include <execinfo.h>
    int main() { void* buf[10]; ::backtrace(buf, 10); return 0; }
    " HAVE_BACKTRACE)
check_c_source_compiles ("
    #include <unwind.h>
    int main() { return 0; }
    " HAVE_UNWIND)

if (HAVE_WINDBG)
    add_definitions (-DBOOST_STACKTRACE_USE_WINDBG)
elseif (HAVE_BACKTRACE)
    add_definitions (-DBOOST_STACKTRACE_USE_BACKTRACE)
elseif (HAVE_UNWIND)
    add_definitions (-DBOOST_STACKTRACE_USE_UNWIND)
else ()
    add_definitions (-DBOOST_STACKTRACE_USE_NOOP)
endif ()

set (CONFIG_EX_INPUT  "${PROJECT_SOURCE_DIR}/template/config-ex.h.in")
set (CONFIG_EX_OUTPUT "${PROJECT_SOURCE_DIR}/${MAIN_NAME}/config-ex.h")
configure_file ("${CONFIG_EX_INPUT}" "${CONFIG_EX_OUTPUT}")

## Run project.
tbag_project2 (tea-bag)

## libtbag-cmake build packages.
install (DIRECTORY "${PROJECT_SOURCE_DIR}/cmake"
         DESTINATION "share/libtbag")

## Print result messages.
tbag_preview ()

