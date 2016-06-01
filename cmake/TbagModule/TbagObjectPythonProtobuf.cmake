#/// @file   TbagObjectPythonProtobuf.cmake
#/// @brief  Google-protocol-buffers python object module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT PROTOBUF_FOUND)
    find_package (TbagProtobuf)
endif ()

if (PROTOBUF_FOUND)
    tbag_protobuf__generate_py (__proto_srcs __proto_headers "${TBAG_OBJECT_CONST_SOURCES}")

    #add_custom_target ("${TBAG_PROJECT_CONST_NAME}_pythyon" ALL DEPENDS ${__proto_srcs})
    add_custom_target ("${TBAG_PROJECT_CONST_NAME}_${__proto_srcs}" ALL DEPENDS ${__proto_srcs})
    list (APPEND TBAG_OBJECT_DEPENDENCIES ${TBAG_PROJECT_CONST_NAME}_python)
endif ()

