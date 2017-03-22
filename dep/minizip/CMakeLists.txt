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

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options (-w)
endif ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${zlib_EXT_INCLUDE_DIR})

prepend (SRCS "${CMAKE_CURRENT_SOURCE_DIR}" ioapi.c unzip.c zip.c)
add_library (minizip STATIC ${SRCS})

add_dependencies (minizip zlib)
target_link_libraries (minizip PRIVATE ${zlib_EXT_STATIC_LIB})

