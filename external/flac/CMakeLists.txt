cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0005 NEW) # do not escape preprocessor defines

project(libflac)

if (NOT DEFINED LIBFLAC_ARCHITECTURE)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set (LIBFLAC_ARCHITECTURE x64)
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
        set (LIBFLAC_ARCHITECTURE x86)
    endif()
endif()

if(NOT DEFINED LIBFLAC_ARCHITECTURE)
    message(FATAL_ERROR "Target architecture not specified")
endif()

include (CheckIncludeFile)
check_include_file (sys/param.h HAVE_SYS_PARAM_H)
if (HAVE_SYS_PARAM_H)
    add_definitions(-DHAVE_SYS_PARAM_H)
endif ()

file(GLOB LIBFLAC_SOURCES src/libFLAC/*.c)

function (__remove_windows_files __result)
    set (${__result})
    foreach (__source_cursor ${ARGN})
        if (NOT "${__source_cursor}" MATCHES "windows[^/]*$")
            list (APPEND ${__result} "${__source_cursor}")
        endif ()
    endforeach ()
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

if(WIN32)
    list(APPEND LIBFLAC_SOURCES src/share/win_utf8_io/win_utf8_io.c)
else()
    if (CMAKE_VERSION VERSION_LESS "3.6")
        __remove_windows_files (LIBFLAC_SOURCES_TEMP ${LIBFLAC_SOURCES})
        set (LIBFLAC_SOURCES ${LIBFLAC_SOURCES_TEMP})
        unset (LIBFLAC_SOURCES_TEMP)
    else ()
        list(FILTER LIBFLAC_SOURCES EXCLUDE REGEX "windows[^/]*$")
    endif ()

    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/config.h "#include <inttypes.h>")
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    add_definitions(-DHAVE_CONFIG_H)
endif()

file(GLOB LIBFLACXX_SOURCES src/libFLAC++/*.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/libFLAC/include)

if(NOT LIBFLAC_ARCHITECTURE MATCHES arm)
    add_definitions(-DFLAC__SSE_OS)
    add_definitions(-DFLAC__HAS_X86INTRIN)
endif()

if(LIBFLAC_ARCHITECTURE MATCHES x86)
    add_definitions(-DFLAC__CPU_IA32)
    add_definitions(-DFLAC__HAS_NASM)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/nasm)
    foreach(ASM_SOURCE cpu_asm fixed_asm lpc_asm)
        execute_process(COMMAND nasm "-i${PROJECT_SOURCE_DIR}/src/libFLAC/ia32/" -f win32 -d OBJ_FORMAT_win32 -f win32 
            -o "${CMAKE_BINARY_DIR}/nasm/${ASM_SOURCE}.obj" "${PROJECT_SOURCE_DIR}/src/libFLAC/ia32/${ASM_SOURCE}.nasm"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        list(APPEND LIBFLAC_SOURCES ${CMAKE_BINARY_DIR}/nasm/${ASM_SOURCE}.obj)
    endforeach()
elseif(LIBFLAC_ARCHITECTURE MATCHES x64)
    add_definitions(-DFLAC__CPU_X86_64)
    add_definitions(-DENABLE_64_BIT_WORDS)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DFLAC__OVERFLOW_DETECT)
endif()

add_definitions(-DPACKAGE_VERSION="1.3.2")
add_definitions(-DFLAC__HAS_OGG)
add_definitions(-DFLAC__ALIGN_MALLOC_DATA) # 32 byte boundary
add_definitions(-DHAVE_LROUND)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)

add_library(FLAC ${LIBFLAC_SOURCES})
add_library(FLACXX ${LIBFLACXX_SOURCES})

#set_target_properties(FLAC PROPERTIES OUTPUT_NAME FLAC)
#set_target_properties(FLACXX PROPERTIES OUTPUT_NAME FLAC++)

if (NOT OGG_ROOT)
    find_library(OGG_LIBRARY ogg)
    find_path(OGG_INCLUDE_DIRS ogg/ogg.h)
else ()
    find_path (OGG_INCLUDE_DIRS NAMES ogg/ogg.h HINTS ${OGG_ROOT}/include PATH_SUFFIXES ogg)
    find_library (OGG_LIBRARIES NAMES ogg HINTS ${OGG_ROOT}/lib ${OGG_ROOT}/lib64)
endif ()

include_directories(${OGG_INCLUDE_DIRS})
target_link_libraries(FLAC ${OGG_LIBRARIES})
target_link_libraries(FLACXX FLAC)

if(BUILD_SHARED_LIBS)
    set_target_properties(FLAC   PROPERTIES COMPILE_FLAGS -DFLAC_API_EXPORTS)
    set_target_properties(FLACXX PROPERTIES COMPILE_FLAGS -DFLACPP_API_EXPORTS)
else()
    add_definitions(-DFLAC__NO_DLL)
endif()

install(TARGETS FLAC FLACXX
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)

if(NOT DEFINED LIBFLAC_SKIP_HEADERS)
    install(DIRECTORY include/FLAC/
        DESTINATION include/FLAC
        FILES_MATCHING PATTERN "*.h")
    install(DIRECTORY include/FLAC++/
        DESTINATION include/FLAC++
        FILES_MATCHING PATTERN "*.h")
endif()
