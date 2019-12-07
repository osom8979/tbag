cmake_minimum_required (VERSION 2.8.8)

project (flatbuffers CXX)

if (POLICY CMP0046)
    cmake_policy (SET CMP0042 NEW)
endif ()
if (POLICY CMP0054)
    cmake_policy (SET CMP0054 NEW)
endif ()

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}"
                     "${CMAKE_CURRENT_SOURCE_DIR}/include"
                     "${CMAKE_CURRENT_SOURCE_DIR}/grpc")
link_directories (${CMAKE_CURRENT_SOURCE_DIR})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # EMPTY.
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -Werror -Wextra -stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -Werror -Wextra")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wimplicit-fallthrough=0")
    endif ()
endif ()

## flatbuffers (lib)
set (FLATBUFFERS_SRCS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/code_generators.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_parser.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_text.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/reflection.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/util.cpp")
add_library (flatbuffers ${FLATBUFFERS_SRCS})

## flathash
set (FLATHASH_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/flathash.cpp")
add_executable (flathash ${FLATHASH_SRCS})

## flatc (compiler)
set (FLATC_SRCS
        #"${CMAKE_CURRENT_SOURCE_DIR}/src/code_generators.cpp"
        #"${CMAKE_CURRENT_SOURCE_DIR}/src/idl_parser.cpp"
        #"${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_text.cpp"
        #"${CMAKE_CURRENT_SOURCE_DIR}/src/reflection.cpp"
        #"${CMAKE_CURRENT_SOURCE_DIR}/src/util.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_cpp.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_general.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_go.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_js.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_php.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_python.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_fbs.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_grpc.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_dart.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_json_schema.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_lobster.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_lua.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/idl_gen_rust.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/flatc.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/flatc_main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/grpc/src/compiler/cpp_generator.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/grpc/src/compiler/go_generator.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/grpc/src/compiler/java_generator.cc")
add_executable (flatc ${FLATC_SRCS})
target_link_libraries (flatc PRIVATE flatbuffers)

install (TARGETS flatbuffers flatc flathash
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)
install (DIRECTORY "${CMAKE_SOURCE_DIR}/include/flatbuffers"
         DESTINATION include)

