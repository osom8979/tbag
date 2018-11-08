cmake_minimum_required (VERSION 3.0.2 FATAL_ERROR)

cmake_policy (SET CMP0016 NEW)
if (POLICY CMP0042)
    cmake_policy (SET CMP0042 NEW)
endif ()

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include (TargetArch)
include (CheckIncludeFileCXX)

project (cds)

set (PROJECT_VERSION 2.4.0)

# Options
option (WITH_ASAN "Build ASan+UBSan instrumented code" OFF)
option (WITH_TSAN "Build TSan instrumented code" OFF)
set (CMAKE_TARGET_ARCHITECTURE "" CACHE string "Target build architecture")

find_package (Threads)

if (NOT CMAKE_TARGET_ARCHITECTURE)
    target_architecture (CMAKE_TARGET_ARCHITECTURE)
endif ()

if (APPLE)
    set (CMAKE_C_FLAGS     "${CMAKE_C_FLAGS} -D_DARWIN_C_SOURCE")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DARWIN_C_SOURCE")
endif ()

if (WITH_ASAN)
    if (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set (CMAKE_CXX_FLAGS_DEBUG "-D_DEBUG")
        set (CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -fPIC -fsanitize=address,undefined -g -DCDS_ADDRESS_SANITIZER_ENABLED -fno-omit-frame-pointer -fno-optimize-sibling-calls")
        set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1 -fsanitize=address,undefined -g -DCDS_ASAN_ENABLED -fno-omit-frame-pointer -fno-optimize-sibling-calls")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined -pie")
    else ()
        message (WARNING "Compiler does not support AddressSanitizer")
    endif ()
endif ()

if (WITH_TSAN)
    if (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set (CMAKE_CXX_FLAGS_DEBUG   "-D_DEBUG")
        set (CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -fPIC -fsanitize=thread -g -DCDS_THREAD_SANITIZER_ENABLED -fno-omit-frame-pointer")
        set (CMAKE_C_FLAGS   "${CMAKE_CXX_FLAGS} -O1 -fPIC -fsanitize=thread -g -DCDS_THREAD_SANITIZER_ENABLED -fno-omit-frame-pointer")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread -pie")
    else ()
        message (WARNING "Compiler does not support ThreadSanitizer")
    endif()
endif ()

set (CDS_STATIC_LIBRARY ${PROJECT_NAME})
set (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

if (CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    string (REGEX MATCHALL "-std=[^ ]+" cxx_std_found ${CMAKE_CXX_FLAGS} " dummy@rg")
    if (cxx_std_found)
        message ("C++ std: ${cxx_std_found}")
    else ()
        list (APPEND LIBCDS_PUBLIC_CXX_FLAGS "-std=c++11")
        message ("C++ std: -std=c++11 (default)")
    endif ()

    list (APPEND LIBCDS_PRIVATE_CXX_FLAGS "-Wall" "-Wextra" "-pedantic")

    if (CMAKE_TARGET_ARCHITECTURE STREQUAL "x86_64")
        list (APPEND LIBCDS_PUBLIC_CXX_FLAGS "-mcx16")
        set (LIB_SUFFIX "64")

        # GCC-7: 128-bit atomics support is implemented via libatomic on amd64
        #        see https://gcc.gnu.org/ml/gcc/2017-01/msg00167.html
        # Maybe, it will be changed in future
        if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "7.0.0" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "8.0.0")
            set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -latomic")
        endif ()
    endif ()
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4512")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:inline")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "AIX")
    set  (CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> -q -c ${CMAKE_STATIC_LINKER_FLAGS} -o <TARGET> <OBJECTS>")
    list (APPEND LIBCDS_PRIVATE_CXX_FLAGS "-Wl,-G")
    set  (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-brtl")
endif ()

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

check_include_file_cxx (linux/membarrier.h CDS_HAVE_LINUX_MEMBARRIER_H CMAKE_CXX_FLAGS)

set (SOURCES src/init.cpp
        src/hp.cpp
        src/dhp.cpp
        src/urcu_gp.cpp
        src/urcu_sh.cpp
        src/thread_data.cpp
        src/topology_hpux.cpp
        src/topology_linux.cpp
        src/topology_osx.cpp
        src/dllmain.cpp)

add_library (${CDS_STATIC_LIBRARY} STATIC ${SOURCES})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_definitions (${CDS_STATIC_LIBRARY} PRIVATE CDS_BUILD_STATIC_LIB _ENABLE_ATOMIC_ALIGNMENT_FIX CDS_BUILD_LIB)
else ()
    target_compile_definitions (${CDS_STATIC_LIBRARY} PRIVATE CDS_BUILD_STATIC_LIB)
endif ()

target_link_libraries      (${CDS_STATIC_LIBRARY} PRIVATE   "${CMAKE_THREAD_LIBS_INIT}")
target_include_directories (${CDS_STATIC_LIBRARY} INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>" $<INSTALL_INTERFACE:include>)
target_compile_options     (${CDS_STATIC_LIBRARY} PUBLIC    "${LIBCDS_PUBLIC_CXX_FLAGS}")
target_compile_options     (${CDS_STATIC_LIBRARY} PRIVATE   "${LIBCDS_PRIVATE_CXX_FLAGS}")

