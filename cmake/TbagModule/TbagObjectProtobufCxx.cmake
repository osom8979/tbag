#/// @file   TbagObjectProtobufCxx.cmake
#/// @brief  Google-protocol-buffers C++ object module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT PROTOBUF_FOUND)
    message (WARNING "Not found Google-Protocol-buffers.")
endif ()

tbag_protobuf__generate_cpp (__proto_srcs __proto_headers "${TBAG_OBJECT_CONST_SOURCES}")
list (APPEND TBAG_OBJECT_OBJECTS ${__proto_srcs})

