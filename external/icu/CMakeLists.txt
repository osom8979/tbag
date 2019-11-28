cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

enable_language (C)
enable_language (CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options (/wd4244) ## conversion from 'type1' to 'type2', possible loss of data.
    add_compile_options (/wd4251) ## class 'type' needs to have dll-interface to be used by clients of class 'type2'.
    add_compile_options (/wd4267) ## conversion from 'size_t' to 'type', possible loss of data.
    add_definitions (-D_CRT_SECURE_NO_WARNINGS)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif ()

## icu-uc (Common)
file (GLOB_RECURSE ICUUC_SRCS "${CMAKE_SOURCE_DIR}/common/*.c"
                              "${CMAKE_SOURCE_DIR}/common/*.cpp")
file (GLOB_RECURSE ICUDATA_SRCS "${CMAKE_SOURCE_DIR}/stubdata/*.c")
add_library (icuuc ${ICUUC_SRCS} ${ICUDATA_SRCS})
target_compile_definitions (icuuc PRIVATE -DU_COMBINED_IMPLEMENTATION
                                          -DU_ATTRIBUTE_DEPRECATED=
                                          -DU_COMMON_IMPLEMENTATION)
target_include_directories (icuuc PRIVATE "${CMAKE_SOURCE_DIR}/common")

## icu-i18n (i18n)
file (GLOB_RECURSE ICUI18N_SRCS "${CMAKE_SOURCE_DIR}/i18n/*.c"
                                "${CMAKE_SOURCE_DIR}/i18n/*.cpp")
add_library (icui18n ${ICUI18N_SRCS})
target_compile_definitions (icui18n PRIVATE -DU_COMBINED_IMPLEMENTATION
                                            -DU_ATTRIBUTE_DEPRECATED=
                                            -DU_I18N_IMPLEMENTATION)
target_include_directories (icui18n PRIVATE "${CMAKE_SOURCE_DIR}/common"
                                            "${CMAKE_SOURCE_DIR}/i18n")
target_link_libraries (icui18n PRIVATE icuuc)

install (TARGETS icuuc icui18n
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (DIRECTORY "${CMAKE_SOURCE_DIR}/common"
                   "${CMAKE_SOURCE_DIR}/i18n"
         DESTINATION include/icu
         FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")

