#/// @file   TbagProjectProtobufCxx.cmake
#/// @brief  Google-protocol-buffers C++ project module.
#/// @author zer0
#/// @date   2016-06-10

if (NOT PROTOBUF_FOUND)
    message (WARNING "Not found Google-Protocol-buffers.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${PROTOBUF_LIBRARIES} -lz)

