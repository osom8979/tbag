#/// @file   ProtobufGenerator.cmake
#/// @brief  protobuf_generate function prototype.
#/// @author zer0
#/// @date   2016-05-27

#! bug fix version of protobuf_generate_cpp function.
#
# \param _sources [out] value name of result source files.
# \param _headers [out] value name of result header files.
# \param _protos  [in]  list of proto files.
function (protobuf_generate_cpp2 _sources _headers _protos)
    #message ("_sources: ${_sources}")
    #message ("_headers: ${_headers}")
    #message ("_protos:  ${_protos}")

    set (_protoc_include)
    set (${_sources})
    set (${_headers})

    # loop of proto_path list.
    foreach (_dir_cursor ${PROTOBUF_INCLUDE_DIRS})
        get_filename_component (_dir_abs_cursor ${_dir_cursor} ABSOLUTE)
        list (APPEND _protoc_include -I ${_dir_abs_cursor})
    endforeach ()
    foreach (_cursor ${_protos})
        get_filename_component (_absolute ${_cursor} ABSOLUTE)
        get_filename_component (_dir_abs_cursor ${_absolute} DIRECTORY)
        list (APPEND _protoc_include -I ${_dir_abs_cursor})
    endforeach ()

    # loop of protoc command.
    foreach (_cursor ${_protos})
        get_filename_component (_absolute ${_cursor} ABSOLUTE)

        #string (REPLACE "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}" _cursor_of_bin_path "${_absolute}")
        set (_cursor_of_bin_path "${_absolute}")
        string (REPLACE ".proto" ".pb.cc" _cursor_of_cc_path ${_cursor_of_bin_path})
        string (REPLACE ".proto" ".pb.h"  _cursor_of_h_path  ${_cursor_of_bin_path})
        get_filename_component (_output_dir ${_cursor_of_cc_path} DIRECTORY)

        list (APPEND ${_sources} ${_cursor_of_cc_path})
        list (APPEND ${_headers} ${_cursor_of_h_path})

        if (NOT EXISTS ${_output_dir})
            file (MAKE_DIRECTORY ${_output_dir})
        endif ()

        add_custom_command (
                OUTPUT  ${_cursor_of_cc_path}
                ${_cursor_of_h_path}
                COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} --cpp_out ${_output_dir} ${_protoc_include} ${_absolute}
                DEPENDS ${_absolute}
                WORKING_DIRECTORY ${_output_dir}
                COMMENT "Running C++ protocol buffer compiler on ${_cursor}" VERBATIM)
    endforeach ()

    set_source_files_properties (${${_sources}} ${${_headers}} PROPERTIES GENERATED TRUE)

    # source & header result.
    set (${_sources} ${${_sources}} PARENT_SCOPE)
    set (${_headers} ${${_headers}} PARENT_SCOPE)
endfunction ()

