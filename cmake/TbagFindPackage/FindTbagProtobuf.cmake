#/// @file   FindTbagProtobuf.cmake
#/// @brief  Find the Google-protocol-buffers library.
#/// @author zer0
#/// @date   2016-05-27
#
# The following variables are optionally searched for defaults
#  TbagProtobuf_SKIP_FOUND
#  TbagProtobuf_GENERATE_DEPENDS (in tbag_protobuf__generate() function.)

if (NOT TbagProtobuf_SKIP_FOUND)
    if (PROTOBUF_FOUND)
        return ()
    endif ()

    if (TbagProtobuf_FIND_QUIETLY)
        set (__tbag_protobuf_quiet QUIET)
    else ()
        set (__tbag_protobuf_quiet)
    endif ()

    find_package (Protobuf ${__tbag_protobuf_quiet})
endif ()

#/// Tbag version of protobuf_generate function.
#///
#/// @param __sources [out] Value name of result source files.
#/// @param __headers [out] Value name of result header files.
#/// @param __type    [in]  Result file type (cpp, csharp, java, javanano, js, objc, python, ruby).
#/// @param __protos  [in]  List of proto files.
function (tbag_protobuf__generate __sources __headers __type __protos)
    set (${__sources})
    set (${__headers})

    set (__output_flag)
    set (__output_source_extension)
    set (__output_header_extension)
    set (__protoc_include)

    # Select protobuf type.
    if ("${__type}" STREQUAL "cpp")
        set (__output_source_extension ".pb.cc")
        set (__output_header_extension ".pb.h")
    elseif ("${__type}" STREQUAL "csharp")
        set (__output_source_extension ".cs")
    elseif ("${__type}" STREQUAL "java")
        message (FATAL_ERROR "Unsupported protobuf type: ${__type}")
    elseif ("${__type}" STREQUAL "javanano")
        message (FATAL_ERROR "Unsupported protobuf type: ${__type}")
    elseif ("${__type}" STREQUAL "js")
        set (__output_source_extension ".js")
    elseif ("${__type}" STREQUAL "objc")
        set (__output_source_extension ".pbobjc.m")
        set (__output_header_extension ".pbobjc.h")
    elseif ("${__type}" STREQUAL "python")
        set (__output_source_extension "_pb2.py")
    elseif ("${__type}" STREQUAL "ruby")
        set (__output_source_extension ".rb")
    else ()
        message (FATAL_ERROR "Unknown protobuf type: ${__type}")
    endif ()
    set (__output_flag "--${__type}_out")

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
        get_filename_component (__absolute  ${__proto_cursor} ABSOLUTE)
        get_filename_component (__extension ${__proto_cursor} EXT)

        if (NOT "${__extension}" STREQUAL ".proto")
            message (WARNING "Is it really a protobuf file? ${__proto_cursor}")
        endif ()

        string (REPLACE "${__extension}" "${__output_source_extension}" __cursor_of_source_path "${__absolute}")
        list (APPEND ${__sources} ${__cursor_of_source_path})
        get_filename_component (__output_dir ${__cursor_of_source_path} DIRECTORY)

        # Check the result of header file.
        if ("${__output_header_extension}" STREQUAL "")
            set (__cursor_of_header_path)
        else ()
            string (REPLACE "${__extension}" "${__output_header_extension}" __cursor_of_header_path "${__absolute}")
            list (APPEND ${__headers} ${__cursor_of_header_path})
        endif ()

        if (NOT EXISTS ${__output_dir})
            file (MAKE_DIRECTORY ${__output_dir})
        endif ()

        add_custom_command (
                OUTPUT  "${__cursor_of_source_path}" "${__cursor_of_header_path}"
                COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${__output_flag} ${__output_dir} ${__protoc_include} ${__absolute}
                DEPENDS ${__absolute} ${TbagProtobuf_GENERATE_DEPENDS}
                WORKING_DIRECTORY ${__output_dir}
                COMMENT "Running Google-protocol-buffers compiler on ${__proto_cursor}" VERBATIM)
    endforeach ()

    set_source_files_properties (${${__sources}} ${${__headers}} PROPERTIES GENERATED TRUE)

    # source & header result.
    set (${__sources} ${${__sources}} PARENT_SCOPE)
    set (${__headers} ${${__headers}} PARENT_SCOPE)
endfunction ()

macro (tbag_protobuf__generate_cpp __sources __headers __protos)
    tbag_protobuf__generate ("${__sources}" "${__headers}" cpp "${__protos}")
endmacro ()

macro (tbag_protobuf__generate_py __sources __protos)
    tbag_protobuf__generate ("${__sources}" __unused_header__ python "${__protos}")
endmacro ()

