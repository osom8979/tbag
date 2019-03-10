#/// @file   TbagModules.cmake
#/// @brief  Modules utilities.
#/// @author zer0
#/// @date   2016-05-27

include (TbagUtils)

## -----------
## Properties.
## -----------

macro (tbag_modules__preview)
    # Flag variables.
    message (STATUS "TBAG_PROJECT_LIB_TYPE: ${TBAG_PROJECT_LIB_TYPE}")

    # List variables.
    message (STATUS "TBAG_PROJECT_OBJECTS: ${TBAG_PROJECT_OBJECTS}")
    message (STATUS "TBAG_PROJECT_DEPENDENCIES: ${TBAG_PROJECT_DEPENDENCIES}")
    message (STATUS "TBAG_PROJECT_DEFINITIONS: ${TBAG_PROJECT_DEFINITIONS}")
    message (STATUS "TBAG_PROJECT_INCLUDE_DIRS: ${TBAG_PROJECT_INCLUDE_DIRS}")
    message (STATUS "TBAG_PROJECT_CXXFLAGS: ${TBAG_PROJECT_CXXFLAGS}")
    message (STATUS "TBAG_PROJECT_LDFLAGS: ${TBAG_PROJECT_LDFLAGS}")

    # Constant variables.
    message (STATUS "TBAG_PROJECT_CONST_CMAKE_PATH: ${TBAG_PROJECT_CONST_CMAKE_PATH}")
    message (STATUS "TBAG_PROJECT_CONST_DIR_PATH: ${TBAG_PROJECT_CONST_DIR_PATH}")
    message (STATUS "TBAG_PROJECT_CONST_DIR_NAME: ${TBAG_PROJECT_CONST_DIR_NAME}")
    message (STATUS "TBAG_PROJECT_CONST_TYPE: ${TBAG_PROJECT_CONST_TYPE}")
    message (STATUS "TBAG_PROJECT_CONST_NAME: ${TBAG_PROJECT_CONST_NAME}")
endmacro ()

## -----------------
## Project settings.
## -----------------

macro (tbag_modules__apply_simple)
    tbag_modules__apply_default ()
    tbag_modules__update_default_objects ()
    tbag_modules__add_target ()
    tbag_modules__update_all_properties ()
endmacro ()

macro (tbag_modules__apply_default)
    #string (TOUPPER "${TBAG_PROJECT_CONST_NAME}" __tbag_project_upper_name)
    #list (APPEND TBAG_PROJECT_DEFINITIONS  ${__tbag_project_upper_name})
    list  (APPEND TBAG_PROJECT_INCLUDE_DIRS ${TBAG_PROJECT_CONST_DIR_PATH})

    if (UNIX AND APPLE AND IS_DIRECTORY "${THIRD_PREFIX}")
        list (APPEND TBAG_PROJECT_LDFLAGS "-Wl,-rpath,${THIRD_PREFIX}/lib"
                                          "-Wl,-rpath,${THIRD_PREFIX}/Library/Frameworks")
    endif ()
endmacro ()

macro (tbag_modules__apply_name __name)
    set (TBAG_PROJECT_CONST_NAME ${__name})
endmacro ()

macro (tbag_modules__apply_lib)
    set (TBAG_PROJECT_CONST_NAME ${TBAG_PROJECT_LIBRARY_PREFIX})
endmacro ()

macro (tbag_modules__apply_exe)
    set (TBAG_PROJECT_CONST_NAME ${TBAG_PROJECT_EXECUTABLE_PREFIX})
endmacro ()

macro (tbag_modules__apply_pch __original_pch)
    if (NOT EXISTS ${__original_pch})
        message (FATAL_ERROR "Not found original header file path: ${__original_pch}")
    endif ()

    if (NOT DEFINED TbagProjectPCH_NAME)
        get_filename_component (TbagProjectPCH_ORIGINAL_NAME "${__original_pch}" NAME)
        string (REPLACE "." "-" TbagProjectPCH_NAME "${TbagProjectPCH_ORIGINAL_NAME}-pch")
        set (TbagProjectPCH_COPY_PATH ${CMAKE_BINARY_DIR}/${TbagProjectPCH_ORIGINAL_NAME})

        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            set (TbagProjectPCH_RESULT_PATH "${TbagProjectPCH_COPY_PATH}.gch")
        else ()
            set (TbagProjectPCH_RESULT_PATH "${TbagProjectPCH_COPY_PATH}.pch")
        endif ()

        ## Extension CXX flags.
        if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
            set (TbagProjectPCH_CXX_FLAGS_EX ${CMAKE_C_FLAGS_DEBUG})
        elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
            set (TbagProjectPCH_CXX_FLAGS_EX ${CMAKE_C_FLAGS_RELEASE})
        elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
            set (TbagProjectPCH_CXX_FLAGS_EX ${CMAKE_C_FLAGS_RELWITHDEBINFO})
        elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
            set (TbagProjectPCH_CXX_FLAGS_EX ${CMAKE_C_FLAGS_MINSIZEREL})
        endif()

        string (REPLACE " " ";" TbagProjectPCH_CXX_FLAGS "${CMAKE_CXX_FLAGS};${TbagProjectPCH_CXX_FLAGS_EX}")

        ## Definitions.
        foreach (__define_cursor ${TBAG_PROJECT_DEFINITIONS})
            if ("${__define_cursor}" MATCHES "^-D")
                list (APPEND TbagProjectPCH_CXX_FLAGS "${__define_cursor}")
            else ()
                list (APPEND TbagProjectPCH_CXX_FLAGS "-D${__define_cursor}")
            endif ()
        endforeach ()

        ## Include directories.
        foreach (__include_cursor ${TBAG_PROJECT_INCLUDE_DIRS})
            if ("${__include_cursor}" MATCHES "^-I")
                list (APPEND TbagProjectPCH_CXX_FLAGS "${__include_cursor}")
            else ()
                list (APPEND TbagProjectPCH_CXX_FLAGS "-I${__include_cursor}")
            endif ()
        endforeach ()

        ## PCH custom target.
        add_custom_target (${TbagProjectPCH_NAME} ALL DEPENDS ${TbagProjectPCH_RESULT_PATH})

        add_custom_command (
                OUTPUT ${TbagProjectPCH_COPY_PATH}
                COMMAND ${CMAKE_COMMAND} -E copy "${__original_pch}" "${TbagProjectPCH_COPY_PATH}"
                DEPENDS "${__original_pch}"
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

        add_custom_command (
                OUTPUT ${TbagProjectPCH_RESULT_PATH}
                COMMAND ${CMAKE_CXX_COMPILER} ${TbagProjectPCH_CXX_FLAGS}
                        -x c++-header ${TbagProjectPCH_COPY_PATH}
                        -o ${TbagProjectPCH_RESULT_PATH}
                DEPENDS "${TbagProjectPCH_COPY_PATH}"
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif ()

    list (APPEND TBAG_PROJECT_DEPENDENCIES ${TbagProjectPCH_NAME})
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_BINARY_DIR})
    list (APPEND TBAG_PROJECT_CXXFLAGS -include ${TbagProjectPCH_COPY_PATH})
