## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

function (prepend __result __prefix)
    set (${__result})
    foreach (__cursor ${ARGN})
        list (APPEND ${__result} "${__prefix}/${__cursor}")
    endforeach ()
    set (${__result} "${${__result}}" PARENT_SCOPE)
endfunction ()

function (check_include_and_define __header __define)
    check_include_file (${__header} ${__define})
    if (${__define})
        add_definitions (-D${__define})
    endif ()
endfunction ()

enable_language (C)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
link_directories (${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_definitions (-DHAVE_HIDDEN)
endif ()

prepend (SRCS "${CMAKE_CURRENT_SOURCE_DIR}"
        adler32.c crc32.c deflate.c infback.c inffast.c inflate.c inftrees.c
        trees.c zutil.c compress.c uncompr.c gzclose.c gzlib.c gzread.c gzwrite.c)

#include (CheckCSourceCompiles)
#enable_testing ()

include (CheckIncludeFile)
check_include_and_define (sys/types.h HAVE_SYS_TYPES_H)
check_include_and_define (unistd.h    HAVE_UNISTD_H)
check_include_and_define (stdarg.h    HAVE_STDARG_H)

include (CheckTypeSize)
check_type_size (off64_t OFF64_T)
if (HAVE_OFF64_T)
    add_definitions (-D_LARGEFILE64_SOURCE=1)
endif ()

include (CheckFunctionExists)
check_function_exists (fseeko HAVE_FSEEKO)
if (NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif ()

if (MSVC)
    add_definitions (-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions (-D_CRT_NONSTDC_NO_DEPRECATE)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options (-w)
    #add_compile_options (-m64)
endif ()

add_library (z STATIC ${SRCS})

