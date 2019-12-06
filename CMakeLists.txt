## CMake build script.

cmake_minimum_required (VERSION 2.8.8)
project (tea-bag)

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

## libtbag cmake module directory.
list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include (TbagConfig)
tbag_config ("${PROJECT_SOURCE_DIR}/INFORMATION")

if (TBAG_MSVC)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4146") ## unary minus operator applied to unsigned type, result still unsigned.
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244") ## conversion from 'type1' to 'type2', possible loss of data.
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251") ## class 'type' needs to have dll-interface to be used by clients of class 'type2'.
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267") ## conversion from 'size_t' to 'type', possible loss of data.
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275") ## non - dll-interface classkey 'identifier' used as base for dll-interface classkey 'identifier'.
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4333") ## right shift by too large amount, data loss.
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4595") ## non-member operator new or delete functions may not be declared inline.
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819") ## code page warning.
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996") ## declared deprecated warning.
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")      ## Disables all compiler warnings.
    add_definitions (-D_CRT_SECURE_NO_WARNINGS)
    add_definitions (-D_SCL_SECURE_NO_WARNINGS)
    add_definitions (-D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)
elseif (TBAG_CLANG)
    set  (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-delete-non-virtual-dtor")
elseif (TBAG_GCC)
    #set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG") ## Default
    set  (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
endif ()

tbag_flags__set_visibility_hidden ()

## Library logging.
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DENABLE_TBAG_LIBRARY_DEBUGGING_LOG=5")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DENABLE_TBAG_LIBRARY_DEBUGGING_LOG_LOCK")

tbag_config__set_option (USE_CCACHE  ON) ## Enable ccache compile.
tbag_config__set_option (USE_GOLD    ON) ## Enable ld.gold linker if gcc compiler.
tbag_config__set_option (USE_PCH    OFF) ## Enable PCH compile.
tbag_config__set_option (USE_RTTI   OFF) ## Enable RTTI flag.

tbag_config__set_option (USE_MSAN OFF) ## Enable MemorySanitizer if clang compiler.
tbag_config__set_option (USE_ASAN OFF) ## Enable AddressSanitizer if clang compiler.

tbag_config__set_option (DISABLE_TDLOG                OFF) ## Disable tbag default logger.
tbag_config__set_option (DISABLE_LUAJIT_EXE_FLAGS     OFF) ## Disable LuaJIT Embedding flags if apple.
tbag_config__set_option (DISABLE_DEFAULT_TPOT_LUAJIT  OFF) ## Do not use the default TpoT mode as LuaJIT.
tbag_config__set_option (DISABLE_TESTER               OFF) ## Do not build the tester target.

tbag_config__set_option (INSTALL_WITH_FAKE_LUAJIT ON) ## Install fake LuaJIT.

## Install prefix.
if (CMAKE_INSTALL_PREFIX)
    set (TBAG_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
    add_definitions ("-DTBAG_INSTALL_PREFIX=\"${TBAG_INSTALL_PREFIX}\"")
endif ()

## Configure settings.
if (USE_CCACHE)
    tbag_config__use_ccache ()
endif ()
if (USE_GOLD AND TBAG_GCC)
    set (CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}    -fuse-ld=gold")
    set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold")
endif ()
if (USE_MSAN)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=memory")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
    if (TBAG_MACOSX)
        message (WARNING "MemorySanitizer is not supported on macOS.")
    endif ()
endif ()
if (USE_ASAN)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif ()

tbag_config__find_thread ()
tbag_config__default_generate_c_header ()

tbag_config__set_library_option (USE_OPENGL   OpenGL    OPENGL_FOUND)
tbag_config__set_library_option (USE_OPENCL   OpenCL    OpenCL_FOUND)
tbag_config__set_library_option (USE_CUDA     TbagCUDA  CUDA_FOUND)
tbag_config__set_library_option (USE_DOXYGEN  Doxygen   DOXYGEN_FOUND)

#set (TbagPython_PYENV_NAME "opy-$ENV{USER}-3.6.1")
#find_package (TbagPython COMPONENTS Interpreter Development NumPy)

if (DISABLE_TDLOG)
    message (STATUS "Disable tDLog feature")
else ()
    message (STATUS "Enable tDLog feature")
endif ()

## External project.
include (${CMAKE_SOURCE_DIR}/external/External.cmake)
list (APPEND EXTERNAL_TARGETS archive blend2d bzip2 civetweb date)
list (APPEND EXTERNAL_TARGETS freetype glfw icu lmdb luajit lzma)
list (APPEND EXTERNAL_TARGETS nng openblas ressl sqlite3 uv zlib)
add_custom_target (external DEPENDS ${EXTERNAL_TARGETS})

## SIMDPP.
include (${CMAKE_SOURCE_DIR}/dep/simdpp/cmake/SimdppMultiarch.cmake)

# Check what instruction sets the current host supports. Use them for testing
#message (STATUS "Checking instruction set support in the compiler...")
#simdpp_get_compilable_archs (SIMDPP_COMPILABLE_ARCHS)
message (STATUS "Checking instruction sets to run tests for on this host...")
simdpp_get_runnable_archs (SIMDPP_RUNNABLE_ARCHS)
message (STATUS "Simdpp runnable archs: ${SIMDPP_RUNNABLE_ARCHS}")
foreach (__arch_cursor ${SIMDPP_RUNNABLE_ARCHS})
    set (TBAG_SIMDPP_RUNNABLE_ARCH_${__arch_cursor} 1)
endforeach ()

## Library subproject.
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/gtest)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/flatbuffers)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/http-parser)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/minizip)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/lemon)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/lfds)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/raylib)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/imgui)
add_subdirectory (${CMAKE_SOURCE_DIR}/dep/imnodes)