endmacro ()

macro (tbag_modules__add_whole_archive __lib_name)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" AND MSVC_VERSION)
        if (NOT MSVC_VERSION LESS 1900)
            #list (APPEND TBAG_PROJECT_LDFLAGS "/WHOLEARCHIVE:${__lib_name}") ## ERROR: Auto converting '/' -> '\'
            list (APPEND TBAG_PROJECT_LDFLAGS ${__lib_name})
        else ()
            list (APPEND TBAG_PROJECT_LDFLAGS ${__lib_name})
        endif ()
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        list (APPEND TBAG_PROJECT_LDFLAGS -Wl,-force_load,${__lib_name}) ## All files: -Wl,-all_load
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (APPEND TBAG_PROJECT_LDFLAGS -Wl,--whole-archive ${__lib_name} -Wl,--no-whole-archive)
    else ()
        list (APPEND TBAG_PROJECT_LDFLAGS ${__lib_name})
    endif ()
endmacro ()

macro (tbag_modules__apply_shared_library)
    set (TBAG_PROJECT_LIB_TYPE SHARED)
endmacro ()

macro (tbag_modules__apply_static_library)
    set (TBAG_PROJECT_LIB_TYPE STATIC)
endmacro ()

macro (tbag_modules__apply_module_library)
    set (TBAG_PROJECT_LIB_TYPE MODULE)
endmacro ()

macro (tbag_modules__append_objects)
    list (APPEND TBAG_PROJECT_OBJECTS ${ARGN})
endmacro ()

macro (tbag_modules__append_dependencies)
    list (APPEND TBAG_PROJECT_DEPENDENCIES ${ARGN})
endmacro ()

macro (tbag_modules__append_definitions)
    list (APPEND TBAG_PROJECT_DEFINITIONS ${ARGN})
endmacro ()

macro (tbag_modules__append_include_dirs)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${ARGN})
endmacro ()

macro (tbag_modules__append_cxxflags)
    list (APPEND TBAG_PROJECT_CXXFLAGS ${ARGN})
endmacro ()

macro (tbag_modules__append_ldflags)
    list (APPEND TBAG_PROJECT_LDFLAGS ${ARGN})
endmacro ()

macro (tbag_modules__apply_window_subsystem)
    if (WIN32)
        if (MSVC)
            list (APPEND TBAG_PROJECT_LDFLAGS  /SUBSYSTEM:WINDOWS)
        elseif (MINGW)
            list (APPEND TBAG_PROJECT_CXXFLAGS -Wno-deprecated-declarations)
            list (APPEND TBAG_PROJECT_LDFLAGS  -mwindows)
        endif ()
    endif ()
endmacro ()

macro (tbag_modules__apply_disable_warnings)
    if (MSVC)
        list (APPEND TBAG_PROJECT_CXXFLAGS /w)
    else ()
        list (APPEND TBAG_PROJECT_CXXFLAGS -w)
    endif ()
endmacro ()

macro (tbag_modules__apply_rtti)
    if (WIN32)
        if (MSVC)
            list (APPEND TBAG_PROJECT_CXXFLAGS /GR)
        elseif (MINGW)
            list (APPEND TBAG_PROJECT_CXXFLAGS -frtti)
        endif ()
    else ()
        list (APPEND TBAG_PROJECT_CXXFLAGS -frtti)
    endif ()
endmacro ()

macro (tbag_modules__apply_no_rtti)
    if (WIN32)
        if (MSVC)
            list (APPEND TBAG_PROJECT_CXXFLAGS /GR-)
        elseif (MINGW)
            list (APPEND TBAG_PROJECT_CXXFLAGS -fno-rtti)
        endif ()
    else ()
        list (APPEND TBAG_PROJECT_CXXFLAGS -fno-rtti)
    endif ()
endmacro ()

macro (tbag_modules__apply_thread)
    if (CMAKE_USE_SPROC_INIT)
    endif ()
    if (CMAKE_USE_WIN32_THREADS_INIT)
    endif ()
    if (CMAKE_USE_PTHREADS_INIT)
        list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
    endif ()
    if (CMAKE_HP_PTHREADS_INIT)
    endif ()
endmacro ()

## ----------
## Generators
## ----------

#/// Write Data to String header file.
#///
#/// @param __target      [in] Target name.
#/// @param __output_path [in] Output header file path.
#/// @param __name        [in] Variable name.
#/// @param __source_path [in] Source file path.
#/// @param __hex_mode    [in] Hex mode (ON/OFF)
macro (tbag_modules__data_to_string __target __output_path __name __source_path __hex_mode)
    add_custom_command (
            OUTPUT  "${__output_path}"
            COMMAND ${CMAKE_COMMAND} "-DOUTPUT_PATH=${__output_path}"
                                     "-DNAME=${__name}"
                                     "-DSOURCE_PATH=${__source_path}"
                                     "-DHEX_MODE=${__hex_mode}"
                                     -P "${TBAG_SCRIPT_DIR}/TbagData2String.cmake"
            DEPENDS ${__text_path}
            COMMENT "Data to String: ${__output_path}" VERBATIM)
    add_custom_target (${__target} SOURCES "${__output_path}")
    list (APPEND TBAG_PROJECT_DEPENDENCIES ${__target})
endmacro ()

#/// Write Data to 7zip to String header file.
#///
#/// @param __target      [in] Target name.
#/// @param __output_path [in] Output header file path.
#/// @param __name        [in] Variable name.
#/// @param __source_path [in] Source file path.
macro (tbag_modules__data_to_7z_to_string __target __output_path __name __source_path)
    set (__7zip_output_temp_path "${CMAKE_BINARY_DIR}/${__target}.7z")
    get_filename_component (__text_path_dir ${__source_path} DIRECTORY)
    get_filename_component (__text_path_name ${__source_path} NAME)
    add_custom_command (
            OUTPUT  "${__7zip_output_temp_path}" "${__output_path}"
            COMMAND ${CMAKE_COMMAND} -E tar cf
                                     "${__7zip_output_temp_path}"
                                     --format=7zip
                                     "${__text_path_name}"
            COMMAND ${CMAKE_COMMAND} "-DOUTPUT_PATH=${__output_path}"
                                     "-DNAME=${__name}"
                                     "-DSOURCE_PATH=${__7zip_output_temp_path}"
                                     "-DHEX_MODE=ON"
                                     -P "${TBAG_SCRIPT_DIR}/TbagData2String.cmake"
            DEPENDS ${__text_path}
            WORKING_DIRECTORY "${__text_path_dir}"
            COMMENT "Data to 7zip to String: ${__output_path}" VERBATIM)
    add_custom_target (${__target} SOURCES "${__output_path}")
    list (APPEND TBAG_PROJECT_DEPENDENCIES ${__target})
