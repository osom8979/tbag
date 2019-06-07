## CMake build script.
cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

enable_language (CXX)
project (imnodes)

set (SRCS imnodes.cpp)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options (/w)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-w)
endif ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/dep/raylib
                     ${CMAKE_SOURCE_DIR}/dep/imgui)
link_directories (${CMAKE_CURRENT_SOURCE_DIR})
add_library (imnodes STATIC ${SRCS})
add_dependencies (imnodes raylib imgui)

