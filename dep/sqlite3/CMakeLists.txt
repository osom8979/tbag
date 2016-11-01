## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

enable_language (C)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")
set (SRCS "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.c")

if (USE_SHARED_DEPS)
    add_library (sqlite3 SHARED ${SRCS})
else ()
    add_library (sqlite3 STATIC ${SRCS})
endif ()