endmacro ()

#/// Copy raw files.
#///
#/// @param __target             [in] Target name.
#/// @param __source_prefix      [in] Source prefix.
#/// @param __destination_prefix [in] Destination prefix.
#/// @param __glob               [in] File glob pattern.
macro (tbag_modules__copy_file __target __source_prefix __destination_prefix __glob)
    set (__copy_files)
    file (GLOB __glob_files RELATIVE "${__source_prefix}" "${__source_prefix}${__glob}")

    foreach (__cursor ${__glob_files})
        set (__cursor_src  "${__source_prefix}${__cursor}")
        set (__cursor_dest "${__destination_prefix}${__cursor}")

        if (IS_DIRECTORY "${__cursor_src}")
            tbag_debug (tbag_modules__copy_dir "Target(${__target}) copy info: ${__cursor_src} -> ${__cursor_dest}")
            add_custom_command (
                    OUTPUT  "${__cursor_dest}"
                    COMMAND ${CMAKE_COMMAND} -E copy_directory "${__cursor_src}" "${__cursor_dest}"
                    DEPENDS "${__cursor_src}")
        else ()
            tbag_debug (tbag_modules__copy_file "Target(${__target}) copy info: ${__cursor_src} -> ${__cursor_dest}")
            add_custom_command (
                    OUTPUT  "${__cursor_dest}"
                    COMMAND ${CMAKE_COMMAND} -E copy "${__cursor_src}" "${__cursor_dest}"
                    DEPENDS "${__cursor_src}")
        endif ()

        list (APPEND __copy_files "${__cursor_dest}")
        unset (__cursor_src)
        unset (__cursor_dest)
    endforeach ()

    add_custom_target (${__target} ALL
            DEPENDS ${__copy_files}
            COMMENT "Copying assets: ${__source_prefix}${__glob} -> ${__destination_prefix}${__glob}")
    unset (__glob_files)
    unset (__copy_files)

    list (APPEND TBAG_PROJECT_DEPENDENCIES ${__target})
endmacro ()

## -----
## CUDA.
## -----

macro (tbag_modules__check_cuda)
    if (NOT CUDA_FOUND)
        message (WARNING "Not found CUDA.")
    endif ()
endmacro ()

macro (tbag_modules__apply_include_of_cuda)
    foreach (__include_dir_cursor ${ARGN})
        cuda_include_directories (${__include_dir_cursor})
    endforeach ()
endmacro ()

macro (tbag_modules__apply_all_include_of_cuda)
    tbag_modules__apply_include_of_cuda (${TBAG_PROJECT_INCLUDE_DIRS})
endmacro ()

macro (tbag_modules__build_cuda)
    foreach (__cuda_cusor ${ARGN})
        tabg_cuda__compile (__cuda_object ${__cuda_cusor})
        list (APPEND TBAG_PROJECT_OBJECTS ${__cuda_object})
    endforeach ()
endmacro ()

macro (tbag_modules__apply_cuda)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CUDA_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${CUDA_LIBRARIES})
endmacro ()

macro (tbag_modules__apply_cublas)
    list (APPEND TBAG_PROJECT_LDFLAGS ${CUDA_CUBLAS_LIBRARIES})
endmacro ()

macro (tbag_modules__apply_cudnn)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CUDNN_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${CUDNN_LIBRARIES})
endmacro ()

macro (tbag_modules__apply_curand)
    list (APPEND TBAG_PROJECT_LDFLAGS ${CUDA_curand_LIBRARY})
endmacro ()

## -----
## GCOV.
## -----

macro (tbag_modules__apply_gcov)
    list (APPEND TBAG_PROJECT_CXXFLAGS -fprofile-arcs -ftest-coverage)
    list (APPEND TBAG_PROJECT_LDFLAGS -lgcov --coverage)
endmacro ()

## --------------
## Dep libraries.
## --------------

macro (tbag_modules__apply_dep_boost_include)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/boost/include)
endmacro ()

macro (tbag_modules__build_dep_flatc_cpp __target __fbs)
    list (APPEND TBAG_PROJECT_DEPENDENCIES flatc ${__target})

    set (Flatc_SKIP_FOUND ON)
    find_package (Flatc QUIET)
    unset (Flatc_SKIP_FOUND)

    flatc_generate_target2 (${__target} $<TARGET_FILE:flatc> cpp ${__fbs})
endmacro ()

macro (tbag_modules__apply_dep_flatbuffers)
    list (APPEND TBAG_PROJECT_DEPENDENCIES flatbuffers)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/flatbuffers/include)
    tbag_modules__add_whole_archive ($<TARGET_FILE:flatbuffers>)
endmacro ()

macro (tbag_modules__apply_dep_gtest)
    list (APPEND TBAG_PROJECT_DEPENDENCIES gtest)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/gtest/include)

    tbag_modules__add_whole_archive ($<TARGET_FILE:gtest>)

    if (WIN32)
        list (APPEND TBAG_PROJECT_DEFINITIONS GTEST_OS_WINDOWS)
    endif ()

    ## external libraries.
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
    endif ()
endmacro ()

macro (tbag_modules__apply_dep_icu)
    list (APPEND TBAG_PROJECT_DEPENDENCIES icuuc icui18n)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/icu/common
                                           ${CMAKE_SOURCE_DIR}/dep/icu/i18n)
    tbag_modules__add_whole_archive ($<TARGET_FILE:icuuc>)
    tbag_modules__add_whole_archive ($<TARGET_FILE:icui18n>)
endmacro ()

macro (tbag_modules__apply_dep_http_parser)
    list (APPEND TBAG_PROJECT_DEPENDENCIES http_parser)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/http-parser)
    tbag_modules__add_whole_archive ($<TARGET_FILE:http_parser>)
endmacro ()

macro (tbag_modules__apply_dep_lmdb)
    list (APPEND TBAG_PROJECT_DEPENDENCIES lmdb)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/lmdb)
    tbag_modules__add_whole_archive ($<TARGET_FILE:lmdb>)
endmacro ()

