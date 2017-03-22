## CMake build script.

include (ExternalProject)

set (EXT_PREFIX_DIR  "${CMAKE_BINARY_DIR}/external/prefix")
set (EXT_INSTALL_DIR "${CMAKE_BINARY_DIR}/external/local")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set (EXT_CXX_FLAGS)
    set (EXT_C_FLAGS)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (EXT_CXX_FLAGS "-w -fPIC")
    set (EXT_C_FLAGS   "-w -fPIC")
endif ()


##########
## ZLIB ##
##########

set (zlib_EXT_SOURCE_DIR  "${CMAKE_SOURCE_DIR}/external/zlib")
set (zlib_EXT_INCLUDE_DIR "${EXT_INSTALL_DIR}/include")
set (zlib_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (zlib_EXT_LIBRARIES   "${zlib_EXT_STATIC_LIB}")

if (EXISTS "${zlib_EXT_STATIC_LIB}")
    add_custom_target (zlib)
    message (STATUS "Skip external/zlib (Exists: ${zlib_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/zlib")
    ExternalProject_Add (zlib
            PREFIX "${EXT_PREFIX_DIR}"
            #--Download step--------------
            #URL "http://www.zlib.net/zlib-1.2.11.tar.gz"
            #URL_MD5 "1c9f62f0778697a09d36121ead88e08e"
            #--Configure step-------------
            SOURCE_DIR "${zlib_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
endif ()


