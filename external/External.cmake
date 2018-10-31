## CMake build script.

include (ExternalProject)

#/// Check exists libraries.
#///
#/// @param __result  [out] value name of output result.
#/// @param __files   [in]  Files.
function (exists_files __result __files)
    set (${__result} ON)

    foreach (__lib_cursor ${__files})
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
#/// @param __files [in] Output files.
function (fake_output_library __name __deps __files)
    add_custom_command (OUTPUT  ${__files}
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
    set (EXT_CXX_FLAGS "-w -fPIC")
    set (EXT_C_FLAGS   "-w -fPIC")
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
exists_files (uv_EXT_EXISTS "${uv_EXT_LIBRARIES}")

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
    fake_output_library (uv_ext_output uv_ext "${uv_EXT_LIBRARIES}")
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

set (zlib_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/zlib")
set (zlib_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (zlib_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${zlib_EXT_STATIC_LIB_NAME}${zlib_EXT_DEBUG_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (zlib_EXT_LIBRARIES    "${zlib_EXT_STATIC_LIB}")
exists_files (zlib_EXT_EXISTS "${zlib_EXT_LIBRARIES}")

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
    fake_output_library (zlib_ext_output zlib_ext "${zlib_EXT_LIBRARIES}")
endif ()
add_custom_target (zlib DEPENDS ${zlib_EXT_LIBRARIES})

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
exists_files (ressl_EXT_EXISTS "${ressl_EXT_LIBRARIES}")

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
    fake_output_library (ressl_ext_output ressl_ext "${ressl_EXT_LIBRARIES}")
endif ()
add_custom_target (ressl DEPENDS ${ressl_EXT_LIBRARIES})

##############
## FREETYPE ##
##############

set (freetype_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/freetype")
set (freetype_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (freetype_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}freetype${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (freetype_EXT_LIBRARIES    "${freetype_EXT_STATIC_LIB}")
exists_files (freetype_EXT_EXISTS "${freetype_EXT_LIBRARIES}")

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
    fake_output_library (freetype_ext_output freetype_ext "${freetype_EXT_LIBRARIES}")
endif ()
add_custom_target (freetype DEPENDS ${freetype_EXT_LIBRARIES})

#########
## OGG ##
#########

set (ogg_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/ogg")
set (ogg_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (ogg_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}ogg${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (ogg_EXT_LIBRARIES    "${ogg_EXT_STATIC_LIB}")
exists_files (ogg_EXT_EXISTS "${ogg_EXT_LIBRARIES}")

if (ogg_EXT_EXISTS)
    message (STATUS "Skip external/ogg (Exists: ${ogg_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/ogg")
    ExternalProject_Add (ogg_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${ogg_EXT_SOURCE_DIR}"
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
    fake_output_library (ogg_ext_output ogg_ext "${ogg_EXT_LIBRARIES}")
endif ()
add_custom_target (ogg DEPENDS ${ogg_EXT_LIBRARIES})

##########
## FLAC ##
##########

set (flac_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/flac")
set (flac_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (flac_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}FLAC${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (flacxx_EXT_STATIC_LIB "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}FLACXX${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (flac_EXT_LIBRARIES    "${flac_EXT_STATIC_LIB}"
                           "${flacxx_EXT_STATIC_LIB}")
exists_files (flac_EXT_EXISTS "${flac_EXT_LIBRARIES}")

if (flac_EXT_EXISTS)
    message (STATUS "Skip external/flac (Exists: ${flac_EXT_STATIC_LIB})")
    message (STATUS "Skip external/flacxx (Exists: ${flacxx_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/flac")
    ExternalProject_Add (flac_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${flac_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DOGG_LIBRARY_DIR=${EXT_INSTALL_DIR}/lib"
                       "-DOGG_INCLUDE_DIR=${EXT_INSTALL_DIR}/include"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1
            #--Dependencies---------------
            DEPENDS ogg)
    fake_output_library (flac_ext_output flac_ext "${flac_EXT_LIBRARIES}")
endif ()
add_custom_target (flac DEPENDS ${flac_EXT_LIBRARIES})

############
## VORBIS ##
############

set (vorbis_EXT_SOURCE_DIR      "${CMAKE_SOURCE_DIR}/external/vorbis")
set (vorbis_EXT_INCLUDE_DIR     "${EXT_INSTALL_DIR}/include")
set (vorbis_EXT_STATIC_LIB      "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}vorbis${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (vorbisenc_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}vorbisenc${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (vorbisfile_EXT_STATIC_LIB  "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}vorbisfile${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (vorbis_EXT_LIBRARIES       "${vorbis_EXT_STATIC_LIB}"
                                "${vorbisenc_EXT_STATIC_LIB}"
                                "${vorbisfile_EXT_STATIC_LIB}")
exists_files (vorbis_EXT_EXISTS "${vorbis_EXT_LIBRARIES}")

if (vorbis_EXT_EXISTS)
    message (STATUS "Skip external/vorbis (Exists: ${vorbis_EXT_STATIC_LIB})")
    message (STATUS "Skip external/vorbisenc (Exists: ${vorbisenc_EXT_STATIC_LIB})")
    message (STATUS "Skip external/vorbisfile (Exists: ${vorbisfile_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/vorbis")
    ExternalProject_Add (vorbis_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${vorbis_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DOGG_ROOT=${EXT_INSTALL_DIR}"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1
            #--Dependencies---------------
            DEPENDS ogg)
    fake_output_library (vorbis_ext_output vorbis_ext "${vorbis_EXT_LIBRARIES}")
endif ()
add_custom_target (vorbis DEPENDS ${vorbis_EXT_LIBRARIES})

##########
## SFML ##
##########

if ("${CMAKE_BUILD_TYPE_LOWER}" STREQUAL "" OR "${CMAKE_BUILD_TYPE_LOWER}" STREQUAL "debug")
    set (sfml_POSTFIX -d)
else ()
    set (sfml_POSTFIX)
endif ()

set (sfml_EXT_SOURCE_DIR            "${CMAKE_SOURCE_DIR}/external/sfml")
set (sfml_EXT_INCLUDE_DIR           "${EXT_INSTALL_DIR}/include")
set (sfml_system_EXT_STATIC_LIB     "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-system-s${sfml_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (sfml_window_EXT_STATIC_LIB     "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-window-s${sfml_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (sfml_graphics_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-graphics-s${sfml_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (sfml_audio_EXT_STATIC_LIB      "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}sfml-audio-s${sfml_POSTFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (sfml_EXT_LIBRARIES             "${sfml_system_EXT_STATIC_LIB}"
                                    "${sfml_window_EXT_STATIC_LIB}"
                                    "${sfml_graphics_EXT_STATIC_LIB}"
                                    "${sfml_audio_EXT_STATIC_LIB}")
exists_files (sfml_EXT_EXISTS   "${sfml_EXT_LIBRARIES}")

if (sfml_EXT_EXISTS)
    message (STATUS "Skip external/sfml_system (Exists: ${sfml_system_EXT_STATIC_LIB})")
    message (STATUS "Skip external/sfml_window (Exists: ${sfml_window_EXT_STATIC_LIB})")
    message (STATUS "Skip external/sfml_graphics (Exists: ${sfml_graphics_EXT_STATIC_LIB})")
    message (STATUS "Skip external/sfml_audio (Exists: ${sfml_audio_EXT_STATIC_LIB})")
else ()
    message (STATUS "Add external/sfml")
    ExternalProject_Add (sfml_ext
            PREFIX "${EXT_PREFIX_DIR}"
            #--Configure step-------------
            SOURCE_DIR "${sfml_EXT_SOURCE_DIR}"
            CMAKE_ARGS "-DCMAKE_MACOSX_RPATH=${CMAKE_MACOSX_RPATH}"
                       "-DBUILD_SHARED_LIBS=OFF"
                       "-DCMAKE_C_FLAGS=${EXT_C_FLAGS}"
                       "-DCMAKE_CXX_FLAGS=${EXT_CXX_FLAGS}"
                       "-DCMAKE_BUILD_TYPE=${EXT_BUILD_TYPE}"
                       "-DCMAKE_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DSFML_BUILD_FRAMEWORKS=OFF"
                       "-DSFML_INSTALL_XCODE_TEMPLATES=OFF"
                       "-DSFML_DEPENDENCIES_INSTALL_PREFIX=${EXT_INSTALL_DIR}"
                       "-DSFML_INSTALL_PKGCONFIG_FILES=OFF"
                       "-DSTB_INCLUDE_DIR=${CMAKE_SOURCE_DIR}/dep/stb"
                       "-DFREETYPE_INCLUDE_DIR=${EXT_INSTALL_DIR}/include"
                       "-DCMAKE_FIND_ROOT_PATH=${EXT_INSTALL_DIR}"
                       "-DSFML_BUILD_WINDOW=ON"
                       "-DSFML_BUILD_GRAPHICS=ON"
                       "-DSFML_BUILD_AUDIO=ON"
                       "-DSFML_BUILD_NETWORK=OFF"
                       "-DSFML_BUILD_DOC=OFF"
            #--Output logging-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1
            #--Dependencies---------------
            DEPENDS ogg vorbis flac freetype)
    fake_output_library (sfml_ext_output sfml_ext "${sfml_EXT_LIBRARIES}")
endif ()
add_custom_target (sfml DEPENDS ${sfml_EXT_LIBRARIES})

############
## LUAJIT ##
############

set (luajit_EXT_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external/luajit")
set (luajit_EXT_INCLUDE_DIR  "${EXT_INSTALL_DIR}/include")
set (luajit_EXT_CONF_HEADER  "${luajit_EXT_INCLUDE_DIR}/luaconf.h")
set (luajit_EXT_STATIC_LIB   "${EXT_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lua${CMAKE_STATIC_LIBRARY_SUFFIX}")
set (luajit_EXT_LIBRARIES    "${luajit_EXT_STATIC_LIB}"
                             "${luajit_EXT_CONF_HEADER}")
exists_files (luajit_EXT_EXISTS "${luajit_EXT_LIBRARIES}")

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
            #--Output lluajiting-------------
            LOG_DOWNLOAD  1
            LOG_UPDATE    1
            LOG_CONFIGURE 1
            LOG_BUILD     0
            LOG_TEST      1
            LOG_INSTALL   1)
    fake_output_library (luajit_ext_output luajit_ext "${luajit_EXT_LIBRARIES}")
endif ()
add_custom_target (luajit DEPENDS ${luajit_EXT_LIBRARIES})