## CI Preset.
if (RUN_TRAVIS)
    message (STATUS "Enable Travis CI")
    tbag_config__set_option (WITH_COVERAGE         ON)
    tbag_config__set_option (SKIP_FSPOLL_TESTER    ON)
    tbag_config__set_option (SKIP_FINDPATH_TESTER  ON)
elseif (RUN_APPVEYOR)
    message (STATUS "Enable AppVeyor CI")
    tbag_config__set_option (WITH_COVERAGE         OFF)
    tbag_config__set_option (SKIP_FSPOLL_TESTER    OFF)
    tbag_config__set_option (SKIP_FINDPATH_TESTER  OFF)
else ()
    tbag_config__set_option (WITH_COVERAGE         OFF)
    tbag_config__set_option (SKIP_FSPOLL_TESTER    OFF)
    tbag_config__set_option (SKIP_FINDPATH_TESTER  OFF)
endif ()

if (WITH_COVERAGE)
    message (STATUS "Enable coverage.")
endif ()

if (USE_DOXYGEN)
    find_package (TbagDoxygen)
    if (DOXYGEN_FOUND)
        set (DOXYFILE_INPUT       "${PROJECT_SOURCE_DIR}/template/Doxyfile.in")
        set (DOXYFILE_OUTPUT      "${PROJECT_BINARY_DIR}/Doxyfile")
        set (DOXYFILE_OUTPUT_ZIP  "${PROJECT_BINARY_DIR}/libtbag-doc-${VERSION}.tar.gz")
        set (DOXYFILE_HTML_DIR    "${PROJECT_BINARY_DIR}/doxygen")

        configure_file ("${DOXYFILE_INPUT}" "${DOXYFILE_OUTPUT}")
        tbag_doxygen__generate_index ("${PROJECT_BINARY_DIR}/doxygen.html")
        tbag_doxygen__target ("${DOXYFILE_OUTPUT}" "${PROJECT_BINARY_DIR}")
        tbag_doxygen__target_tgz ("${DOXYFILE_OUTPUT_ZIP}" "${DOXYFILE_HTML_DIR}")
    endif ()
endif ()

## Header checker.
include (CheckIncludeFile)
## - ${CMAKE_REQUIRED_FLAGS}: string of compile command line flags
## - ${CMAKE_REQUIRED_DEFINITIONS}: list of macros to define (-DFOO=bar)
## - ${CMAKE_REQUIRED_INCLUDES}: list of include directories

