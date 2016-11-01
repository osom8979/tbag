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

enable_language (C)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
link_directories (${CMAKE_CURRENT_SOURCE_DIR})

prepend (SRCS "${CMAKE_CURRENT_SOURCE_DIR}"
        7zFile.c 7zStream.c Alloc.c CpuArch.c LzFind.c LzmaDec.c LzmaEnc.c Sha256.c)
add_definitions (-D_7ZIP_ST)

if (USE_SHARED_DEPS)
    add_library (lzma SHARED ${SRCS})
else ()
    add_library (lzma STATIC ${SRCS})
endif ()

