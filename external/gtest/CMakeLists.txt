cmake_minimum_required (VERSION 2.8.8)

project (gtest CXX)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

if (WIN32)
    add_definitions (-DGTEST_OS_WINDOWS)
endif ()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options (-Wno-unused-const-variable)
endif ()

add_library (gtest
        src/gtest.cc
        src/gtest-death-test.cc
        src/gtest-filepath.cc
        src/gtest-port.cc
        src/gtest-printers.cc
        src/gtest-test-part.cc
        src/gtest-test-part.cc)
target_include_directories (gtest PRIVATE "${CMAKE_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/include")

install (TARGETS gtest
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (DIRECTORY "${CMAKE_SOURCE_DIR}/include/gtest"
         DESTINATION include
         FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")

