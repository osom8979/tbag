cmake_minimum_required (VERSION 2.8.8)

project (lfds C)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options (-w)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options (-w)
endif ()

file (GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/src"
                     "${CMAKE_CURRENT_SOURCE_DIR}/inc")
add_library (lfds ${SRCS})

install (TARGETS lfds
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (DIRECTORY "${CMAKE_SOURCE_DIR}/inc/liblfds711"
         DESTINATION include
         FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
install (FILES "${CMAKE_SOURCE_DIR}/inc/liblfds711.h"
         DESTINATION include)

