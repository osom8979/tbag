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
            message (STATUS "Not found ${__lib_cursor}")
        endif ()
    endforeach ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Print a file that does not exist.
#///
#/// @param __target  [in] Target name.
#/// @param ...       [in] Files.
function (print_not_exists_files __target)
    foreach (__lib_cursor ${ARGN})
        if (NOT EXISTS "${__lib_cursor}")
            message (STATUS "Skip external/${__target} (Exists: ${__lib_cursor})")
        endif ()
    endforeach ()
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
    set (EXT_CXX_FLAGS "-fPIC -w -fvisibility=hidden")
    set (EXT_C_FLAGS   "-fPIC -w -fvisibility=hidden")
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

if (NOT uv_EXT_EXISTS)
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

if (NOT zlib_EXT_EXISTS)
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

#############
## minizip ##
#############

set (minizip_EXT_SOURCE_DIR  "${CMAKE_SOURCE_DIR}/external/minizip")
set (minizip_EXT_INCLUDE_DIR "${EXT_INSTALL_DIR}/include")
set (minizip_EXT_HEADER      "${minizip_EXT_INCLUDE_DIR}/zip.h")
set (minizip_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}minizip${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (minizip_EXT_LIBRARIES   "${minizip_EXT_STATIC_LIB}")
exists_files (minizip_EXT_EXISTS ${minizip_EXT_HEADER} ${minizip_EXT_LIBRARIES})

if (NOT minizip_EXT_EXISTS)
    message (STATUS "Add external/minizip")
    ExternalProject_Add (minizip_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${minizip_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DZLIB_INCLUDE_DIR=${zlib_EXT_INCLUDE_DIR}"
                       "-DZLIB_STATIC_LIB=${zlib_EXT_STATIC_LIB}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1
            DEPENDS zlib)
fake_output_library (minizip_ext_output minizip_ext ${minizip_EXT_LIBRARIES})
endif ()
add_custom_target (minizip DEPENDS ${minizip_EXT_LIBRARIES})

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

if (NOT ressl_EXT_EXISTS)
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

if (NOT freetype_EXT_EXISTS)
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
            #--Output logging-------------
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

if (NOT luajit_EXT_EXISTS)
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
            #--Output logging-------------
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

if (NOT bzip2_EXT_EXISTS)
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
            #--Output logging-------------
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

if (NOT lzma_EXT_EXISTS)
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
            #--Output logging-------------
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

if (NOT archive_EXT_EXISTS)
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
            #--Output logging-------------
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

if (NOT nng_EXT_EXISTS)
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
            #--Output logging-------------
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

if (NOT glfw_EXT_EXISTS)
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
            #--Output logging-------------
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

if (NOT sqlite3_EXT_EXISTS)
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
            #--Output logging-------------
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

if (NOT date_EXT_EXISTS)
    message (STATUS "Add external/date")
    ExternalProject_Add (date_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${date_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DUSE_SYSTEM_TZ_DB=ON"
                       "-DUSE_TZ_DB_IN_DOT=OFF"
                       "-DENABLE_DATE_TESTING=OFF"
                       "-DDISABLE_STRING_VIEW=OFF"
                       "-DCMAKE_CXX_STANDARD=14"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (date_ext_output date_ext ${date_EXT_LIBRARIES})
endif ()
add_custom_target (date DEPENDS ${date_EXT_LIBRARIES})

##########
## LMDB ##
##########

set (lmdb_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/lmdb")
set (lmdb_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (lmdb_EXT_HEADER       "${lmdb_EXT_INCLUDE_DIR}/lmdb.h")
set (lmdb_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lmdb${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (lmdb_EXT_LIBRARIES    "${lmdb_EXT_STATIC_LIB}")
exists_files (lmdb_EXT_EXISTS ${lmdb_EXT_LIBRARIES} ${lmdb_EXT_HEADER})

if (NOT lmdb_EXT_EXISTS)
    message (STATUS "Add external/lmdb")
    ExternalProject_Add (lmdb_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${lmdb_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (lmdb_ext_output lmdb_ext ${lmdb_EXT_LIBRARIES})
endif ()
add_custom_target (lmdb DEPENDS ${lmdb_EXT_LIBRARIES})

##############
## CIVETWEB ##
##############

set (civetweb_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/civetweb")
set (civetweb_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (civetweb_EXT_HEADER       "${civetweb_EXT_INCLUDE_DIR}/civetweb.h")
set (civetweb_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}civetweb${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (civetweb_EXT_LIBRARIES    "${civetweb_EXT_STATIC_LIB}")
exists_files (civetweb_EXT_EXISTS ${civetweb_EXT_LIBRARIES} ${civetweb_EXT_HEADER})

if (NOT civetweb_EXT_EXISTS)
    message (STATUS "Add external/civetweb")
    ExternalProject_Add (civetweb_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${civetweb_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS} -I${ressl_EXT_INCLUDE_DIR} -D'DEBUG_TRACE(...)='"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DCIVETWEB_BUILD_TESTING=OFF"
                       "-DCIVETWEB_ENABLE_THIRD_PARTY_OUTPUT=OFF"
                       "-DCIVETWEB_THREAD_STACK_SIZE=102400"
                       "-DCIVETWEB_ENABLE_SERVER_EXECUTABLE=OFF"
                       "-DCIVETWEB_SERVE_NO_FILES=OFF"
                       "-DCIVETWEB_DISABLE_CGI=OFF"
                       "-DCIVETWEB_DISABLE_CACHING=OFF"
                       "-DCIVETWEB_ENABLE_CXX=OFF"
                       "-DCIVETWEB_ENABLE_IPV6=OFF"
                       "-DCIVETWEB_ENABLE_WEBSOCKETS=ON"
                       "-DCIVETWEB_ENABLE_SERVER_STATS=OFF"
                       "-DCIVETWEB_ENABLE_MEMORY_DEBUGGING=OFF"
                       "-DCIVETWEB_ENABLE_ASAN=OFF"
                       "-DCIVETWEB_ARCH=OFF"
                       "-DCIVETWEB_ENABLE_LUA=OFF"
                       "-DCIVETWEB_INSTALL_EXECUTABLE=OFF"
                       "-DCIVETWEB_ALLOW_WARNINGS=ON"
                       "-DCIVETWEB_ENABLE_SSL_DYNAMIC_LOADING=OFF"
                       "-DCIVETWEB_ENABLE_SSL=ON"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1
            DEPENDS zlib ressl)
    fake_output_library (civetweb_ext_output civetweb_ext ${civetweb_EXT_LIBRARIES})
endif ()
add_custom_target (civetweb DEPENDS ${civetweb_EXT_LIBRARIES})

######################
## Blend2d + asmjit ##
######################

set (asmjit_EXT_SOURCE_DIR    "${CMAKE_SOURCE_DIR}/external/asmjit")
set (blend2d_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/blend2d")
set (blend2d_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (blend2d_EXT_HEADER       "${blend2d_EXT_INCLUDE_DIR}/blend2d.h")
set (blend2d_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}blend2d${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (blend2d_EXT_LIBRARIES    "${blend2d_EXT_STATIC_LIB}")
exists_files (blend2d_EXT_EXISTS ${blend2d_EXT_LIBRARIES} ${blend2d_EXT_HEADER})

if (NOT blend2d_EXT_EXISTS)
    message (STATUS "Add external/blend2d")
    ExternalProject_Add (blend2d_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${blend2d_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DASMJIT_DIR=${asmjit_EXT_SOURCE_DIR}"
                       "-DBLEND2D_DIR=${blend2d_EXT_SOURCE_DIR}"
                       "-DBLEND2D_TEST=OFF"
                       "-DBLEND2D_STATIC=ON"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (blend2d_ext_output blend2d_ext ${blend2d_EXT_LIBRARIES})
endif ()
add_custom_target (blend2d DEPENDS ${blend2d_EXT_LIBRARIES})

##############
## OpenBLAS ##
##############

set (openblas_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/openblas")
set (openblas_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (openblas_EXT_HEADER       "${openblas_EXT_INCLUDE_DIR}/openblas/cblas.h")
set (openblas_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}openblas${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (openblas_EXT_LIBRARIES    "${openblas_EXT_STATIC_LIB}")
exists_files (openblas_EXT_EXISTS ${openblas_EXT_LIBRARIES} ${openblas_EXT_HEADER})

if (NOT openblas_EXT_EXISTS)
    message (STATUS "Add external/openblas")
    ExternalProject_Add (openblas_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${openblas_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS} -DDllMain=openblas_DllMain"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DNOFORTRAN=ON"
                       "-DBUILD_WITHOUT_LAPACK=ON"
                       "-DBUILD_WITHOUT_CBLAS=OFF"
                       "-DDYNAMIC_ARCH=OFF"
                       "-DDYNAMIC_OLDER=OFF"
                       "-DBUILD_RELAPACK=OFF"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (openblas_ext_output openblas_ext ${openblas_EXT_LIBRARIES})
endif ()
add_custom_target (openblas DEPENDS ${openblas_EXT_LIBRARIES})

###########
## ICU4C ##
###########

set (icu_EXT_SOURCE_DIR      "${CMAKE_SOURCE_DIR}/external/icu")
set (icuuc_EXT_INCLUDE_DIR   "${EXT_INSTALL_DIR}/include/icu/common")
set (icui18n_EXT_INCLUDE_DIR "${EXT_INSTALL_DIR}/include/icu/i18n")
set (icu_EXT_HEADER          "${icuuc_EXT_INCLUDE_DIR}/unicode/uconfig.h")
set (icuuc_EXT_STATIC_LIB    "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}icuuc${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (icui18n_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}icui18n${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (icu_EXT_LIBRARIES       "${icuuc_EXT_STATIC_LIB}"
                             "${icui18n_EXT_STATIC_LIB}")
exists_files (icu_EXT_EXISTS ${icu_EXT_LIBRARIES} ${icu_EXT_HEADER})

if (NOT icu_EXT_EXISTS)
    message (STATUS "Add external/icu")
    ExternalProject_Add (icu_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${icu_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (icu_ext_output icu_ext ${icu_EXT_LIBRARIES})
endif ()
add_custom_target (icu DEPENDS ${icu_EXT_LIBRARIES})

##############
## demangle ##
##############

set (demangle_EXT_SOURCE_DIR  "${CMAKE_SOURCE_DIR}/external/demangle")
set (demangle_EXT_INCLUDE_DIR "${EXT_INSTALL_DIR}/include")
set (demangle_EXT_HEADER      "${demangle_EXT_INCLUDE_DIR}/demangle.h")
set (demangle_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}demangle${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (demangle_EXT_LIBRARIES   "${demangle_EXT_STATIC_LIB}")
exists_files (demangle_EXT_EXISTS ${demangle_EXT_HEADER} ${demangle_EXT_LIBRARIES})

if (NOT demangle_EXT_EXISTS)
    message (STATUS "Add external/demangle")
    ExternalProject_Add (demangle_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${demangle_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (demangle_ext_output demangle_ext ${demangle_EXT_LIBRARIES})
endif ()
add_custom_target (demangle DEPENDS ${demangle_EXT_LIBRARIES})

#################
## FlatBuffers ##
#################

set (flatbuffers_EXT_SOURCE_DIR  "${CMAKE_SOURCE_DIR}/external/flatbuffers")
set (flatbuffers_EXT_BIN_DIR     "${EXT_INSTALL_DIR}/bin")
set (flatbuffers_EXT_COMPILER    "${flatbuffers_EXT_BIN_DIR}/flatc${CMAKE_EXECUTABLE_SUFFIX}")
set (flatbuffers_EXT_HASH        "${flatbuffers_EXT_BIN_DIR}/flathash${CMAKE_EXECUTABLE_SUFFIX}")
set (flatbuffers_EXT_INCLUDE_DIR "${EXT_INSTALL_DIR}/include")
set (flatbuffers_EXT_HEADER      "${flatbuffers_EXT_INCLUDE_DIR}/flatbuffers/flatbuffers.h")
set (flatbuffers_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}flatbuffers${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (flatbuffers_EXT_LIBRARIES   "${flatbuffers_EXT_STATIC_LIB}")
exists_files (flatbuffers_EXT_EXISTS
        ${flatbuffers_EXT_COMPILER}
        ${flatbuffers_EXT_HASH}
        ${flatbuffers_EXT_HEADER}
        ${flatbuffers_EXT_LIBRARIES})

if (NOT flatbuffers_EXT_EXISTS)
    message (STATUS "Add external/flatbuffers")
    ExternalProject_Add (flatbuffers_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${flatbuffers_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (flatbuffers_ext_output flatbuffers_ext ${flatbuffers_EXT_LIBRARIES})
endif ()
add_custom_target (flatbuffers DEPENDS ${flatbuffers_EXT_LIBRARIES})

##########
## lfds ##
##########

set (lfds_EXT_VERSION     711)
set (lfds_EXT_SOURCE_DIR  "${CMAKE_SOURCE_DIR}/external/lfds")
set (lfds_EXT_INCLUDE_DIR "${EXT_INSTALL_DIR}/include/liblfds${lfds_EXT_VERSION}")
set (lfds_EXT_HEADER      "${EXT_INSTALL_DIR}/include/liblfds${lfds_EXT_VERSION}.h")
set (lfds_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lfds${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (lfds_EXT_LIBRARIES   "${lfds_EXT_STATIC_LIB}")
exists_files (lfds_EXT_EXISTS ${lfds_EXT_HEADER} ${lfds_EXT_LIBRARIES})

if (NOT lfds_EXT_EXISTS)
    message (STATUS "Add external/lfds")
    ExternalProject_Add (lfds_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${lfds_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (lfds_ext_output lfds_ext ${lfds_EXT_LIBRARIES})
endif ()
add_custom_target (lfds DEPENDS ${lfds_EXT_LIBRARIES})

#################
## http-parser ##
#################

set (http_parser_EXT_SOURCE_DIR  "${CMAKE_SOURCE_DIR}/external/http-parser")
set (http_parser_EXT_INCLUDE_DIR "${EXT_INSTALL_DIR}/include")
set (http_parser_EXT_HEADER      "${http_parser_EXT_INCLUDE_DIR}/http_parser.h")
set (http_parser_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}http_parser${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (http_parser_EXT_LIBRARIES   "${http_parser_EXT_STATIC_LIB}")
exists_files (http_parser_EXT_EXISTS ${http_parser_EXT_HEADER} ${http_parser_EXT_LIBRARIES})

if (NOT http_parser_EXT_EXISTS)
    message (STATUS "Add external/http_parser")
    ExternalProject_Add (http_parser_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${http_parser_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (http_parser_ext_output http_parser_ext ${http_parser_EXT_LIBRARIES})
endif ()
add_custom_target (http_parser DEPENDS ${http_parser_EXT_LIBRARIES})

###########
## lemon ##
###########

set (lemon_EXT_SOURCE_DIR  "${CMAKE_SOURCE_DIR}/external/lemon")
set (lemon_EXT_INCLUDE_DIR "${EXT_INSTALL_DIR}/include")
set (lemon_EXT_HEADER      "${lemon_EXT_INCLUDE_DIR}/lemon/config.h")
set (lemon_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lemon${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (lemon_EXT_LIBRARIES   "${lemon_EXT_STATIC_LIB}")
exists_files (lemon_EXT_EXISTS ${lemon_EXT_HEADER} ${lemon_EXT_LIBRARIES})

if (NOT lemon_EXT_EXISTS)
    message (STATUS "Add external/lemon")
    ExternalProject_Add (lemon_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${lemon_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (lemon_ext_output lemon_ext ${lemon_EXT_LIBRARIES})
endif ()
add_custom_target (lemon DEPENDS ${lemon_EXT_LIBRARIES})

###########
## gtest ##
###########

set (gtest_EXT_SOURCE_DIR  "${CMAKE_SOURCE_DIR}/external/gtest")
set (gtest_EXT_INCLUDE_DIR "${EXT_INSTALL_DIR}/include")
set (gtest_EXT_HEADER      "${gtest_EXT_INCLUDE_DIR}/gtest/gtest.h")
set (gtest_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (gtest_EXT_LIBRARIES   "${gtest_EXT_STATIC_LIB}")
exists_files (gtest_EXT_EXISTS ${gtest_EXT_HEADER} ${gtest_EXT_LIBRARIES})

if (NOT gtest_EXT_EXISTS)
    message (STATUS "Add external/gtest")
    ExternalProject_Add (gtest_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${gtest_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (gtest_ext_output gtest_ext ${gtest_EXT_LIBRARIES})
endif ()
add_custom_target (gtest DEPENDS ${gtest_EXT_LIBRARIES})

