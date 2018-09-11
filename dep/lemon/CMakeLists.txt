## CMake build script.
cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

project (LEMON)
add_definitions (-DLEMON_VERSION=1.3.1)

include (CheckTypeSize)
check_type_size ("long long" LONG_LONG)
if (HAVE_LONG_LONG)
    add_definitions (-DLEMON_HAVE_LONG_LONG=1)
endif ()

include (FindThreads)

if (NOT LEMON_THREADING)
    if (CMAKE_USE_PTHREADS_INIT)
        add_definitions (-DLEMON_USE_PTHREAD=1)
    elseif (CMAKE_USE_WIN32_THREADS_INIT)
        add_definitions (-DLEMON_USE_WIN32_THREADS=1)
    endif ()
endif ()

set (LEMON_SRCS src/arg_parser.cc
                src/base.cc
                src/color.cc
                src/lp_base.cc
                src/lp_skeleton.cc
                src/random.cc
                src/bits/windows.cc)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}")
link_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_library (lemon STATIC ${LEMON_SRCS})

