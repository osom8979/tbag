## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

enable_language (C)
enable_language (CXX)


if (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
    add_compile_options (/wd4244) ## conversion from 'type1' to 'type2', possible loss of data.
    add_compile_options (/wd4267) ## conversion from 'size_t' to 'type', possible loss of data.
elseif ((CMAKE_CXX_COMPILER_ID STREQUAL Clang) OR (CMAKE_CXX_COMPILER_ID STREQUAL GNU))
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif ()

add_definitions (-DU_COMBINED_IMPLEMENTATION)

## icu-uc (Common)
file (GLOB_RECURSE ICUUC_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/common/*.c"
                              "${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp")
file (GLOB_RECURSE ICUDATA_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/stubdata/*.c")
add_library (icuuc STATIC ${ICUUC_SRCS} ${ICUDATA_SRCS})
target_compile_definitions (icuuc PRIVATE -DU_ATTRIBUTE_DEPRECATED=
                                          -DU_COMMON_IMPLEMENTATION)
target_include_directories (icuuc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common")

## icu-i18n (i18n)
file (GLOB_RECURSE ICUI18N_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/i18n/*.c"
                                "${CMAKE_CURRENT_SOURCE_DIR}/i18n/*.cpp")
add_library (icui18n STATIC ${ICUI18N_SRCS})
target_compile_definitions (icui18n PRIVATE -DU_ATTRIBUTE_DEPRECATED=
                                            -DU_I18N_IMPLEMENTATION)
target_include_directories (icui18n PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/i18n")

