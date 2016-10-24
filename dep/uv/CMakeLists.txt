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

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/src"
                     "${CMAKE_CURRENT_SOURCE_DIR}/include"
                     "${CMAKE_CURRENT_SOURCE_DIR}")

prepend (COMMON_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src"
        fs-poll.c inet.c threadpool.c uv-common.c version.c)

if (WIN32 AND (CMAKE_CXX_COMPILER_ID STREQUAL MSVC))
    ## Windows
    prepend (WIN_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/win"
            async.c core.c dl.c error.c fs-event.c fs.c getaddrinfo.c getnameinfo.c handle.c
            loop-watcher.c pipe.c poll.c process-stdio.c process.c req.c signal.c stream.c
            tcp.c thread.c timer.c tty.c udp.c util.c winapi.c winsock.c)
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/src/win")
    add_definitions (-DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0600 -D_GNU_SOURCE
                     -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
    add_compile_options (/wd4244) ## conversion from 'type1' to 'type2', possible loss of data.
    set (ALL_SRCS ${COMMON_SRCS} ${WIN_SRCS})

elseif (UNIX)
    ## Unix-like systems
    prepend (UNIX_COMMON_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/unix"
            async.c core.c dl.c fs.c getaddrinfo.c getnameinfo.c loop-watcher.c loop.c pipe.c
            poll.c process.c signal.c stream.c tcp.c thread.c timer.c tty.c udp.c)
    include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/src/unix")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu89 -pedantic -Wall -Wextra -Wno-unused-parameter")

    include (CheckIncludeFile)
    check_include_and_define (sys/types.h HAVE_SYS_TYPES_H)
    check_include_and_define (sys/stat.h  HAVE_SYS_STAT_H)
    check_include_and_define (stdlib.h    HAVE_STDLIB_H)
    check_include_and_define (string.h    HAVE_STRING_H)
    check_include_and_define (memory.h    HAVE_MEMORY_H)
    check_include_and_define (strings.h   HAVE_STRINGS_H)
    check_include_and_define (inttypes.h  HAVE_INTTYPES_H)
    check_include_and_define (stdint.h    HAVE_STDINT_H)
    check_include_and_define (unistd.h    HAVE_UNISTD_H)
    check_include_and_define (dlfcn.h     HAVE_DLFCN_H)

    if (APPLE)
        ## MacOSX
        prepend (OSX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/unix"
                darwin.c darwin-proctitle.c fsevents.c kqueue.c proctitle.c pthread-barrier.c)
        add_definitions (-D_DARWIN_USE_64_BIT_INODE=1 -D_DARWIN_UNLIMITED_SELECT=1)
        set (ALL_SRCS ${COMMON_SRCS} ${UNIX_COMMON_SRCS} ${OSX_SRCS})

    else ()
        ## Linux
        prepend (LINUX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/unix"
                linux-core.c linux-inotify.c linux-syscalls.c proctitle.c)
        add_definitions (-D_GNU_SOURC)
        set (ALL_SRCS ${COMMON_SRCS} ${UNIX_COMMON_SRCS} ${LINUX_SRCS})

    endif ()
endif ()

add_library (uv STATIC ${ALL_SRCS})

