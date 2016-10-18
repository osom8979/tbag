## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()

function (prepend __result __prefix)
    set (${__result})
    foreach (__cursor ${ARGN})
        list (APPEND ${__result} "${__prefix}/${__cursor}")
    endforeach ()
    set (${__result} "${${__result}}" PARENT_SCOPE)
endfunction ()

enable_language (C)
enable_language (CXX)


## Find thread library.
find_package (Threads)
if (CMAKE_THREAD_LIBS_INIT)
    message (STATUS "Thread flags: ${CMAKE_THREAD_LIBS_INIT}")
endif ()

if (NOT WIN32)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif ()

## icu-data (Data)
file (GLOB_RECURSE ICUDATA_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/stubdata/*.c")
add_library (icudata STATIC ${ICUDATA_SRCS})
target_compile_definitions (icudata PRIVATE -DU_ATTRIBUTE_DEPRECATED= -DU_COMMON_IMPLEMENTATION)
target_include_directories (icudata PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common")

## icu-uc (Common)
file (GLOB_RECURSE ICUUC_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/common/*.c"
                              "${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp")
add_library (icuuc STATIC ${ICUUC_SRCS})
target_compile_definitions (icuuc PRIVATE -DU_ATTRIBUTE_DEPRECATED= -DU_COMMON_IMPLEMENTATION)
target_include_directories (icuuc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common")

