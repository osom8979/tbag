#/// @file   TbagModuleProtobuf.cmake
#/// @brief  Google-protocol-buffers tbag module.
#/// @author zer0
#/// @date   2016-05-29

include (ProtobufGenerator)

set (TABG_OBFUS_SUFFIX ".proto")

#/// Google-protocol-buffers files.
macro (tbag_module_protobuf__object)
    if (NOT PROTOBUF_FOUND)
        find_package (CxxObfuscator)
    endif ()

    if (PROTOBUF_FOUND)
        protobuf_generate_cpp2 (__proto_srcs __proto_headers "${TBAG_OBJECT_CONST_SOURCES}")
        list (APPEND TBAG_OBJECT_OBJECTS        ${__proto_srcs})
        list (APPEND TBAG_OBJECT_INCLUDE_DIRS   ${PROTOBUF_INCLUDE_DIRS})
        list (APPEND TBAG_OBJECT_LDFLAGS        ${PROTOBUF_LIBRARIES} -lz)
    endif ()
endmacro ()

