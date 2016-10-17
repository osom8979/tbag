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

function (check_include_and_define __header __define)
    check_include_file (${__header} ${__define})
    if (DEFINED ${__define})
        add_definitions (-D${__define}=1)
    elseif ()
        add_definitions (-D${__define}=0)
    endif ()
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
set (ICUDATA_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/stubdata/stubdata.c")
add_library (icudata STATIC ${ICUDATA_SRCS})
target_compile_definitions (icudata PRIVATE -DU_ATTRIBUTE_DEPRECATED= -DU_COMMON_IMPLEMENTATION)
target_include_directories (icudata PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common")

## icu-uc (Common)
file (GLOB_RECURSE ICUUC_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/common/*.c"
                              "${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp")
add_library (icuuc STATIC ${ICUUC_SRCS})
target_compile_definitions (icuuc PRIVATE -DU_ATTRIBUTE_DEPRECATED= -DU_COMMON_IMPLEMENTATION)
target_include_directories (icuuc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common")

## icu-i18n (i18n)
file (GLOB_RECURSE ICUI18N_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/i18n/*.c"
                                "${CMAKE_CURRENT_SOURCE_DIR}/i18n/*.cpp")
add_library (icui18n STATIC ${ICUI18N_SRCS})
target_compile_definitions (icui18n PRIVATE -DU_ATTRIBUTE_DEPRECATED= -DU_I18N_IMPLEMENTATION)
target_include_directories (icui18n PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/i18n")

## icu-tu (Tool utility)
file (GLOB_RECURSE ICUTU_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/tools/toolutil/*.c"
                              "${CMAKE_CURRENT_SOURCE_DIR}/tools/toolutil/*.cpp")
add_library (icutu STATIC ${ICUTU_SRCS})
target_compile_definitions (icutu PRIVATE -DU_ATTRIBUTE_DEPRECATED= -DU_TOOLUTIL_IMPLEMENTATION)
target_include_directories (icutu PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common"
                                          "${CMAKE_CURRENT_SOURCE_DIR}/i18n")

## icupkg (Tool)
set (ICUPKG_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/tools/icupkg/icupkg.cpp")
add_executable (icupkg ${ICUPKG_SRCS})
target_compile_definitions (icupkg PRIVATE -DU_ATTRIBUTE_DEPRECATED=)
target_include_directories (icupkg PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common"
                                           "${CMAKE_CURRENT_SOURCE_DIR}/tools/toolutil")
add_dependencies (icupkg icutu icui18n icuuc icudata)
target_link_libraries (icupkg PRIVATE icutu icui18n icuuc icudata)
if (UNIX)
    if (CMAKE_USE_PTHREADS_INIT)
        target_link_libraries (icupkg PRIVATE -lpthread)
    endif ()
    target_link_libraries (icupkg PRIVATE -lm)
endif ()

## pkgdata (Tool)
set (PKGDATA_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/tools/pkgdata/pkgdata.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/tools/pkgdata/pkgtypes.c")
add_executable (pkgdata ${PKGDATA_SRCS})
target_compile_definitions (pkgdata PRIVATE -DU_ATTRIBUTE_DEPRECATED=)
target_include_directories (pkgdata PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common"
                                            "${CMAKE_CURRENT_SOURCE_DIR}/tools/toolutil")
add_dependencies (pkgdata icutu icui18n icuuc icudata)
target_link_libraries (pkgdata PRIVATE icutu icui18n icuuc icudata)
if (UNIX)
    if (CMAKE_USE_PTHREADS_INIT)
        target_link_libraries (icupkg PRIVATE -lpthread)
    endif ()
    target_link_libraries (icupkg PRIVATE -lm)
endif ()

## icudata.lst custom target.
set (icudata_lst_output   "${CMAKE_CURRENT_BINARY_DIR}/icudata.lst")
set (icudata_dat_s_output "${CMAKE_CURRENT_BINARY_DIR}/out/tmp/icudt57l_dat.S")
add_custom_target (icudata_lst ALL DEPENDS "${icudata_lst_output}" ${icudata_dat_s_output})

add_custom_command (
        OUTPUT "${icudata_lst_output}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/out/build/icudt57l"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/out/tmp"
        COMMAND $<TARGET_FILE:icupkg> -d "${CMAKE_CURRENT_BINARY_DIR}/out/build/icudt57l"
                --list -x "\\*" "${CMAKE_CURRENT_SOURCE_DIR}/data/in/icudt57l.dat"
                -o "${CMAKE_CURRENT_BINARY_DIR}/out/tmp/icudata.lst"
        DEPENDS icupkg
        COMMENT "Unpacking icudt57l.dat and generating icudata.lst (list of data files)")

add_custom_command (
        OUTPUT "${icudata_dat_s_output}"
        COMMAND $<TARGET_FILE:pkgdata>
                -O "${CMAKE_CURRENT_SOURCE_DIR}/data/icupkg.inc"
                -q -c
                -s "${CMAKE_CURRENT_SOURCE_DIR}/data/out/build/icudt57l"
                -d "${CMAKE_CURRENT_BINARY_DIR}"
                -e icudt57
                -T "${CMAKE_CURRENT_BINARY_DIR}/out/tmp"
                -p icudt57l
                -m static
                -r 57.1
                -L icudata
                "${CMAKE_CURRENT_BINARY_DIR}/out/tmp/icudata.lst"
        DEPENDS pkgdata "${icudata_lst_output}")

## icu-dt57l
set (ICUDT57L_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/stubdata/stubdata.c"
                   "${icudata_dat_s_output}")
add_library (icudt57l STATIC ${ICUDT57L_SRCS})
target_compile_definitions (icudt57l PRIVATE -DU_ATTRIBUTE_DEPRECATED=)
target_include_directories (icudt57l PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common")

