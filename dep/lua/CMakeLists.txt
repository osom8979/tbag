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

enable_language (C)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}")
link_directories (${CMAKE_CURRENT_SOURCE_DIR})

prepend (CORE_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src"
        lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c
        lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c
        ltm.c lundump.c lvm.c lzio.c)

prepend (LIB_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src"
        lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c
        lmathlib.c loslib.c lstrlib.c ltablib.c lutf8lib.c loadlib.c linit.c)

if (WIN32)
    add_definitions (-DLUA_BUILD_AS_DLL)
elseif (UNIX)
    if (APPLE)
        add_definitions (-DLUA_USE_MACOSX)
        #target_link_libraries (lua -lreadline)
    else ()
        add_definitions (-DLUA_USE_POSIX)
    endif ()
endif ()

add_library (lua STATIC ${CORE_SRCS} ${LIB_SRCS})

## If shared library & windows.
#target_compile_definitions (lua PRIVATE LUA_BUILD_AS_DLL)

