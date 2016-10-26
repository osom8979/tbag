## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

enable_language (CXX)

set (SRCS "${CMAKE_CURRENT_SOURCE_DIR}/tbproc.cpp")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")
add_executable (tbproc ${SRCS})

## Result target.
set (tbproc_test_filename "tbproc${CMAKE_EXECUTABLE_SUFFIX}")
set (tbproc_test_path "${CMAKE_BINARY_DIR}/${tbproc_test_filename}")
add_custom_target (tbproc_copy ALL DEPENDS "${tbproc_test_path}")

add_custom_command (
        OUTPUT  "${tbproc_test_path}"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:tbproc>" "${tbproc_test_path}"
        DEPENDS tbproc
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

