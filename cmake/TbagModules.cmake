#/// @file   TbagModules.cmake
#/// @brief  Modules utilities.
#/// @author zer0
#/// @date   2016-05-27

include (TbagUtils)

## -----------
## Properties.
## -----------

macro (tbag_modules__preview)
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

## ------------------
## Find object files.
## ------------------

#/// Find & register object files.
#///
#/// @param __find_dir [in] find directory.
#/// @param __suffix   [in] file suffix.
macro (tbag_modules__update_objects __find_dir __suffix)
    get_filename_component (__find_dir_absolute "${__find_dir}" ABSOLUTE)
    file (GLOB_RECURSE __find_compile_objs "${__find_dir_absolute}/*${__suffix}")
    list (APPEND TBAG_PROJECT_OBJECTS ${__find_compile_objs})
endmacro ()

macro (tbag_modules__update_default_objects)
    tbag_modules__update_objects ("${TBAG_PROJECT_CONST_DIR_PATH}" ".cpp")
endmacro ()

## -----------------
## Project settings.
## -----------------

macro (tbag_modules__apply_default)
    string (TOUPPER "${TBAG_PROJECT_CONST_NAME}" __tbag_project_upper_name)
    list (APPEND TBAG_PROJECT_DEFINITIONS  ${__tbag_project_upper_name})
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${TBAG_PROJECT_CONST_DIR_PATH})

    if (UNIX AND APPLE AND IS_DIRECTORY "${THIRD_PREFIX}")
        list (APPEND TBAG_PROJECT_LDFLAGS "-Wl,-rpath,${THIRD_PREFIX}/lib"
                                          "-Wl,-rpath,${THIRD_PREFIX}/Library/Frameworks")
    endif ()
endmacro ()

macro (tbag_modules__apply_pch __original_pch)
    if (NOT EXISTS ${__original_pch})
        message (FATAL_ERROR "Not found original header file path: ${__original_pch}")
    endif ()

    if (NOT DEFINED TbagProjectPCH_NAME)
        get_filename_component (TbagProjectPCH_ORIGINAL_NAME "${__original_pch}" NAME)
        string (REPLACE "." "-" TbagProjectPCH_NAME "${TbagProjectPCH_ORIGINAL_NAME}-pch")
        set (TbagProjectPCH_COPY_PATH ${CMAKE_BINARY_DIR}/${TbagProjectPCH_ORIGINAL_NAME})

        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND MSVC_VERSION)
        if (NOT MSVC_VERSION LESS 1900)
            #list (APPEND TBAG_PROJECT_LDFLAGS "/WHOLEARCHIVE:${__lib_name}") ## ERROR: Auto converting '/' -> '\'
            list (APPEND TBAG_PROJECT_LDFLAGS ${__lib_name})
        else ()
            list (APPEND TBAG_PROJECT_LDFLAGS ${__lib_name})
        endif ()
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        list (APPEND TBAG_PROJECT_LDFLAGS -Wl,-force_load,${__lib_name}) ## All files: -Wl,-all_load
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        list (APPEND TBAG_PROJECT_LDFLAGS -Wl,--whole-archive ${__lib_name} -Wl,--no-whole-archive)
    else ()
        list (APPEND TBAG_PROJECT_LDFLAGS ${__lib_name})
    endif ()
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
    cuda_include_directories (${TBAG_PROJECT_INCLUDE_DIRS})
endmacro ()

macro (tbag_modules__build_cuda __original_files)
    foreach (__cuda_cusor "${__original_files}")
        cuda_compile (__cuda_object ${__cuda_cusor})
        list (APPEND TBAG_PROJECT_OBJECTS ${__cuda_object})
    endforeach ()
endmacro ()

macro (tbag_modules__apply_cublas)
    list (APPEND TBAG_PROJECT_LDFLAGS ${CUDA_CUBLAS_LIBRARIES})
endmacro ()

macro (tbag_modules__apply_cuda)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CUDA_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${CUDA_LIBRARIES})
endmacro ()

macro (tbag_modules__apply_cudnn)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CUDNN_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${CUDNN_LIBRARIES})
endmacro ()

macro (tbag_modules__apply_curand)
    list (APPEND TBAG_PROJECT_LDFLAGS ${CUDA_curand_LIBRARY})
endmacro ()

## --------------
## Dep libraries.
## --------------

macro (tbag_modules__apply_dep_gtest)
    list (APPEND TBAG_PROJECT_DEPENDENCIES gtest_main)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/gtest/include)
    tbag_modules__add_whole_archive ($<TARGET_FILE:gtest_main>)

    if (WIN32)
        list (APPEND TBAG_PROJECT_DEFINITIONS GTEST_OS_WINDOWS)
    else ()

    ## external libraries.
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
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

macro (tbag_modules__apply_dep_jsoncpp)
    list (APPEND TBAG_PROJECT_DEPENDENCIES jsoncpp)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/jsoncpp)
    tbag_modules__add_whole_archive ($<TARGET_FILE:jsoncpp>)
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

macro (tbag_modules__apply_dep_luabridge)
    ## Header-only library.
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/luabridge)
endmacro ()

macro (tbag_modules__apply_dep_lzma)
    list (APPEND TBAG_PROJECT_DEPENDENCIES lzma)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/lzma)
    tbag_modules__add_whole_archive ($<TARGET_FILE:lzma>)
endmacro ()

macro (tbag_modules__apply_dep_sqlite3)
    list (APPEND TBAG_PROJECT_DEPENDENCIES sqlite3)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/sqlite3)
    tbag_modules__add_whole_archive ($<TARGET_FILE:sqlite3>)
endmacro ()

macro (tbag_modules__apply_dep_uv)
    list (APPEND TBAG_PROJECT_DEPENDENCIES uv)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/dep/uv/include)
    tbag_modules__add_whole_archive ($<TARGET_FILE:uv>)

    ## external libraries.
    if (WIN32)
        list (APPEND TBAG_PROJECT_LDFLAGS advapi32.lib iphlpapi.lib psapi.lib shell32.lib
                                          user32.lib userenv.lib winmm.lib ws2_32.lib)
    else ()
        list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
        if (APPLE)
            list (APPEND TBAG_PROJECT_LDFLAGS -ldl)
        else ()
            list (APPEND TBAG_PROJECT_LDFLAGS -lnsl -ldl -lrt)
        endif ()
    endif ()
endmacro ()

## ----------------
## Other libraries.
## ----------------

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
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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
        if ("${CMAKE_VERSION}" VERSION_GREATER "3.0.2")
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
        add_library (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_OBJECTS})
    else ()
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