macro (tbag_modules__apply_dep_lua)
    list (APPEND TBAG_PROJECT_DEPENDENCIES lua)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/lua/include)
    tbag_modules__add_whole_archive ($<TARGET_FILE:lua>)
endmacro ()

macro (tbag_modules__apply_dep_lua_platform_macros)
    if (WIN32)
        list (APPEND TBAG_PROJECT_DEFINITIONS LUA_BUILD_AS_DLL)
    elseif (UNIX)
        if (APPLE)
            list (APPEND TBAG_PROJECT_DEFINITIONS LUA_USE_MACOSX)
        else ()
            list (APPEND TBAG_PROJECT_DEFINITIONS LUA_USE_POSIX)
        endif ()
    endif ()
endmacro ()

macro (tbag_modules__apply_dep_lua_export_macros)
    tbag_modules__apply_dep_lua_platform_macros ()
    if (WIN32)
        list (APPEND TBAG_PROJECT_DEFINITIONS LUA_CORE LUA_LIB)
    endif ()
endmacro ()

macro (tbag_modules__apply_dep_lua_import_macros)
    tbag_modules__apply_dep_lua_platform_macros ()
endmacro ()

macro (tbag_modules__apply_dep_stb)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/stb)
endmacro ()

macro (tbag_modules__apply_dep_luabridge)
    ## Header-only library.
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/luabridge)
endmacro ()

macro (tbag_modules__apply_dep_minizip)
    list (APPEND TBAG_PROJECT_DEPENDENCIES minizip)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/minizip)
    tbag_modules__add_whole_archive ($<TARGET_FILE:minizip>)
endmacro ()

macro (tbag_modules__apply_dep_sqlite3)
    list (APPEND TBAG_PROJECT_DEPENDENCIES sqlite3)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/sqlite3)
    tbag_modules__add_whole_archive ($<TARGET_FILE:sqlite3>)
endmacro ()

macro (tbag_modules__apply_dep_z)
    list (APPEND TBAG_PROJECT_DEPENDENCIES z)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/zlib)
    tbag_modules__add_whole_archive ($<TARGET_FILE:z>)
endmacro ()

macro (tbag_modules__apply_dep_simdpp)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/simdpp)
endmacro ()

macro (tbag_modules__build_dep_simdpp)
    if (DEFINED SIMDPP_RUNNABLE_ARCHS)
        foreach (__simd_cusor ${ARGN})
            simdpp_multiarch (__simd_object "${__simd_cusor}" ${SIMDPP_RUNNABLE_ARCHS})
            list (APPEND TBAG_PROJECT_OBJECTS ${__simd_object})
        endforeach ()
    endif ()
endmacro ()

macro (tbag_modules__apply_dep_lemon)
    list (APPEND TBAG_PROJECT_DEPENDENCIES lemon)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/lemon/include)

    tbag_modules__add_whole_archive ($<TARGET_FILE:lemon>)

    if (HAVE_LONG_LONG)
        list (APPEND TBAG_PROJECT_DEFINITIONS LEMON_HAVE_LONG_LONG=1)
    endif ()
    if (CMAKE_USE_PTHREADS_INIT)
        list (APPEND TBAG_PROJECT_DEFINITIONS LEMON_USE_PTHREAD=1)
    elseif (CMAKE_USE_WIN32_THREADS_INIT)
        list (APPEND TBAG_PROJECT_DEFINITIONS LEMON_USE_WIN32_THREADS=1)
    endif ()

    ## external libraries.
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
    endif ()
endmacro ()

macro (tbag_modules__apply_dep_nng)
    list (APPEND TBAG_PROJECT_DEPENDENCIES nng)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/nng/src)

    tbag_modules__add_whole_archive ($<TARGET_FILE:nng>)

    ## external libraries.
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
    endif ()
endmacro ()

macro (tbag_modules__apply_dep_cds)
    list (APPEND TBAG_PROJECT_DEPENDENCIES cds)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/cds)
    tbag_modules__add_whole_archive ($<TARGET_FILE:cds>)
endmacro ()

macro (tbag_modules__apply_dep_lfds)
    list (APPEND TBAG_PROJECT_DEPENDENCIES lfds)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/lfds/inc)
    tbag_modules__add_whole_archive ($<TARGET_FILE:lfds>)
endmacro ()

macro (tbag_modules__apply_dep_luasfml)
    list (APPEND TBAG_PROJECT_DEPENDENCIES luasfml)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/luasfml/src)
    tbag_modules__add_whole_archive ($<TARGET_FILE:luasfml>)
endmacro ()

## -------------------
## External libraries.
## -------------------

macro (tbag_modules__apply_ext_uv)
    list (APPEND TBAG_PROJECT_DEPENDENCIES uv)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${uv_EXT_INCLUDE_DIR})
    tbag_modules__add_whole_archive (${uv_EXT_STATIC_LIB})

    ## external libraries.
    if (WIN32)
        list (APPEND TBAG_PROJECT_LDFLAGS advapi32.lib iphlpapi.lib psapi.lib shell32.lib
                                          user32.lib userenv.lib ws2_32.lib)
    else ()
        list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
        if (APPLE)
            list (APPEND TBAG_PROJECT_LDFLAGS -ldl)
        else ()
            list (APPEND TBAG_PROJECT_LDFLAGS -lnsl -ldl -lrt)
        endif ()
    endif ()
endmacro ()

macro (tbag_modules__apply_ext_z)
    list (APPEND TBAG_PROJECT_DEPENDENCIES zlib)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${zlib_EXT_INCLUDE_DIR})
    tbag_modules__add_whole_archive (${zlib_EXT_STATIC_LIB})
endmacro ()

macro (tbag_modules__apply_ext_ressl)
    list (APPEND TBAG_PROJECT_DEPENDENCIES ressl)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${ressl_EXT_INCLUDE_DIR})
    tbag_modules__add_whole_archive (${ressl_crypto_EXT_STATIC_LIB})
    tbag_modules__add_whole_archive (${ressl_ssl_EXT_STATIC_LIB})
    tbag_modules__add_whole_archive (${ressl_tls_EXT_STATIC_LIB})
endmacro ()

macro (tbag_modules__apply_ext_capnp)
    list (APPEND TBAG_PROJECT_DEPENDENCIES capnpc)
    #list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${capnp_EXT_INCLUDE_DIR}) # Don't use include directory.
    tbag_modules__add_whole_archive (${capnp_EXT_STATIC_LIB})
    tbag_modules__add_whole_archive (${capnp_kj_EXT_STATIC_LIB})
endmacro ()

