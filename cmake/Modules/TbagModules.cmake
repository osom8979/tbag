#/// @file   TbagModules.cmake
#/// @brief  List of tbag modules.
#/// @author zer0
#/// @date   2016-05-27

## ---------------
## Common modules.
## ---------------

#/// Assign default module settings.
#///
#/// @param __define [in] Default define.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module_default __define)
    set (TBAG_PROJECT_DEFINITIONS  -D${__define})
    set (TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
    set (TBAG_PROJECT_LDFLAGS      -L${CMAKE_CURRENT_LIST_DIR})
endmacro ()

#/// Assign soversion property.
#///
#/// @remarsk
#///  Recommended apply to the library project.
macro (tbag_module_soversion)
    set_target_properties (${TBAG_PROJECT_CONST_NAME} PROPERTIES
            VERSION   "${VERSION}"
            SOVERSION "${SOVERSION}")
endmacro ()

#/// Assign header install.
#///
#/// @param __parent_path [in] parent of include directory.
#///
#/// @remarsk
#///  Recommended apply to the library project.
macro (tbag_module_install_headers __parent_path)
    # Header install.
    install (DIRECTORY "${__parent_path}"
             DESTINATION include
             FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
endmacro ()

#/// Assign default install properties.
#///
#/// @remarsk
#///  Recommended apply to the library & executable project.
macro (tbag_module_install_targets)
    set (TBAG_PROJECT_FLAG_TARGET_INSTALL ON)
endmacro ()

## --------------
## Object filter.
## --------------

#/// C++ Obfuscator or Native C/C++ Source files.
#///
#/// @param __obfus_config_path [in] C++ obfuscator config path.
macro (tbag_object_cxx_obfuscator __obfus_config_path)
    if (EXISTS "${__obfus_config_path}")
        message (FATAL_ERROR "Not found ${__obfus_config_path}")
    endif ()

    if (NOT CxxObfuscator_FOUND)
        find_package (CxxObfuscator)
    endif ()

    if (CxxObfuscator_FOUND)
        obfus_generate_cpp (TBAG_OBJECT_OBJECTS "${TBAG_OBJECT_CONST_SOURCES}" "${__obfus_config_path}")
    endif ()
endmacro ()

#/// NVIDIA CUDA files.
macro (tbag_object_cuda)
    if (NOT CUDA_FOUND)
        find_package (CUDA)
    endif ()

    if (CUDA_FOUND)
        foreach (__cuda_cusor "${TBAG_OBJECT_CONST_SOURCES}")
            cuda_compile (__cuda_object ${__cuda_cusor})
            list (APPEND TBAG_OBJECT_OBJECTS ${__cuda_object})
        endforeach ()

        list (APPEND TBAG_OBJECT_INCLUDE_DIRS ${CUDA_INCLUDE_DIRS})
        list (APPEND TBAG_OBJECT_LDFLAGS ${CUDA_LIBRARIES})
        #if (USE_CUBLAS)
        #    list (APPEND ${__ldflags} ${CUDA_CUBLAS_LIBRARIES})
        #endif ()
    endif ()
endmacro ()

#/// Google-protocol-buffers files.
macro (tbag_object_protobuf_cpp)
    if (NOT PROTOBUF_FOUND)
        find_package (Protobuf)
    endif ()

    if (PROTOBUF_FOUND)
        include (TbagProtobuf)
        tbag_protobuf__generate_cpp (__proto_srcs __proto_headers "${TBAG_OBJECT_CONST_SOURCES}")
        list (APPEND TBAG_OBJECT_OBJECTS        ${__proto_srcs})
        list (APPEND TBAG_OBJECT_INCLUDE_DIRS   ${PROTOBUF_INCLUDE_DIRS})
        list (APPEND TBAG_OBJECT_LDFLAGS        ${PROTOBUF_LIBRARIES} -lz)
    endif ()
endmacro ()

#/// Google-protocol-buffers files.
macro (tbag_object_protobuf_py)
    if (NOT PROTOBUF_FOUND)
        find_package (Protobuf)
    endif ()

    if (PROTOBUF_FOUND)
        include (TbagProtobuf)
        tbag_protobuf__generate_py (__proto_srcs __proto_headers "${TBAG_OBJECT_CONST_SOURCES}")

        add_custom_target (${TBAG_PROJECT_CONST_NAME}_python ALL DEPENDS ${__proto_srcs})
        list (APPEND TBAG_OBJECT_DEPENDENCIES ${TBAG_PROJECT_CONST_NAME}_python)
    endif ()
endmacro ()

## ----------------
## Project modules.
## ----------------

#/// Assign google-test libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module_gtest)
    if (NOT GTEST_FOUND)
        find_package (GTest)
    endif ()

    if (GTEST_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_GTEST)
        list (APPEND TBAG_PROJECT_LDFLAGS      ${GTEST_BOTH_LIBRARIES} -lpthread)
    endif ()
endmacro ()

#/// Assign NCurses libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module_ncurses)
    if (NOT NCurses_FOUND)
        find_package (NCurses)
    endif ()

    if (NCurses_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_NCURSES)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${NCurses_INCLUDE_DIRS})
        list (APPEND TBAG_PROJECT_LDFLAGS      ${NCurses_LIBRARIES})
    endif ()
endmacro ()

#/// Assign speed-log libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module_spdlog)
    if (NOT Spdlog_FOUND)
        find_package (Spdlog)
    endif ()

    if (Spdlog_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_SPDLOG)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Spdlog_INCLUDE_DIRS})
    endif ()
endmacro ()

#/// Assign SQLite3 libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module_sqlite3)
    if (NOT SQLite3_FOUND)
        find_package (SQLite3)
    endif ()

    if (SQLite3_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_SQLITE3)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${SQLite3_INCLUDE_DIRS})
        list (APPEND TBAG_PROJECT_LDFLAGS      ${SQLite3_LIBRARIES})
    endif ()
endmacro ()

#/// Assign TinyXML2 libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module_tinyxml2)
    if (NOT TinyXML2_FOUND)
        find_package (TinyXML2)
    endif ()

    if (TinyXML2_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_TINYXML2)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${TinyXML2_INCLUDE_DIRS})
        list (APPEND TBAG_PROJECT_LDFLAGS      ${TinyXML2_LIBRARIES})
    endif ()
endmacro ()

#/// Assign libuv.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module_uv)
    if (NOT UV_FOUND)
        find_package (UV)
    endif ()

    if (UV_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_LIBUV)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${UV_INCLUDE_DIRS})
        list (APPEND TBAG_PROJECT_LDFLAGS      ${UV_LIBRARIES})
    endif ()
endmacro ()

#/// Assign libtbag.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module_tbag)
    if (NOT Tbag_FOUND)
        find_package (Tbag)
    endif ()

    if (Tbag_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_TBAG)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${Tbag_INCLUDE_DIRS})
    endif ()
endmacro ()

