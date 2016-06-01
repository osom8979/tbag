#/// @file   TbagObjectCxxProtobuf.cmake
#/// @brief  Google-protocol-buffers C++ object module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT PROTOBUF_FOUND)
    find_package (Protobuf)
endif ()

if (PROTOBUF_FOUND)
    include (TbagProtobuf)
    tbag_protobuf__generate_cpp (__proto_srcs __proto_headers "${TBAG_OBJECT_CONST_SOURCES}")

    list (APPEND TBAG_OBJECT_OBJECTS      ${__proto_srcs})
    list (APPEND TBAG_OBJECT_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS})
    list (APPEND TBAG_OBJECT_LDFLAGS      ${PROTOBUF_LIBRARIES} -lz)
endif ()