check_include_file (netinet/in.h   TBAG_HAVE_NETINET_IN_H)
check_include_file (arpa/inet.h    TBAG_HAVE_ARPA_INET_H)
check_include_file (sys/syscall.h  TBAG_HAVE_SYS_SYSCALL_H)
check_include_file (sys/sysctl.h   TBAG_HAVE_SYS_SYSCTL_H)
check_include_file (sys/statvfs.h  TBAG_HAVE_SYS_STATVFS_H)
check_include_file (sys/mman.h     TBAG_HAVE_SYS_MMAN_H)
check_include_file (sys/types.h    TBAG_HAVE_SYS_TYPES_H)
check_include_file (sys/uio.h      TBAG_HAVE_SYS_UIO_H)
check_include_file (syscall.h      TBAG_HAVE_SYSCALL_H)
check_include_file (unistd.h       TBAG_HAVE_UNISTD_H)
check_include_file (dlfcn.h        TBAG_HAVE_DLFCN_H)
check_include_file (execinfo.h     TBAG_HAVE_EXECINFO_H)
check_include_file (unwind.h       TBAG_HAVE_UNWIND_H)
check_include_file (libunwind.h    TBAG_HAVE_LIBUNWIND_H)
check_include_file (Windows.h      TBAG_HAVE_WINDOWS_H) ## Win32
check_include_file (Objbase.h      TBAG_HAVE_OBJBASE_H) ## Win32
check_include_file (Dbgeng.h       TBAG_HAVE_DBGENG_H)  ## Win32
check_include_file (intrin.h       TBAG_HAVE_INTRIN_H)  ## Win32
check_include_file (cpuid.h        TBAG_HAVE_CPUID_H)   ## GCC or CLANG
check_include_file (endian.h       TBAG_HAVE_ENDIAN_H)

