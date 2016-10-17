## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

enable_language (C)
enable_language (CXX)

## Find thread library.
find_package (Threads)
if (CMAKE_THREAD_LIBS_INIT)
    message (STATUS "Thread flags: ${CMAKE_THREAD_LIBS_INIT}")
endif ()

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}")
link_directories (${CMAKE_CURRENT_SOURCE_DIR})

set (LIB_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/gtest.cc"
              "${CMAKE_CURRENT_SOURCE_DIR}/src/gtest-death-test.cc"
              "${CMAKE_CURRENT_SOURCE_DIR}/src/gtest-filepath.cc"
              "${CMAKE_CURRENT_SOURCE_DIR}/src/gtest-port.cc"
              "${CMAKE_CURRENT_SOURCE_DIR}/src/gtest-printers.cc"
              "${CMAKE_CURRENT_SOURCE_DIR}/src/gtest-test-part.cc"
              "${CMAKE_CURRENT_SOURCE_DIR}/src/gtest-test-part.cc")
set (MAIN_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/gtest_main.cc)

#add_library (gtest      STATIC ${LIB_SRCS})
add_library  (gtest_main STATIC ${LIB_SRCS} ${MAIN_SRCS})

