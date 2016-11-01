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

enable_language (CXX)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}")
link_directories (${CMAKE_CURRENT_SOURCE_DIR})

prepend (SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src"
        gtest.cc
        gtest-death-test.cc
        gtest-filepath.cc
        gtest-port.cc
        gtest-printers.cc
        gtest-test-part.cc
        gtest-test-part.cc)

if (WIN32)
    add_definitions (-DGTEST_OS_WINDOWS)
endif ()
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options (-Wno-unused-const-variable)
endif ()

if (USE_SHARED_DEPS)
    add_library (gtest SHARED ${SRCS})
else ()
    add_library (gtest STATIC ${SRCS})
endif ()

