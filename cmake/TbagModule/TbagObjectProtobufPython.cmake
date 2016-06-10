#/// @file   TbagObjectProtobufPython.cmake
#/// @brief  Google-protocol-buffers python object module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT PROTOBUF_FOUND)
    message (WARNING "Not found Google-Protocol-buffers.")
endif ()

tbag_protobuf__generate_py (__proto_srcs __proto_headers "${TBAG_OBJECT_CONST_SOURCES}")

string (MD5 __proto_srcs_hash "${__proto_srcs}")
set (__protobuf_python_target_name "${TBAG_PROJECT_CONST_NAME}_protobuf_python_${__proto_srcs_hash}")
add_custom_target ("${__protobuf_python_target_name}" ALL DEPENDS ${__proto_srcs})
list (APPEND TBAG_OBJECT_DEPENDENCIES "${__protobuf_python_target_name}")

