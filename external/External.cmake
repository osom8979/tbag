## CMake build script.

include (ExternalProject)

#/// Check exists libraries.
#///
#/// @param __result  [out] value name of output result.
#/// @param __libs    [in]  Libraries.
function (exists_libraries __result __libs)
    set (${__result} ON)

    foreach (__lib_cursor ${__libs})
        if (NOT EXISTS "${__lib_cursor}")
            set (${__result} OFF)
            break ()
        endif ()
    endforeach ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

set (EXT_PREFIX_DIR  "${CMAKE_BINARY_DIR}/external/prefix")
set (EXT_INSTALL_DIR "${CMAKE_BINARY_DIR}/external/local")

string (TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

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

set (zlib_EXT_DEBUG_NAME)
set (zlib_EXT_STATIC_LIB_NAME z)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set (zlib_EXT_STATIC_LIB_NAME zlibstatic)
    if (NOT CMAKE_BUILD_TYPE_LOWER STREQUAL "release")
        set (zlib_EXT_DEBUG_NAME d)
    endif ()
endif ()

set (zlib_EXT_SOURCE_DIR  "${CMAKE_SOURCE_DIR}/external/zlib")
set (zlib_EXT_INCLUDE_DIR "${EXT_INSTALL_DIR}/include")
set (zlib_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${zlib_EXT_STATIC_LIB_NAME}${zlib_EXT_DEBUG_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (zlib_EXT_LIBRARIES   "${zlib_EXT_STATIC_LIB}")
exists_libraries (zlib_EXT_EXISTS "${zlib_EXT_LIBRARIES}")

if (zlib_EXT_EXISTS)
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

###############
## CAPNPROTO ##
###############

set (capnp_EXT_SOURCE_DIR    "${CMAKE_SOURCE_DIR}/external/capnproto/c++")
set (capnp_EXT_INCLUDE_DIR   "${EXT_INSTALL_DIR}/include")
set (capnp_EXT_STATIC_LIB    "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}capnp${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (capnp_kj_EXT_STATIC_LIB "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}kj${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (capnp_EXT_LIBRARIES     "${capnp_EXT_STATIC_LIB}")
exists_libraries (capnp_EXT_EXISTS "${capnp_EXT_LIBRARIES}")

if (capnp_EXT_EXISTS)
    add_custom_target (capnp)
    message (STATUS "Skip external/capnp (Exists: ${capnp_EXT_STATIC_LIB})")
    message (STATUS "Skip external/kj (Exists: ${capnp_kj_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/capnp")
    ExternalProject_Add (capnp
            PREFIX "${EXT_PREFIX_DIR}"
            #--Download step--------------
            #URL "https://codeload.github.com/sandstorm-io/capnproto/tar.gz/v0.5.3"
            #URL_MD5 "909bd13ad6b8bc840ac78ab8f5bcb0a4"
            #--Configure step-------------
            SOURCE_DIR "${capnp_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DBUILD_TESTING=OFF"
                       "-DCAPNP_LITE=ON"
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

##############
## LIBRESSL ##
##############

set (ressl_EXT_SOURCE_DIR        "${CMAKE_SOURCE_DIR}/external/ressl")
set (ressl_EXT_INCLUDE_DIR       "${EXT_INSTALL_DIR}/include")
set (ressl_crypto_EXT_STATIC_LIB "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}crypto${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (ressl_ssl_EXT_STATIC_LIB    "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ssl${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (ressl_tls_EXT_STATIC_LIB    "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}tls${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (ressl_EXT_LIBRARIES         "${ressl_crypto_EXT_STATIC_LIB}"
                                 "${ressl_ssl_EXT_STATIC_LIB}"
                                 "${ressl_tls_EXT_STATIC_LIB}")
exists_libraries (ressl_EXT_EXISTS "${ressl_EXT_LIBRARIES}")

if (ressl_EXT_EXISTS)
    add_custom_target (ressl)
    message (STATUS "Skip external/ressl_crypto (Exists: ${ressl_crypto_EXT_STATIC_LIB})")
    message (STATUS "Skip external/ressl_ssl (Exists: ${ressl_ssl_EXT_STATIC_LIB})")
    message (STATUS "Skip external/ressl_tls (Exists: ${ressl_tls_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/ressl")
    ExternalProject_Add (ressl
            PREFIX "${EXT_PREFIX_DIR}"
            #--Download step--------------
            #URL "https://ftp.openbsd.org/pub/OpenBSD/LibreSSL/libressl-2.5.1.tar.gz"
            #URL_MD5 "1ff612428af67c0acdd61f8079c43555"
            #--Configure step-------------
            SOURCE_DIR "${ressl_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       #"-DCMAKE_C_FLAGS=${EXT_C_FLAGS}" ## [WARNING] Don't use this flag.
                       #"-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}" ## [WARNING] Don't use this flag.
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

