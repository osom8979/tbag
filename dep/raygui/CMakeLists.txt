cmake_minimum_required (VERSION 2.8.12)

project (raygui C)

set (SOURCES raygui.c)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    add_compile_options (-w -std=c99)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    add_compile_options (/w)
endif ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR} ${raylib_EXT_INCLUDE_DIR})
add_library (raygui STATIC ${SOURCES})
add_dependencies (raygui raylib)
target_link_libraries (raygui PRIVATE ${raylib_EXT_STATIC_LIB})

