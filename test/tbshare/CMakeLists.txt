## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

enable_language (CXX)

set (SRCS "${CMAKE_CURRENT_SOURCE_DIR}/tbshare.cpp")
add_definitions (-DUSE_LIBTEST_EXPORT)
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")
add_library (tbshare SHARED ${SRCS})