macro (tbag_modules__build_ext_capnp_cpp __target __capnp)
    set (Capnp_SKIP_FOUND ON)
    find_package (Capnp QUIET)
    unset (Capnp_SKIP_FOUND)

    get_filename_component (__capnp_output_dir ${__capnp} DIRECTORY)
    capnp_generate (__capnp_result "${capnp_COMPILER_BIN}" "${__capnp_output_dir}" c++ "${__capnp}")
    add_custom_target (${__target} DEPENDS capnpc SOURCES "${__capnp_result}")

    list (APPEND TBAG_PROJECT_DEPENDENCIES ${__target})
    list (APPEND TBAG_PROJECT_OBJECTS ${__capnp_result})

    unset (__capnp_output_dir)
    unset (__capnp_result)
endmacro ()

macro (tbag_modules__apply_ext_ogg)
    list (APPEND TBAG_PROJECT_DEPENDENCIES ogg)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${ogg_EXT_INCLUDE_DIR})
    tbag_modules__add_whole_archive (${ogg_EXT_STATIC_LIB})
endmacro ()

macro (tbag_modules__apply_ext_flac)
    list (APPEND TBAG_PROJECT_DEPENDENCIES flac)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${flac_EXT_INCLUDE_DIR})
    tbag_modules__add_whole_archive (${flac_EXT_STATIC_LIB})
    tbag_modules__add_whole_archive (${flacxx_EXT_STATIC_LIB})
endmacro ()

macro (tbag_modules__apply_ext_vorbis)
    list (APPEND TBAG_PROJECT_DEPENDENCIES vorbis)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${vorbis_EXT_INCLUDE_DIR})
    tbag_modules__add_whole_archive (${vorbis_EXT_STATIC_LIB})
    tbag_modules__add_whole_archive (${vorbisenc_EXT_STATIC_LIB})
    tbag_modules__add_whole_archive (${vorbisfile_EXT_STATIC_LIB})
endmacro ()

macro (tbag_modules__apply_ext_freetype)
    list (APPEND TBAG_PROJECT_DEPENDENCIES freetype)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${freetype_EXT_INCLUDE_DIR})
    tbag_modules__add_whole_archive (${freetype_EXT_STATIC_LIB})
endmacro ()

macro (tbag_modules__apply_ext_sfml_ignore_gl_al)
    list (APPEND TBAG_PROJECT_DEPENDENCIES sfml)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${sfml_EXT_INCLUDE_DIR})
    list (APPEND TBAG_PROJECT_DEFINITIONS SFML_STATIC OV_EXCLUDE_STATIC_CALLBACKS FLAC__NO_DLL)

    tbag_modules__add_whole_archive (${sfml_system_EXT_STATIC_LIB})
    tbag_modules__add_whole_archive (${sfml_window_EXT_STATIC_LIB})
    tbag_modules__add_whole_archive (${sfml_graphics_EXT_STATIC_LIB})

    if (WIN32)
        list (APPEND TBAG_PROJECT_LDFLAGS winmm.lib gdi32.lib)
    elseif (UNIX)
        if (APPLE)
            list (APPEND TBAG_PROJECT_LDFLAGS -ObjC
                    -Wl,-framework,Foundation
                    -Wl,-framework,AppKit
                    -Wl,-framework,IOKit
                    -Wl,-framework,Carbon)
        else ()
            list (APPEND TBAG_PROJECT_LDFLAGS -lX11 -lXrandr -ludev)
        endif ()
    endif ()

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
    endif ()
endmacro ()

macro (tbag_modules__apply_ext_tgui)
    list (APPEND TBAG_PROJECT_DEPENDENCIES tgui)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${tgui_EXT_INCLUDE_DIR})
    list (APPEND TBAG_PROJECT_DEFINITIONS TGUI_STATIC)
    tbag_modules__add_whole_archive (${tgui_EXT_STATIC_LIB})
endmacro ()

macro (tbag_modules__apply_ext_luajit)
    list (APPEND TBAG_PROJECT_DEPENDENCIES luajit)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${luajit_EXT_INCLUDE_DIR})
    tbag_modules__add_whole_archive (${luajit_EXT_STATIC_LIB})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (APPEND TBAG_PROJECT_LDFLAGS -lm -ldl)
    endif ()
endmacro ()

macro (tbag_modules__apply_ext_luajit_install_conf __target __output)
    add_custom_command (
            OUTPUT ${__output}
            COMMAND ${CMAKE_COMMAND} -E copy "${luajit_EXT_CONF_HEADER}" "${__output}"
            DEPENDS "${luajit_EXT_CONF_HEADER}")
    add_custom_target (${__target} ALL DEPENDS ${__output})
    list (APPEND TBAG_PROJECT_DEPENDENCIES ${__target})
endmacro ()

macro (tbag_modules__apply_luajit_embedding_exe_flags)
    if (UNIX AND APPLE AND CMAKE_SIZEOF_VOID_P EQUAL 8)
        # If you're building a 64 bit application on OSX which links directly or indirectly against LuaJIT,
        # you need to link your main executable with these flags:
        # Reference (Embedding LuaJIT): http://luajit.org/install.html
        list (APPEND TBAG_PROJECT_LDFLAGS "-Wl,-pagezero_size,10000"
                                          "-Wl,-image_base,100000000"
                                          "-Wl,-image_base,7fff04c4a000")
    endif()
endmacro ()

macro (tbag_modules__apply_ext_bzip2)
    list (APPEND TBAG_PROJECT_DEPENDENCIES bzip2)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${bzip2_EXT_INCLUDE_DIR})
    tbag_modules__add_whole_archive (${bzip2_EXT_STATIC_LIB})
endmacro ()

macro (tbag_modules__apply_ext_lzma)
    list (APPEND TBAG_PROJECT_DEPENDENCIES lzma)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${lzma_EXT_INCLUDE_DIR})
    tbag_modules__add_whole_archive (${lzma_EXT_STATIC_LIB})
endmacro ()

macro (tbag_modules__apply_ext_archive)
    list (APPEND TBAG_PROJECT_DEPENDENCIES archive)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${archive_EXT_INCLUDE_DIR})
    list (APPEND TBAG_PROJECT_DEFINITIONS LIBARCHIVE_STATIC)
    tbag_modules__add_whole_archive (${archive_EXT_STATIC_LIB})
endmacro ()

## ----------------
## Other libraries.
## ----------------

macro (tbag_modules__check_opengl)
    if (NOT OPENGL_FOUND)
        message (WARNING "Not found OpenGL.")
    endif ()
endmacro ()

macro (tbag_modules__apply_opengl)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
    list (APPEND TBAG_PROJECT_LDFLAGS ${OPENGL_LIBRARIES})
endmacro ()

