#/// @file   TbagFindObject.cmake
#/// @brief  tbag_find_object function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagFindObjectConfig)
include (TbagObjects)

set (TABG_DEFAULT_CXX_SUFFIX ".cpp")

#set (_src_suffix   ".cpp")
#set (_obfus_suffix ".obf.cpp")
#set (_proto_suffix ".proto")
#set (_cuda_suffix  ".cu")

#/// Find compile object.
#///
#/// @param __objs         [out] value name of result object files.
#/// @param __dependencies [out] value name of result dependencies.
#/// @param __definitions  [out] value name of result definitions.
#/// @param __include_dirs [out] value name of result include directories.
#/// @param __cxxflags     [out] value name of result cxx flags.
#/// @param __ldflags      [out] value name of result linker flags.
#/// @param __find_dir     [in]  find directory.
function (tbag_find_object __objs __dependencies __definitions __include_dirs __cxxflags __ldflags __find_dir)
    set (${__objs})
    set (${__dependencies})
    set (${__definitions})
    set (${__include_dirs})
    set (${__cxxflags})
    set (${__ldflags})

    tbag_find_object_config (__config_files "${__find_dir}")
    tbag_debug__list (tbag_project_build__update_objects ${__config_files})
    if ("${__config_files}" STREQUAL "")
        get_filename_component (__find_dir_absolute "${__find_dir}" ABSOLUTE)
        file (GLOB_RECURSE ${__objs} "${__find_dir_absolute}/*${TABG_DEFAULT_CXX_SUFFIX}")
    else ()
        foreach (__config_file_cursor ${__config_files})
            if (EXISTS "${__config_file_cursor}")
                tbag_object__clear ()
                tbag_object__update_const ("${__config_file_cursor}")

                # Call object config file.
                include ("${__config_file_cursor}")

                list (APPEND ${__objs}         ${TBAG_OBJECT_OBJECTS})
                list (APPEND ${__dependencies} ${TBAG_OBJECT_DEPENDENCIES})
                list (APPEND ${__definitions}  ${TBAG_OBJECT_DEFINITIONS})
                list (APPEND ${__include_dirs} ${TBAG_OBJECT_INCLUDE_DIRS})
                list (APPEND ${__cxxflags}     ${TBAG_OBJECT_CXXFLAGS})
                list (APPEND ${__ldflags}      ${TBAG_OBJECT_LDFLAGS})
            else ()
                message (WARNING "Not found ${__config_file_cursor}")
            endif ()
        endforeach()
    endif ()

#    # C++ Obfuscator or Native C/C++ Source files.
#    set (_obfus_excepted_path "${CMAKE_CURRENT_SOURCE_DIR}/excepted.obfus")
#    if (USE_OBFUSCATE AND CXX_OBFUSCATOR_FOUND AND (EXISTS "${_obfus_excepted_path}"))
#        obfus_generate_cpp (_find_obfus "${_find_srcs}" "${_obfus_excepted_path}")
#        list (APPEND ${__objs} ${_find_obfus})
#    else ()
#        list (APPEND ${__objs} ${_find_srcs})
#    endif ()

#    # Google-protocol-buffers files.
#    if (USE_protobuf AND PROTOBUF_FOUND)
#        file (GLOB_RECURSE _find_protos "${_absolute}/*${_proto_suffix}")
#        list (LENGTH _find_protos _find_protos_length)
#        if (${_find_protos_length} GREATER 0)
#            protobuf_generate_cpp2 (_proto_srcs _proto_headers "${_find_protos}")
#            list (APPEND ${__objs} ${_proto_srcs})
#            list (APPEND ${__include_dirs} ${PROTOBUF_INCLUDE_DIRS})
#            list (APPEND ${__ldflags} ${PROTOBUF_LIBRARIES} -lz)
#        endif ()
#    endif ()

#    # NVIDIA CUDA files.
#    if (USE_CUDA AND CUDA_FOUND)
#        file (GLOB_RECURSE _find_cudas "${_absolute}/*${_cuda_suffix}")
#        list (LENGTH _find_cudas _find_cudas_length)
#        if (${_find_cudas_length} GREATER 0)
#            foreach (_cuda_cusor ${_find_cudas})
#                cuda_compile (_cuda_cusor_object ${_cuda_cusor})
#                list (APPEND ${__objs} ${_cuda_cusor_object})
#            endforeach ()
#            list (APPEND ${__include_dirs} ${CUDA_INCLUDE_DIRS})
#            list (APPEND ${__ldflags} ${CUDA_LIBRARIES})
#            if (USE_CUBLAS)
#                list (APPEND ${__ldflags} ${CUDA_CUBLAS_LIBRARIES})
#            endif ()
#        endif ()
#    endif ()

    # update result.
    set (${__objs}         ${${__objs}}         PARENT_SCOPE)
    set (${__dependencies} ${${__dependencies}} PARENT_SCOPE)
    set (${__definitions}  ${${__definitions}}  PARENT_SCOPE)
    set (${__include_dirs} ${${__include_dirs}} PARENT_SCOPE)
    set (${__cxxflags}     ${${__cxxflags}}     PARENT_SCOPE)
    set (${__ldflags}      ${${__ldflags}}      PARENT_SCOPE)
endfunction ()
