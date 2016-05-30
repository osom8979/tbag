#/// @file   TbagProtobuf.cmake
#/// @brief  protobuf_generate function prototype.
#/// @author zer0
#/// @date   2016-05-27

#/// bug fix version of protobuf_generate_cpp function.
#///
#/// @param __sources [out] value name of result source files.
#/// @param __headers [out] value name of result header files.
#/// @param __protos  [in]  list of proto files.
function (protobuf_generate_cpp2 __sources __headers __protos)
    set (__protoc_include)
    set (${__sources})
    set (${__headers})

    # loop of proto_path list.
    foreach (__dir_cursor ${PROTOBUF_INCLUDE_DIRS})
        get_filename_component (__dir_abs_cursor ${__dir_cursor} ABSOLUTE)
        list (APPEND __protoc_include -I ${__dir_abs_cursor})
    endforeach ()
    foreach (__proto_cursor ${__protos})
        get_filename_component (__absolute ${__proto_cursor} ABSOLUTE)
        get_filename_component (__dir_abs_cursor ${__absolute} DIRECTORY)
        list (APPEND __protoc_include -I ${__dir_abs_cursor})
    endforeach ()

    # loop of protoc command.
    foreach (__proto_cursor ${__protos})
        get_filename_component (__absolute ${__proto_cursor} ABSOLUTE)

        set (__cursor_of_bin_path "${__absolute}")
        string (REPLACE ".proto" ".pb.cc" __cursor_of_cc_path ${__cursor_of_bin_path})
        string (REPLACE ".proto" ".pb.h"  __cursor_of_h_path  ${__cursor_of_bin_path})
        get_filename_component (__output_dir ${__cursor_of_cc_path} DIRECTORY)

        list (APPEND ${__sources} ${__cursor_of_cc_path})
        list (APPEND ${__headers} ${__cursor_of_h_path})

        if (NOT EXISTS ${__output_dir})
            file (MAKE_DIRECTORY ${__output_dir})
        endif ()

        add_custom_command (
                OUTPUT  ${__cursor_of_cc_path}
                ${__cursor_of_h_path}
                COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} --cpp_out ${__output_dir} ${__protoc_include} ${__absolute}
                DEPENDS ${__absolute}
                WORKING_DIRECTORY ${__output_dir}
                COMMENT "Running C++ protocol buffer compiler on ${__proto_cursor}" VERBATIM)
    endforeach ()

    set_source_files_properties (${${__sources}} ${${__headers}} PROPERTIES GENERATED TRUE)

    # source & header result.
    set (${__sources} ${${__sources}} PARENT_SCOPE)
    set (${__headers} ${${__headers}} PARENT_SCOPE)
endfunction ()