## Compile checker.
include (CheckCSourceCompiles)
include (CheckCXXSourceCompiles)
## - ${CMAKE_REQUIRED_FLAGS}: string of compile command line flags
## - ${CMAKE_REQUIRED_DEFINITIONS}: list of macros to define (-DFOO=bar)
## - ${CMAKE_REQUIRED_INCLUDES}: list of include directories
## - ${CMAKE_REQUIRED_LIBRARIES}: list of libraries to link
if (WIN32 AND TBAG_HAVE_OBJBASE_H)
    check_cxx_source_compiles ("
        #include <Objbase.h>
        #pragma comment(lib, \"ole32.lib\")
        int main() { return ::CoInitializeEx(NULL, COINIT_MULTITHREADED) == S_OK ? 0 : 1; }
        " TBAG_HAVE_COINITIALIZEEX_FUNC)
endif ()

if (TBAG_HAVE_SYS_UIO_H)
    check_c_source_compiles ("
        #include <sys/uio.h>
        void main() { preadv(0, NULL, 0, 0); }
        " TBAG_HAVE_PREADV_FUNC)
    check_c_source_compiles ("
        #include <sys/uio.h>
        void main() { readv(0, NULL, 0); }
        " TBAG_HAVE_READV_FUNC)
endif ()

## Type checker.
include (CheckTypeSize)
check_type_size ("long long" LONG_LONG)

set (CONFIG_EX_INPUT  "${PROJECT_SOURCE_DIR}/template/config-ex.h.in")
set (CONFIG_EX_OUTPUT "${PROJECT_SOURCE_DIR}/${MAIN_NAME}/config-ex.h")
configure_file ("${CONFIG_EX_INPUT}" "${CONFIG_EX_OUTPUT}")

set (TBAG_BUILD_PROJECTS libtbag)
if (NOT DISABLE_TESTER)
    set (TBAG_BUILD_PROJECTS ${TBAG_BUILD_PROJECTS} tools/libtbshare tools/tbproc tester)
endif ()

## Run project.
tbag_flags__set_install_rpath () # [WARNING] Do not change the calling sequence.
tbag_project (NAME "${PROJECT_NAME}"
              ROOT "${PROJECT_SOURCE_DIR}"
              LANG C CXX CXXPCH
              PROJ ${TBAG_BUILD_PROJECTS})

## libtbag-cmake build packages.
install (DIRECTORY "${PROJECT_SOURCE_DIR}/cmake"
         DESTINATION "share/libtbag")

if (INSTALL_WITH_FAKE_LUAJIT)
    install (CODE "execute_process (COMMAND ${CMAKE_COMMAND} -E create_symlink
                  \"${CMAKE_INSTALL_PREFIX}/bin/tpot${CMAKE_EXECUTABLE_SUFFIX}\"
                  \"${CMAKE_INSTALL_PREFIX}/bin/luajit${CMAKE_EXECUTABLE_SUFFIX}\")")
    install (CODE "execute_process (COMMAND ${CMAKE_COMMAND} -E create_symlink
                  \"${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}tbag${CMAKE_SHARED_LIBRARY_SUFFIX}\"
                  \"${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}lua${CMAKE_SHARED_LIBRARY_SUFFIX}\")")
    install (FILES "${PROJECT_BINARY_DIR}/external/local/include/lauxlib.h"
                   "${PROJECT_BINARY_DIR}/external/local/include/lua.h"
                   "${PROJECT_BINARY_DIR}/external/local/include/lua.hpp"
                   "${PROJECT_BINARY_DIR}/external/local/include/luaconf.h"
                   "${PROJECT_BINARY_DIR}/external/local/include/luajit.h"
                   "${PROJECT_BINARY_DIR}/external/local/include/lualib.h"
             DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
endif ()

## Packaging.
set (CPACK_SYSTEM_NAME                  "${CMAKE_SYSTEM_NAME}")
set (CPACK_PACKAGE_NAME                 "${MAIN_NAME}")
set (CPACK_PACKAGE_VENDOR               "${MAIN_AUTHOR}")
set (CPACK_PACKAGE_CONTACT              "${MAIN_EMAIL}")
set (CPACK_PACKAGE_VERSION_MAJOR        "${VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR        "${VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH        "${VERSION_PATCH}")
set (CPACK_PACKAGE_VERSION              "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set (CPACK_PACKAGE_DESCRIPTION_FILE     "${CMAKE_SOURCE_DIR}/INFORMATION")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY  "${MAIN_BRIEF}")
set (CPACK_PACKAGE_FILE_NAME            "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_SYSTEM_NAME}")
set (CPACK_RESOURCE_FILE_README         "${CMAKE_SOURCE_DIR}/README.md")
set (CPACK_RESOURCE_FILE_LICENSE        "${CMAKE_SOURCE_DIR}/LICENSE")

## Debian packaging.
set (CPACK_DEBIAN_PACKAGE_NAME          "${CPACK_PACKAGE_NAME}")
set (CPACK_DEBIAN_PACKAGE_VERSION       "${CPACK_PACKAGE_VERSION}")
set (CPACK_DEBIAN_PACKAGE_MAINTAINER    "${MAIN_AUTHOR} <${MAIN_EMAIL}>")
set (CPACK_DEBIAN_PACKAGE_HOMEPAGE      "http://osom8979.github.io/tbag/")
set (CPACK_DEBIAN_PACKAGE_SECTION       "devel")
set (CPACK_DEBIAN_PACKAGE_PRIORITY      "optional")
set (CPACK_DEBIAN_PACKAGE_DEPENDS       "")
set (CPACK_DEBIAN_PACKAGE_RECOMMENDS    "")
set (CPACK_DEBIAN_PACKAGE_SUGGESTS      "")
set (CPACK_DEBIAN_PACKAGE_CONFLICTS     "")

include (CPack)

## Enable output of compile commands during generation.
## For the YouCompleteMe
## Check the ${CMAKE_BINARY_DIR}/compile_commands.json file.
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Print result messages.
tbag_preview ()

if (USE_PCH)
    set (CMAKE_CXXPCH_FLAGS ${CMAKE_CXX_FLAGS})
    target_precompiled_header (tbag libtbag/pch.h)
endif ()