macro (tbag_modules__check_opencl)
    if (NOT OpenCL_FOUND)
        message (WARNING "Not found OpenCL.")
    endif ()
endmacro ()

macro (tbag_modules__apply_opencl)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${OpenCL_INCLUDE_DIR})
    list (APPEND TBAG_PROJECT_LDFLAGS ${OpenCL_LIBRARY})
endmacro ()

macro (tbag_modules__check_openal)
    if (NOT OPENAL_FOUND)
        message (WARNING "Not found OpenAL.")
    endif ()
endmacro ()

macro (tbag_modules__apply_openal)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${OPENAL_INCLUDE_DIR})
    list (APPEND TBAG_PROJECT_LDFLAGS ${OPENAL_LIBRARY})
endmacro ()

macro (tbag_modules__check_cxx_obfuscator)
    if (EXISTS "${TBAG_OBFUS_CONFIG_PATH}")
        message (FATAL_ERROR "Not found ${TBAG_OBFUS_CONFIG_PATH}")
    endif ()
endmacro ()

macro (tbag_modules__build_cxx_obfuscator __original_files)
    obfus_generate_cpp (TBAG_PROJECT_OBJECTS "${__original_files}" "${TBAG_OBFUS_CONFIG_PATH}")
endmacro ()

macro (tbag_modules__check_boost)
    if (NOT Boost_FOUND)
        message (WARNING "Not found Boost.")
    endif ()
    #message (STATUS "Boost system: ${Boost_SYSTEM_FOUND}")
    #message (STATUS "Boost thread: ${Boost_THREAD_FOUND}")
    #message (STATUS "Boost python: ${Boost_PYTHON_FOUND}")
endmacro ()

macro (tbag_modules__apply_boost)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${Boost_LIBRARIES})
endmacro ()

macro (tbag_modules__apply_boost_include)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
endmacro ()

macro (tbag_modules__apply_boost_libs)
    foreach (__lib_cursor ${ARGN})
        string (TOUPPER "${__lib_cursor}" __lib_upper_name)
        list (APPEND TBAG_PROJECT_LDFLAGS "${Boost_${__lib_upper_name}_LIBRARY}")
    endforeach ()
endmacro ()

macro (tbag_modules__check_gflags)
    if (NOT GFlags_FOUND)
        message (WARNING "Not found Google-gflags.")
    endif ()
endmacro ()

macro (tbag_modules__apply_gflags)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${GFlags_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${GFlags_LIBRARIES})
    if (WIN32 AND NOT CYGWIN)
        list (APPEND TBAG_PROJECT_LDFLAGS "shlwapi.lib")
    endif ()
endmacro ()

macro (tbag_modules__check_ffmpeg)
    if (NOT FFmpeg_FOUND)
        message (WARNING "Not found FFmpeg.")
    endif ()
endmacro ()

macro (tbag_modules__apply_ffmpeg)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${FFmpeg_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${FFmpeg_LIBRARIES})
endmacro ()

macro (tbag_modules__check_glog)
    if (NOT GLog_FOUND)
        message (WARNING "Not found Google-glog.")
    endif ()
endmacro ()

macro (tbag_modules__apply_glog)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${GLog_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${GLog_LIBRARIES})
endmacro ()

macro (tbag_modules__check_gtest)
    if (NOT GTEST_FOUND)
        message (WARNING "Not found Google-gtest.")
    endif ()
endmacro ()

macro (tbag_modules__apply_gtest)
    list (APPEND TBAG_PROJECT_LDFLAGS ${GTEST_BOTH_LIBRARIES})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
    endif ()
endmacro ()

macro (tbag_modules__check_hdf5v2)
    if (NOT HDF5v2_FOUND)
        message (WARNING "Not found HDF5.")
    endif ()
endmacro ()

macro (tbag_modules__apply_hdf5v2)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${HDF5v2_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${HDF5v2_LIBRARIES}) # HL, C
endmacro ()

macro (tbag_modules__check_icu)
    if (NOT ICU_FOUND)
        message (WARNING "Not found ICU.")
    endif ()
endmacro ()

macro (tbag_modules__apply_icu)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${ICU_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${ICU_LIBRARIES})
endmacro ()

macro (tbag_modules__check_ncurses)
    if (NOT NCurses_FOUND)
        message (WARNING "Not found NCurses.")
    endif ()
endmacro ()

macro (tbag_modules__apply_ncurses)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${NCurses_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${NCurses_LIBRARIES})
endmacro ()

macro (tbag_modules__check_numpy)
    if (NOT NumPy_FOUND)
        message (WARNING "Not found Python NumPy.")
    endif ()
endmacro ()

macro (tbag_modules__apply_numpy)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${NumPy_INCLUDE_DIRS})
endmacro ()

macro (tbag_modules__check_openblas)
    if (NOT OpenBLAS_FOUND)
        message (WARNING "Not found OpenBLAS.")
    endif ()
endmacro ()

macro (tbag_modules__apply_openblas)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${OpenBLAS_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${OpenBLAS_LIBRARIES})
endmacro ()

macro (tbag_modules__check_opencv)
    if (NOT OpenCV_FOUND)
        message (WARNING "Not found OpenCV.")
    endif ()
endmacro ()

macro (tbag_modules__apply_opencv)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${OpenCV_LIBS})
endmacro ()

macro (tbag_modules__check_protobuf)
    if (NOT PROTOBUF_FOUND)
        message (WARNING "Not found Google-Protocol-buffers.")
    endif ()
endmacro ()

macro (tbag_modules__apply_protobuf)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${PROTOBUF_LIBRARIES})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list (APPEND TBAG_PROJECT_LDFLAGS -lz)
    endif ()
endmacro ()

macro (tbag_modules__build_protobuf_cpp __original_files)
    tbag_protobuf__generate_cpp (__proto_srcs __proto_headers "${__original_files}")
    list (APPEND TBAG_PROJECT_OBJECTS ${__proto_srcs})
endmacro ()

macro (tbag_modules__build_protobuf_py __original_files)
    tbag_protobuf__generate_py (__proto_srcs __proto_headers "${__original_files}")
    #string (MD5 __proto_srcs_hash "${__proto_srcs}")
    #set (__protobuf_python_target_name "${TBAG_PROJECT_CONST_NAME}_protobuf_python_${__proto_srcs_hash}")
    #add_custom_target ("${__protobuf_python_target_name}" ALL DEPENDS ${__proto_srcs})
    #list (APPEND TBAG_OBJECT_DEPENDENCIES "${__protobuf_python_target_name}")
endmacro ()

