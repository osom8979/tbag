## CMake build script.
cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

enable_language (CXX)

project (luasfml)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-w -std=c++11)
endif ()

file (GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${sfml_EXT_INCLUDE_DIR})
include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${luajit_EXT_INCLUDE_DIR})
add_library (luasfml STATIC ${SRCS})
add_dependencies (luasfml luajit sfml)

