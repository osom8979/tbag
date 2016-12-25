## CMake build script.

cmake_minimum_required (VERSION 2.8.8)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

enable_language (C)
enable_language (CXX)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}"
                     "${CMAKE_CURRENT_SOURCE_DIR}/include"
                     "${CMAKE_CURRENT_SOURCE_DIR}/grpc")
link_directories (${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Wall -pedantic -Werror -Wextra")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -Werror -Wextra")
endif ()

## flatbuffers (lib)
set (FLATBUFFERS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_parser.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_text.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/reflection.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/util.cpp")
add_library (flatbuffers STATIC ${FLATBUFFERS_SRCS})

## flatc (compiler)
set (FLATC_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_cpp.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_general.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_go.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_js.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_php.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_python.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_fbs.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_grpc.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/src/flatc.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/grpc/src/compiler/cpp_generator.cc")
add_executable (flatc ${FLATC_SRCS})
target_link_libraries (flatc PRIVATE flatbuffers)

## flathash
set(FLATHASH_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/flathash.cpp")
add_executable (flathash ${FLATHASH_SRCS})