macro (tbag_modules__check_python_interpreter)
    if (NOT PYTHONINTERP_FOUND)
        message (WARNING "Not found Python interpreter.")
    endif ()
    #message (STATUS "Python interpreter: ${PYTHON_EXECUTABLE}")
    #message (STATUS "Python interpreter version: ${PYTHON_VERSION_STRING}")
endmacro ()

macro (tbag_modules__check_python_libs)
    if (NOT PYTHONLIBS_FOUND)
        message (WARNING "Not found CPython libraries.")
    endif ()
    #message (STATUS "CPython version: ${PYTHONLIBS_VERSION_STRING}")
endmacro ()

macro (tbag_modules__apply_python_libs)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${PYTHON_LIBRARIES})
endmacro ()

macro (tbag_modules__check_sfml)
    if (NOT SFML_FOUND)
        message (WARNING "Not found SFML.")
    endif ()
endmacro ()

macro (tbag_modules__apply_sfml)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${SFML_INCLUDE_DIR})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${SFML_DEPENDENCIES} ${SFML_LIBRARIES})
    if (UNIX AND APPLE)
        list (APPEND TBAG_PROJECT_LDFLAGS  -Wl,-rpath,${SFML_DEPENDENCIES} ${SFML_LIBRARIES})
    endif ()
endmacro ()

macro (tbag_modules__check_spdlog)
    if (NOT Spdlog_FOUND)
        message (WARNING "Not found speed-log.")
    endif ()
endmacro ()

macro (tbag_modules__apply_spdlog)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Spdlog_INCLUDE_DIRS})
endmacro ()

macro (tbag_modules__check_sqlite3)
    if (NOT SQLite3_FOUND)
        message (WARNING "Not found SQLite3.")
    endif ()
endmacro ()

macro (tbag_modules__apply_sqlite3)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${SQLite3_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${SQLite3_LIBRARIES})
endmacro ()

macro (tbag_modules__check_tbag)
    if (NOT Tbag_FOUND)
        message (WARNING "Not found Tbag.")
    endif ()
endmacro ()

macro (tbag_modules__apply_tbag)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Tbag_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${Tbag_LIBRARIES})
    if (Tbag_IS_STATIC)
        tbag_debug (tbag_modules__apply_tbag "Found static tbag library.")
        list (APPEND TBAG_PROJECT_DEFINITIONS TBAG_STATIC_API)
    endif ()
endmacro ()

macro (tbag_modules__apply_tbag_static_api)
    list (APPEND TBAG_PROJECT_DEFINITIONS TBAG_STATIC_API)
endmacro ()

macro (tbag_modules__check_tbui)
    if (NOT Tbui_FOUND)
        message (WARNING "Not found Tbag-ui.")
    endif ()
endmacro ()

macro (tbag_modules__apply_tbui)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Tbui_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${Tbui_LIBRARIES})
endmacro ()

macro (tbag_modules__check_tinyxml2)
    if (NOT TinyXML2_FOUND)
        message (WARNING "Not found TinyXML2.")
    endif ()
endmacro ()

macro (tbag_modules__apply_tinyxml2)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${TinyXML2_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${TinyXML2_LIBRARIES})
endmacro ()

macro (tbag_modules__check_uv)
    if (NOT UV_FOUND)
        message (WARNING "Not found libuv.")
    endif ()
endmacro ()

macro (tbag_modules__apply_uv)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${UV_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${UV_LIBRARIES})
endmacro ()

macro (tbag_modules__check_wxwidgets)
    if (NOT wxWidgets_FOUND)
        message (WARNING "Not found wxWidgets.")
    endif ()
endmacro ()

macro (tbag_modules__apply_wxwidgets)
    list (APPEND TBAG_PROJECT_DEFINITIONS  ${wxWidgets_DEFINITIONS})
    list (APPEND TBAG_PROJECT_CXXFLAGS     ${wxWidgets_CXX_FLAGS})
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${wxWidgets_LIBRARIES})
endmacro ()

## ------------------
## Find object files.
## ------------------

function (tbag_modules__find_files __result __find_dir __suffix)
    set (${__result})

    get_filename_component (__find_dir_absolute "${__find_dir}" ABSOLUTE)
    file (GLOB_RECURSE ${__result} "${__find_dir_absolute}/*${__suffix}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

function (tbag_modules__find_relative_files __result __find_dir __relative __suffix)
    set (${__result})

    get_filename_component (__find_dir_absolute "${__find_dir}" ABSOLUTE)
    file (GLOB_RECURSE ${__result} RELATIVE "${__relative}" "${__find_dir}/*${__suffix}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Find & register object files.
#///
#/// @param __find_dir [in] find directory.
#/// @param __suffix   [in] file suffix.
macro (tbag_modules__update_objects __find_dir __suffix)
    tbag_modules__find_files (__find_compile_objs "${__find_dir}" "${__suffix}")
    list (APPEND TBAG_PROJECT_OBJECTS ${__find_compile_objs})
endmacro ()

macro (tbag_modules__update_subdir_object)
    foreach (__obj_cusor ${ARGN})
        list (APPEND TBAG_PROJECT_OBJECTS "${TBAG_PROJECT_CONST_DIR_PATH}/${__obj_cusor}")
    endforeach ()
endmacro ()

#/// Find & register object files.
#///
#/// @param __find_dir [in] find directory.
macro (tbag_modules__update_c_family_objects __find_dir)
    tbag_modules__update_objects ("${__find_dir}" ".c")
    tbag_modules__update_objects ("${__find_dir}" ".cc")
    tbag_modules__update_objects ("${__find_dir}" ".cpp")
endmacro ()

macro (tbag_modules__update_c_family_subdir_objects)
    foreach (__subdir_cusor ${ARGN})
        tbag_modules__update_c_family_objects ("${TBAG_PROJECT_CONST_DIR_PATH}/${__subdir_cusor}")
    endforeach ()
endmacro ()

macro (tbag_modules__update_default_objects)
    tbag_modules__update_c_family_objects ("${TBAG_PROJECT_CONST_DIR_PATH}")
endmacro ()

macro (tbag_modules__update_cuda_objects)
    tbag_modules__find_files (__find_cuda_objs "${TBAG_PROJECT_CONST_DIR_PATH}" ".cu")
    tbag_modules__build_cuda (${__find_cuda_objs})
endmacro ()

macro (tbag_modules__update_simd_objects)
    tbag_modules__find_relative_files (__find_simd_objs "${TBAG_PROJECT_CONST_DIR_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}" ".simd")
    tbag_modules__build_dep_simdpp (${__find_simd_objs})
endmacro ()

## --------------
## Final process.
## --------------

#/// Dependencies setting.
macro (tbag_modules__update_dependencies_property)
    list (LENGTH TBAG_PROJECT_DEPENDENCIES __tbag_project_dependencies_length)
    if (${__tbag_project_dependencies_length} GREATER 0)
        add_dependencies (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_DEPENDENCIES})
    endif ()
endmacro ()

#/// Define setting.
macro (tbag_modules__update_definitions_property)
    list (LENGTH TBAG_PROJECT_DEFINITIONS __project_definitions_length)
    if (${__project_definitions_length} GREATER 0)
        target_compile_definitions (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_DEFINITIONS})
    endif ()
