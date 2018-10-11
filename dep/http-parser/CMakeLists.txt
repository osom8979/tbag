## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

enable_language (C)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options (/w)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-w)
endif ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
link_directories (${CMAKE_CURRENT_SOURCE_DIR})

set (SRCS "${CMAKE_CURRENT_SOURCE_DIR}/http_parser.c")

add_library (http_parser STATIC ${SRCS})

