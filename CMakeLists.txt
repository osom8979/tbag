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

if (CMAKE_C_COMPILER_ID STREQUAL MSVC)
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4146") ## unary minus operator applied to unsigned type, result still unsigned.
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244") ## conversion from 'type1' to 'type2', possible loss of data.
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251") ## class 'type' needs to have dll-interface to be used by clients of class 'type2'.
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267") ## conversion from 'size_t' to 'type', possible loss of data.
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275") ## non - dll-interface classkey 'identifier' used as base for dll-interface classkey 'identifier'.
    #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4333") ## right shift by too large amount, data loss.
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4595") ## non-member operator new or delete functions may not be declared inline.
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819") ## code page warning.
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996") ## declared deprecated warning.
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")      ## Disables all compiler warnings.
    add_definitions (-D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_C_COMPILER_ID STREQUAL Clang)
    # EMPTY.
elseif (CMAKE_C_COMPILER_ID STREQUAL GNU)
    #set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG") ## Default
    set  (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
endif ()

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
tbag_config__use_ccache ()
tbag_config__default_generate_c_header ()

## External project.
include (${CMAKE_SOURCE_DIR}/external/External.cmake)
include (${CMAKE_SOURCE_DIR}/dep/simdpp/cmake/SimdppMultiarch.cmake)

# Check what instruction sets the current host supports. Use them for testing
#message (STATUS "Checking instruction set support in the compiler...")
#simdpp_get_compilable_archs (SIMDPP_COMPILABLE_ARCHS)
message (STATUS "Checking instruction sets to run tests for on this host...")
simdpp_get_runnable_archs (SIMDPP_RUNNABLE_ARCHS)

## Library subproject.
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/gtest)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/flatbuffers)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/http-parser)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/icu)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/minizip)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/uv)

tbag_config__set_option (WITH_COVERAGE         OFF)
tbag_config__set_option (SKIP_FSPOLL_TESTER    OFF)
tbag_config__set_option (SKIP_FINDPATH_TESTER  OFF)

if (WITH_COVERAGE)
    message (STATUS "Enable coverage.")
endif ()

## Thread settings.
tbag_config__find_thread ()

## SFML settings.
tbag_config__set_library_option (USE_SFML TbagSFML SFML_FOUND)

## OpenCL settings.
tbag_config__set_library_option (USE_OPENCL OpenCL OpenCL_FOUND)
if (OpenCL_FOUND)
    message (STATUS "Found OpenCL: ${OpenCL_LIBRARY}")
endif ()

## CUDA settings.
tbag_config__set_library_option (USE_CUDA TbagCUDA CUDA_FOUND)

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

check_include_file (syscall.h      HAVE_SYSCALL_H)
check_include_file (sys/syscall.h  HAVE_SYS_SYSCALL_H)
check_include_file (sys/mman.h     HAVE_SYS_MMAN_H)
check_include_file (unistd.h       HAVE_UNISTD_H)
check_include_file (dlfcn.h        HAVE_DLFCN_H)
check_include_file (execinfo.h     HAVE_EXECINFO_H)
check_include_file (unwind.h       HAVE_UNWIND_H)
check_include_file (libunwind.h    HAVE_LIBUNWIND_H)
check_include_file (Windows.h      HAVE_WINDOWS_H) ## Win32
check_include_file (Objbase.h      HAVE_OBJBASE_H) ## Win32
check_include_file (Dbgeng.h       HAVE_DBGENG_H)  ## Win32
check_include_file (Dbgeng.h       HAVE_DBGENG_H)  ## Win32
check_include_file (intrin.h       HAVE_INTRIN_H)  ## Win32
check_include_file (cpuid.h        HAVE_CPUID_H)   ## GCC or CLANG

## Compile checker.
include (CheckCSourceCompiles)
include (CheckCXXSourceCompiles)
## - ${CMAKE_REQUIRED_FLAGS}: string of compile command line flags
## - ${CMAKE_REQUIRED_DEFINITIONS}: list of macros to define (-DFOO=bar)
## - ${CMAKE_REQUIRED_INCLUDES}: list of include directories
## - ${CMAKE_REQUIRED_LIBRARIES}: list of libraries to link
if (WIN32 AND HAVE_OBJBASE_H)
    check_cxx_source_compiles ("
        #include <Objbase.h>
        #pragma comment(lib, \"ole32.lib\")
        int main() { return ::CoInitializeEx(NULL, COINIT_MULTITHREADED) == S_OK ? 0 : 1; }
        " HAVE_COINITIALIZEEX_FUNC)
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

