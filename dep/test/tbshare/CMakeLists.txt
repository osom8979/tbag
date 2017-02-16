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

## Result target.
set (tbshare_test_filename "${CMAKE_SHARED_LIBRARY_PREFIX}tbshare${CMAKE_SHARED_LIBRARY_SUFFIX}")
set (tbshare_test_path "${CMAKE_BINARY_DIR}/${tbshare_test_filename}")
add_custom_target (tbshare_copy ALL DEPENDS "${tbshare_test_path}")

add_custom_command (
        OUTPUT  "${tbshare_test_path}"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:tbshare>" "${tbshare_test_path}"
        DEPENDS tbshare
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

