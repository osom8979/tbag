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
#/// @remarsk
#///  Recommended apply to the library project.
macro (tbag_module_install__assign_library __directory)
    # Header install.
    install (DIRECTORY "${__directory}"
            DESTINATION include
            FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
endmacro ()

#/// Assign default install properties.
#///
#/// @remarsk
#///  Recommended apply to the library & executable project.
macro (tbag_module_install__assign_default)
    # Target install.
    install (TARGETS "${TBAG_PROJECT_CONST_NAME}"
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)

    # Header install.
    tbag_module_install__assign_library ("${TBAG_PROJECT_CONST_DIR_NAME}")
endmacro ()

## -------------------------
## Extension library Object.
## -------------------------

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
macro (tbag_object_protobuf)
    if (NOT PROTOBUF_FOUND)
        find_package (CxxObfuscator)
    endif ()

    if (PROTOBUF_FOUND)
        include (ProtobufGenerator)
        protobuf_generate_cpp2 (__proto_srcs __proto_headers "${TBAG_OBJECT_CONST_SOURCES}")
        list (APPEND TBAG_OBJECT_OBJECTS        ${__proto_srcs})
        list (APPEND TBAG_OBJECT_INCLUDE_DIRS   ${PROTOBUF_INCLUDE_DIRS})
        list (APPEND TBAG_OBJECT_LDFLAGS        ${PROTOBUF_LIBRARIES} -lz)
    endif ()
endmacro ()

## --------------------------
## Extension library modules.
## --------------------------

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


