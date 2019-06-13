## CMake build script.

include (ExternalProject)

#/// Check exists libraries.
#///
#/// @param __result  [out] value name of output result.
#/// @param ...       [in]  Files.
function (exists_files __result)
    set (${__result} ON)

    foreach (__lib_cursor ${ARGN})
        if (NOT EXISTS "${__lib_cursor}")
            set (${__result} OFF)
            break ()
        endif ()
    endforeach ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// A fake function to inject dependencies into the output file of ExternalProject.
#///
#/// @param __name  [in] Print name.
#/// @param __deps  [in] Dependencies.
#/// @param ...     [in] Output files.
function (fake_output_library __name __deps)
    add_custom_command (OUTPUT  ${ARGN}
                        COMMAND ${CMAKE_COMMAND} -E echo "Fake output: ${__name}"
                        DEPENDS ${__deps})
endfunction ()

string (TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

if ("${CMAKE_BUILD_TYPE_LOWER}" STREQUAL "")
    set (_EXT_BUILD_TYPE Debug)
else ()
    set (_EXT_BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif ()

set (EXT_BUILD_TYPE  "${_EXT_BUILD_TYPE}"                   CACHE STRING "External build type.")
set (EXT_PREFIX_DIR  "${CMAKE_BINARY_DIR}/external/prefix"  CACHE STRING "External build prefix.")
set (EXT_INSTALL_DIR "${CMAKE_BINARY_DIR}/external/local"   CACHE STRING "External install prefix.")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set (EXT_CXX_FLAGS)
    set (EXT_C_FLAGS)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (EXT_CXX_FLAGS "-fPIC -w")
    set (EXT_C_FLAGS   "-fPIC -w")
endif ()

###########
## LIBUV ##
###########

set (uv_EXT_DEBUG_NAME)
set (uv_EXT_STATIC_LIB_NAME uv_a)

set (uv_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/uv")
set (uv_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (uv_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${uv_EXT_STATIC_LIB_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (uv_EXT_LIBRARIES    "${uv_EXT_STATIC_LIB}")
exists_files (uv_EXT_EXISTS ${uv_EXT_LIBRARIES})

if (uv_EXT_EXISTS)
    message (STATUS "Skip external/uv (Exists: ${uv_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/uv")
    ExternalProject_Add (uv_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Download step--------------
            #URL "https://dist.libuv.org/dist/v1.22.0/libuv-v1.22.0.tar.gz"
            #URL_MD5 "02ddfadb2a061618e87018a4e84610c0"
            #--Configure step-------------
            SOURCE_DIR "${uv_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DBUILD_TESTING=OFF"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (uv_ext_output uv_ext ${uv_EXT_LIBRARIES})
endif ()
add_custom_target (uv DEPENDS ${uv_EXT_LIBRARIES})

##########
## ZLIB ##
##########

set (zlib_EXT_DEBUG_NAME)
set (zlib_EXT_STATIC_LIB_NAME z)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set (zlib_EXT_STATIC_LIB_NAME zlibstatic)
    if (NOT "${CMAKE_BUILD_TYPE_LOWER}" STREQUAL "release")
        set (zlib_EXT_DEBUG_NAME d)
    endif ()
endif ()

set (zlib_EXT_VERSION_STR  "1.2.11")
set (zlib_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/zlib")
set (zlib_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (zlib_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${zlib_EXT_STATIC_LIB_NAME}${zlib_EXT_DEBUG_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (zlib_EXT_LIBRARIES    "${zlib_EXT_STATIC_LIB}")
exists_files (zlib_EXT_EXISTS ${zlib_EXT_LIBRARIES})

if (zlib_EXT_EXISTS)
    message (STATUS "Skip external/zlib (Exists: ${zlib_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/zlib")
    ExternalProject_Add (zlib_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Download step--------------
            #URL "http://www.zlib.net/zlib-1.2.11.tar.gz"
            #URL_MD5 "1c9f62f0778697a09d36121ead88e08e"
            #--Configure step-------------
            SOURCE_DIR "${zlib_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       #"-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}" ## [WARNING] Don't use this flag.
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (zlib_ext_output zlib_ext ${zlib_EXT_LIBRARIES})
endif ()
add_custom_target (zlib DEPENDS ${zlib_EXT_LIBRARIES})

##############
## LIBRESSL ##
##############

set (ressl_EXT_VERSION_STR       "2.5.1")
set (ressl_EXT_SOURCE_DIR        "${CMAKE_SOURCE_DIR}/external/ressl")
set (ressl_EXT_INCLUDE_DIR       "${EXT_INSTALL_DIR}/include")
set (ressl_crypto_EXT_STATIC_LIB "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}crypto${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (ressl_ssl_EXT_STATIC_LIB    "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ssl${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (ressl_tls_EXT_STATIC_LIB    "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}tls${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (ressl_EXT_LIBRARIES         "${ressl_crypto_EXT_STATIC_LIB}"
                                 "${ressl_ssl_EXT_STATIC_LIB}"
                                 "${ressl_tls_EXT_STATIC_LIB}")
exists_files (ressl_EXT_EXISTS ${ressl_EXT_LIBRARIES})

if (ressl_EXT_EXISTS)
    message (STATUS "Skip external/ressl_crypto (Exists: ${ressl_crypto_EXT_STATIC_LIB})")
    message (STATUS "Skip external/ressl_ssl (Exists: ${ressl_ssl_EXT_STATIC_LIB})")
    message (STATUS "Skip external/ressl_tls (Exists: ${ressl_tls_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/ressl")
    ExternalProject_Add (ressl_ext
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
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (ressl_ext_output ressl_ext ${ressl_EXT_LIBRARIES})
endif ()
add_custom_target (ressl DEPENDS ${ressl_EXT_LIBRARIES})

##############
## FREETYPE ##
##############

set (freetype_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/freetype")
set (freetype_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include/freetype2")
set (freetype_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}freetype${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (freetype_EXT_LIBRARIES    "${freetype_EXT_STATIC_LIB}")
exists_files (freetype_EXT_EXISTS ${freetype_EXT_LIBRARIES})

if (freetype_EXT_EXISTS)
    message (STATUS "Skip external/freetype (Exists: ${freetype_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/freetype")
    ExternalProject_Add (freetype_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${freetype_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DCMAKE_INSTALL_LIBDIR=lib"
                       "-DCMAKE_INSTALL_INCLUDEDIR=include"
                       "-DDISABLE_FORCE_DEBUG_POSTFIX=ON"
                       "-DBUILD_FRAMEWORK=OFF"
                       "-DFT_WITH_HARFBUZZ=OFF"
                       "-DFT_WITH_PNG=OFF"
                       "-DFT_WITH_ZLIB=OFF"
                       "-DFT_WITH_BZIP2=OFF"
            #--Output lfreetypeing-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (freetype_ext_output freetype_ext ${freetype_EXT_LIBRARIES})
endif ()
add_custom_target (freetype DEPENDS ${freetype_EXT_LIBRARIES})

############
## LUAJIT ##
############

set (luajit_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/luajit")
set (luajit_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (luajit_EXT_CONF_HEADER  "${luajit_EXT_INCLUDE_DIR}/luaconf.h")
set (luajit_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lua${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (luajit_EXT_LIBRARIES    "${luajit_EXT_STATIC_LIB}")
exists_files (luajit_EXT_EXISTS ${luajit_EXT_LIBRARIES} ${luajit_EXT_CONF_HEADER})

if (luajit_EXT_EXISTS)
    message (STATUS "Skip external/luajit (Exists: ${luajit_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/luajit")
    ExternalProject_Add (luajit_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${luajit_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DLUA_USE_READLINE=OFF"
                       "-DLUA_USE_CURSES=OFF"
                       "-DLUA_BUILD_WLUA=OFF"
                       "-DLUA_PATH=LUA_PATH"
                       "-DLUA_CPATH=LUA_CPATH"
                       "-DLUA_INIT=LUA_INIT"
                       "-DLUA_USE_RELATIVE_LOADLIB=OFF"
                       "-DLUA_DIR="
            #--Output lluajiting-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (luajit_ext_output luajit_ext ${luajit_EXT_LIBRARIES})
endif ()
add_custom_target (luajit DEPENDS ${luajit_EXT_LIBRARIES})

###########
## BZIP2 ##
###########

set (bzip2_EXT_VERSION_STR  "1.0.5")
set (bzip2_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/bzip2")
set (bzip2_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (bzip2_EXT_HEADER       "${bzip2_EXT_INCLUDE_DIR}/bzlib.h")
set (bzip2_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cmbzip2${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (bzip2_EXT_LIBRARIES    "${bzip2_EXT_STATIC_LIB}")
exists_files (bzip2_EXT_EXISTS ${bzip2_EXT_LIBRARIES} ${bzip2_EXT_HEADER})

if (bzip2_EXT_EXISTS)
    message (STATUS "Skip external/bzip2 (Exists: ${bzip2_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/bzip2")
    ExternalProject_Add (bzip2_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${bzip2_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output lbzip2ing-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (bzip2_ext_output bzip2_ext ${bzip2_EXT_LIBRARIES})
endif ()
add_custom_target (bzip2 DEPENDS ${bzip2_EXT_LIBRARIES})

##########
## LZMA ##
##########

set (lzma_EXT_VERSION_STR  "5.2.4")
set (lzma_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/lzma")
set (lzma_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (lzma_EXT_HEADER       "${lzma_EXT_INCLUDE_DIR}/lzma.h")
set (lzma_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cmliblzma${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (lzma_EXT_LIBRARIES    "${lzma_EXT_STATIC_LIB}"
                           "${lzma_EXT_HEADER}")
exists_files (lzma_EXT_EXISTS ${lzma_EXT_LIBRARIES})

if (lzma_EXT_EXISTS)
    message (STATUS "Skip external/lzma (Exists: ${lzma_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/lzma")
    ExternalProject_Add (lzma_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${lzma_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output llzmaing-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (lzma_ext_output lzma_ext ${lzma_EXT_LIBRARIES})
endif ()
add_custom_target (lzma DEPENDS ${lzma_EXT_LIBRARIES})

#############
## ARCHIVE ##
#############

set (archive_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/archive")
set (archive_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (archive_EXT_HEADER       "${archive_EXT_INCLUDE_DIR}/archive.h")
set (archive_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}cmlibarchive${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (archive_EXT_LIBRARIES    "${archive_EXT_STATIC_LIB}"
                              "${archive_EXT_HEADER}")
exists_files (archive_EXT_EXISTS ${archive_EXT_LIBRARIES})

if (archive_EXT_EXISTS)
    message (STATUS "Skip external/archive (Exists: ${archive_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/archive")
    ExternalProject_Add (archive_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${archive_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DENABLE_NETTLE=OFF"
                       "-DENABLE_OPENSSL=ON"
                       "-DENABLE_LZ4=OFF"
                       "-DENABLE_LZO=OFF"
                       "-DENABLE_LZMA=ON"
                       "-DENABLE_ZLIB=ON"
                       "-DENABLE_BZip2=ON"
                       "-DENABLE_LIBXML2=OFF"
                       "-DENABLE_EXPAT=OFF"
                       "-DENABLE_CNG=OFF" ## [IMPORTANT] Don't use the CNG(Crypto Next Generation) features.
                       #"-DENABLE_XATTR=ON"
                       #"-DENABLE_ACL=ON"
                       "-DENABLE_ICONV=OFF"
                       "-DBZIP2_FOUND=ON"
                       "-DBZIP2_INCLUDE_DIR=${bzip2_EXT_INCLUDE_DIR}"
                       "-DBZIP2_LIBRARIES=${bzip2_EXT_STATIC_LIB}"
                       "-DBZIP2_VERSION_STRING=${bzip2_EXT_VERSION_STR}"
                       "-DZLIB_FOUND=ON"
                       "-DZLIB_INCLUDE_DIRS=${zlib_EXT_INCLUDE_DIR}"
                       "-DZLIB_LIBRARIES=${zlib_EXT_STATIC_LIB}"
                       "-DZLIB_VERSION_STRING=${zlib_EXT_VERSION_STR}"
                       "-DLIBLZMA_FOUND=ON"
                       "-DLIBLZMA_INCLUDE_DIRS=${lzma_EXT_INCLUDE_DIR}"
                       "-DLIBLZMA_LIBRARIES=${lzma_EXT_STATIC_LIB}"
                       "-DLIBLZMA_VERSION_STRING=${lzma_EXT_VERSION_STR}"
                       "-DOPENSSL_FOUND=ON"
                       "-DOPENSSL_INCLUDE_DIR=${ressl_EXT_INCLUDE_DIR}"
                       "-DOPENSSL_CRYPTO_LIBRARY=${ressl_crypto_EXT_STATIC_LIB}"
                       "-DOPENSSL_SSL_LIBRARY=${ressl_ssl_EXT_STATIC_LIB}"
                       "-DOPENSSL_LIBRARIES=${ressl_EXT_LIBRARIES}"
                       "-DOPENSSL_VERSION=${ressl_EXT_VERSION_STR}"
                       "-DHAVE_LIBCRYPTO=1"
                       "-DHAVE_PKCS5_PBKDF2_HMAC_SHA1=1"
            #--Output larchiveing-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1
            DEPENDS zlib bzip2 lzma ressl)
    fake_output_library (archive_ext_output archive_ext ${archive_EXT_LIBRARIES})
endif ()
add_custom_target (archive DEPENDS ${archive_EXT_LIBRARIES})

#########
## NNG ##
#########

set (nng_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/nng")
set (nng_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (nng_EXT_HEADER       "${nng_EXT_INCLUDE_DIR}/nng/nng.h")
set (nng_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}nng${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (nng_EXT_LIBRARIES    "${nng_EXT_STATIC_LIB}")
exists_files (nng_EXT_EXISTS ${nng_EXT_LIBRARIES} ${nng_EXT_HEADER})

if (nng_EXT_EXISTS)
    message (STATUS "Skip external/nng (Exists: ${nng_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/nng")
    ExternalProject_Add (nng_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${nng_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DNNG_ENABLE_TLS=OFF"
                       "-DNNG_TESTS=OFF"
                       "-DNNG_TOOLS=OFF"
                       "-DNNG_ENABLE_STATS=ON"
                       "-DNNG_ENABLE_NNGCAT=OFF"
                       "-DNNG_ENABLE_COVERAGE=OFF"
            #--Output lnnging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (nng_ext_output nng_ext ${nng_EXT_LIBRARIES})
endif ()
add_custom_target (nng DEPENDS ${nng_EXT_LIBRARIES})

##########
## GLFW ##
##########

set (glfw_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/glfw")
set (glfw_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (glfw_EXT_HEADER       "${glfw_EXT_INCLUDE_DIR}/GLFW/glfw3.h")
set (glfw_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (glfw_EXT_LIBRARIES    "${glfw_EXT_STATIC_LIB}")
exists_files (glfw_EXT_EXISTS ${glfw_EXT_LIBRARIES} ${glfw_EXT_HEADER})

if (glfw_EXT_EXISTS)
    message (STATUS "Skip external/glfw (Exists: ${glfw_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/glfw")
    ExternalProject_Add (glfw_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${glfw_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DGLFW_BUILD_EXAMPLES=OFF"
                       "-DGLFW_BUILD_TESTS=OFF"
                       "-DGLFW_BUILD_DOCS=OFF"
                       "-DGLFW_INSTALL=ON"
                       "-DGLFW_VULKAN_STATIC=OFF"
            #--Output lglfwing-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (glfw_ext_output glfw_ext ${glfw_EXT_LIBRARIES})
endif ()
add_custom_target (glfw DEPENDS ${glfw_EXT_LIBRARIES})

#############
## SQLITE3 ##
#############

set (sqlite3_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/sqlite3")
set (sqlite3_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (sqlite3_EXT_HEADER       "${sqlite3_EXT_INCLUDE_DIR}/sqlite3.h")
set (sqlite3_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sqlite3${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (sqlite3_EXT_LIBRARIES    "${sqlite3_EXT_STATIC_LIB}")
exists_files (sqlite3_EXT_EXISTS ${sqlite3_EXT_LIBRARIES} ${sqlite3_EXT_HEADER})

if (sqlite3_EXT_EXISTS)
    message (STATUS "Skip external/sqlite3 (Exists: ${sqlite3_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/sqlite3")
    ExternalProject_Add (sqlite3_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${sqlite3_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output lsqlite3ing-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (sqlite3_ext_output sqlite3_ext ${sqlite3_EXT_LIBRARIES})
endif ()
add_custom_target (sqlite3 DEPENDS ${sqlite3_EXT_LIBRARIES})

##########
## DATE ##
##########

set (date_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/date")
set (date_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (date_EXT_HEADER       "${date_EXT_INCLUDE_DIR}/date/date.h")
set (date_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}tz${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (date_EXT_LIBRARIES    "${date_EXT_STATIC_LIB}")
exists_files (date_EXT_EXISTS ${date_EXT_LIBRARIES} ${date_EXT_HEADER})

if (date_EXT_EXISTS)
    message (STATUS "Skip external/date (Exists: ${date_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/date")
    ExternalProject_Add (date_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${date_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DUSE_SYSTEM_TZ_DB=ON"
                       "-DUSE_TZ_DB_IN_DOT=OFF"
                       "-DENABLE_DATE_TESTING=OFF"
                       "-DDISABLE_STRING_VIEW=OFF"
            #--Output ldateing-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (date_ext_output date_ext ${date_EXT_LIBRARIES})
endif ()
add_custom_target (date DEPENDS ${date_EXT_LIBRARIES})


