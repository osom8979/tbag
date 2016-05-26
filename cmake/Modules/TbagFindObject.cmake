#/// @file   TbagFindObject.cmake
#/// @brief  tbag_find_object function prototype.
#/// @author zer0
#/// @date   2016-05-26

#! Use Doxygen to create the HTML based API documentation.
#
# \param _config [in] Doxyfile path.
function (doxygen_generate _config)
    if (USE_DOXYGEN AND DOXYGEN_FOUND)
        add_custom_target (doc ALL
                ${DOXYGEN_EXECUTABLE} "${_config}"
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    endif ()
endfunction ()

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

#! find object & library files.
#
# \param _objs         [out] value name of result object files.
# \param _definitions  [out] value name of result defines.
# \param _include_dirs [out] value name of result defines.
# \param _cxxflags     [out] value name of result cxxflags.
# \param _ldflags      [out] value name of result ldflags.
# \param _find_dir     [in]  find directory.
function (find_compile_object _objs _definitions _include_dirs _cxxflags _ldflags _find_dir)
    set (_obj_suffix   ".o")
    set (_src_suffix   ".cpp")
    set (_obfus_suffix ".obf.cpp")
    set (_proto_suffix ".proto")
    set (_cuda_suffix  ".cu")

    set (${_objs})
    set (${_definitions})
    set (${_include_dirs})
    set (${_cxxflags})
    set (${_ldflags})

    # source files.
    get_filename_component (_absolute "${_find_dir}" ABSOLUTE)
    file (GLOB_RECURSE _find_srcs "${_absolute}/*${_src_suffix}")

    # C++ Obfuscator or Native C/C++ Source files.
    set (_obfus_excepted_path "${CMAKE_CURRENT_SOURCE_DIR}/excepted.obfus")
    if (USE_OBFUSCATE AND CXX_OBFUSCATOR_FOUND AND (EXISTS "${_obfus_excepted_path}"))
        obfus_generate_cpp (_find_obfus "${_find_srcs}" "${_obfus_excepted_path}")
        list (APPEND ${_objs} ${_find_obfus})
    else ()
        list (APPEND ${_objs} ${_find_srcs})
    endif ()

    # Google-protocol-buffers files.
    if (USE_protobuf AND PROTOBUF_FOUND)
        file (GLOB_RECURSE _find_protos "${_absolute}/*${_proto_suffix}")
        list (LENGTH _find_protos _find_protos_length)
        if (${_find_protos_length} GREATER 0)
            protobuf_generate_cpp2 (_proto_srcs _proto_headers "${_find_protos}")
            list (APPEND ${_objs} ${_proto_srcs})
            list (APPEND ${_include_dirs} ${PROTOBUF_INCLUDE_DIRS})
            list (APPEND ${_ldflags} ${PROTOBUF_LIBRARIES} -lz)
        endif ()
    endif ()

    # NVIDIA CUDA files.
    if (USE_CUDA AND CUDA_FOUND)
        file (GLOB_RECURSE _find_cudas "${_absolute}/*${_cuda_suffix}")
        list (LENGTH _find_cudas _find_cudas_length)
        if (${_find_cudas_length} GREATER 0)
            foreach (_cuda_cusor ${_find_cudas})
                cuda_compile (_cuda_cusor_object ${_cuda_cusor})
                list (APPEND ${_objs} ${_cuda_cusor_object})
            endforeach ()
            list (APPEND ${_include_dirs} ${CUDA_INCLUDE_DIRS})
            list (APPEND ${_ldflags} ${CUDA_LIBRARIES})
            if (USE_CUBLAS)
                list (APPEND ${_ldflags} ${CUDA_CUBLAS_LIBRARIES})
            endif ()
        endif ()
    endif ()

    # update result.
    set (${_objs}         ${${_objs}}         PARENT_SCOPE)
    set (${_definitions}  ${${_definitions}}  PARENT_SCOPE)
    set (${_include_dirs} ${${_include_dirs}} PARENT_SCOPE)
    set (${_cxxflags}     ${${_cxxflags}}     PARENT_SCOPE)
    set (${_ldflags}      ${${_ldflags}}      PARENT_SCOPE)
endfunction ()