endmacro ()

#/// Include directories settings.
macro (tbag_modules__update_include_dirs_property)
    list (LENGTH TBAG_PROJECT_INCLUDE_DIRS __project_include_dirs_length)
    if (${__project_include_dirs_length} GREATER 0)
        target_include_directories (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_INCLUDE_DIRS})
    endif ()
endmacro ()

#/// C++ compiler flags.
macro (tbag_modules__update_cxx_flags_property)
    list (LENGTH TBAG_PROJECT_CXXFLAGS __project_cxxflags_length)
    if (${__project_cxxflags_length} GREATER 0)
        if ("${CMAKE_VERSION}" VERSION_GREATER "3.3.0")
            target_compile_options (${TBAG_PROJECT_CONST_NAME} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${TBAG_PROJECT_CXXFLAGS}>)
        else ()
            target_compile_options (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_CXXFLAGS})
        endif ()
    endif ()
endmacro ()

#/// Linker flags.
macro (tbag_modules__update_linker_flags_property)
    list (LENGTH TBAG_PROJECT_LDFLAGS __project_ldflags_length)
    if (${__project_ldflags_length} GREATER 0)
        target_link_libraries (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_LDFLAGS})
    endif ()
endmacro ()

macro (tbag_modules__add_target)
    # List variables.
    tbag_debug_variable (tbag_modules__add_target TBAG_PROJECT_OBJECTS)
    tbag_debug_variable (tbag_modules__add_target TBAG_PROJECT_DEPENDENCIES)
    tbag_debug_variable (tbag_modules__add_target TBAG_PROJECT_DEFINITIONS)
    tbag_debug_variable (tbag_modules__add_target TBAG_PROJECT_INCLUDE_DIRS)
    tbag_debug_variable (tbag_modules__add_target TBAG_PROJECT_CXXFLAGS)
    tbag_debug_variable (tbag_modules__add_target TBAG_PROJECT_LDFLAGS)

    # Constant variables.
    tbag_debug_variable (tbag_modules__add_target TBAG_PROJECT_CONST_DIR_NAME)
    tbag_debug_variable (tbag_modules__add_target TBAG_PROJECT_CONST_TYPE)
    tbag_debug_variable (tbag_modules__add_target TBAG_PROJECT_CONST_NAME)

    # Exists objects.
    if ("${TBAG_PROJECT_OBJECTS}" STREQUAL "")
        message (FATAL_ERROR "Not found ${TBAG_PROJECT_CONST_NAME} object files.")
    endif ()

    # Register object files.
    if ("${TBAG_PROJECT_CONST_TYPE}" STREQUAL "${TBAG_PROJECT_LIBRARY_PREFIX}")
        if ("${TBAG_PROJECT_LIB_TYPE}" STREQUAL "STATIC")
            set_property (GLOBAL APPEND PROPERTY GLOBAL_TBAG_FIND_SUBPROJECT_STATIC_LIBS ${TBAG_PROJECT_CONST_NAME})
        elseif ("${TBAG_PROJECT_LIB_TYPE}" STREQUAL "SHARED")
            set_property (GLOBAL APPEND PROPERTY GLOBAL_TBAG_FIND_SUBPROJECT_SHARED_LIBS ${TBAG_PROJECT_CONST_NAME})
        elseif ("${TBAG_PROJECT_LIB_TYPE}" STREQUAL "MODULE")
            set_property (GLOBAL APPEND PROPERTY GLOBAL_TBAG_FIND_SUBPROJECT_MODULE_LIBS ${TBAG_PROJECT_CONST_NAME})
        endif ()

        add_library (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_LIB_TYPE} ${TBAG_PROJECT_OBJECTS})
    else ()
        set_property (GLOBAL APPEND PROPERTY GLOBAL_TBAG_FIND_SUBPROJECT_EXES ${TBAG_PROJECT_CONST_NAME})
        add_executable (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_OBJECTS})
    endif ()
endmacro ()

#/// Update all of target.
macro (tbag_modules__update_all_properties)
    tbag_modules__update_dependencies_property   ()
    tbag_modules__update_definitions_property    ()
    tbag_modules__update_include_dirs_property   ()
    tbag_modules__update_cxx_flags_property      ()
    tbag_modules__update_linker_flags_property   ()
endmacro ()

macro (tbag_modules__update_version __version __soversion)
    set_target_properties (${TBAG_PROJECT_CONST_NAME} PROPERTIES
                           VERSION   "${__version}"
                           SOVERSION "${__soversion}")
endmacro ()

macro (tbag_modules__update_default_version)
    tbag_modules__update_version (${VERSION} ${SOVERSION})
endmacro ()

macro (tbag_modules__install_target)
    install (TARGETS "${TBAG_PROJECT_CONST_NAME}"
             RUNTIME DESTINATION bin
             LIBRARY DESTINATION lib
             ARCHIVE DESTINATION lib)
endmacro ()

macro (tbag_modules__install_cxx_headers __parent_dir)
    install (DIRECTORY "${__parent_dir}"
             DESTINATION include
             FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
endmacro ()

macro (tbag_modules__install_default_cxx_headers)
    install (DIRECTORY "${TBAG_PROJECT_CONST_DIR_PATH}"
             DESTINATION include
             FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
endmacro ()

macro (tbag_modules__install_default_cxx_headers_ref_ignore_list)
    set (__install_subdirs)
    tbag_utils__subdirs (__subdirs "${TBAG_PROJECT_CONST_DIR_PATH}")
    foreach (__cursor ${__subdirs})
        tbag_utils__is_contained (__is_contain ${__cursor} ${ARGN})
        if (NOT __is_contain)
            list (APPEND __install_subdirs ${__cursor})
        endif ()
        unset (__is_contain)
    endforeach ()
    unset (__subdirs)

    foreach (__cursor ${__install_subdirs})
        install (DIRECTORY "${TBAG_PROJECT_CONST_DIR_PATH}/${__cursor}"
                 DESTINATION "include/${TBAG_PROJECT_CONST_DIR_NAME}/${__cursor}"
                 FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
    endforeach ()
    unset (__install_subdirs)
endmacro ()

