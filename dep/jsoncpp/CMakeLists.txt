## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

enable_language (CXX)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})
set (SRCS "${CMAKE_CURRENT_SOURCE_DIR}/jsoncpp.cpp")

if (USE_SHARED_DEPS)
    add_library (jsoncpp SHARED ${SRCS})
else ()
    add_library (jsoncpp STATIC ${SRCS})
endif ()

