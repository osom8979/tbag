## Find the Cap'n Proto compiler program.
#
# The following variables are optionally searched for defaults
#  Capnp_ROOT
#  Capnp_SKIP_FOUND
#  Capnp_INCLUDE_DIRS
#
# The following are set after configuration is done:
#  Capnp_FOUND
#  Capnp_COMPILER

if (NOT Capnp_SKIP_FOUND)
    if (Capnp_FOUND)
        return ()
    endif ()

    set (Capnp_SEARCH_PATHS
            "/usr/bin"
            "/usr/local/bin"
            "${Capnp_ROOT}"
            "${Capnp_ROOT}/bin"
            "${Capnp_ROOT}/cmake-build-debug"
            "${Capnp_ROOT}/cmake-build-release"
            "${Capnp_ROOT}/build"
            "$ENV{Capnp_ROOT}"
            "$ENV{Capnp_ROOT}/bin"
            "$ENV{Capnp_ROOT}/cmake-build-debug"
            "$ENV{Capnp_ROOT}/cmake-build-release"
            "$ENV{Capnp_ROOT}/build")

    find_program (Capnp_COMPILER
            NAMES "capnp" "tbag-capnp"
            PATHS ${Capnp_SEARCH_PATHS})

    set (LOOKED_FOR Capnp_COMPILER)

    include (FindPackageHandleStandardArgs)
    find_package_handle_standard_args (Capnp DEFAULT_MSG ${LOOKED_FOR})

    set (Capnp_FOUND ${FLATC_FOUND})
    if (Capnp_FOUND)
        mark_as_advanced (${LOOKED_FOR})
        if (NOT Capnp_FIND_QUIETLY)
            message (STATUS "Found capnp (Compiler: ${Capnp_COMPILER})")
        endif ()
    endif ()
endif ()

# ----------------
# Module function.
# ----------------

#/// CapnProto protocol generate function.
#///
#/// @param __result     [out] Value name of result files.
#/// @param __capnp      [in]  Capnp executable path.
#/// @param __output_dir [in]  Prefix PATH to all generated files.
#/// @param __type       [in]  Result file type (only c++)
#/// @param __capnps     [in]  List of capnp files.
function (capnp_generate __result __capnp __output_dir __type __capnps)
    set (${__result})

    set (__output_flag)
    set (__output_extension)
    set (__capnp_include)

    if (NOT EXISTS ${__output_dir})
        file (MAKE_DIRECTORY ${__output_dir})
    endif ()

    if (WIN32 AND NOT CYGWIN AND NOT MSYS)
        set (__path_separator ";")
    else ()
        set (__path_separator ":")
    endif ()

    # Update PATH env.
    get_filename_component (__dir_capnp ${__capnp} DIRECTORY)
    set (__path_env "PATH=${__dir_capnp}${__path_separator}$ENV{PATH}")

    # Select capnp type.
    if ("${__type}" STREQUAL "c++")
        set (__output_extension ".c++")
    else ()
        message (FATAL_ERROR "Unknown CapnProto type: ${__type}")
    endif ()
    set (__output_flag "--output=${__type}:${__output_dir}")

    # loop of capnp_path list.
    foreach (__dir_cursor ${Capnp_INCLUDE_DIRS})
        get_filename_component (__dir_abs_cursor ${__dir_cursor} ABSOLUTE)
        list (APPEND __capnp_include "--import-path=${__dir_abs_cursor}")
    endforeach ()

    # loop of capnp command.
    foreach (__capnp_cursor ${__capnps})
        get_filename_component (__absolute  ${__capnp_cursor} ABSOLUTE)
        get_filename_component (__extension ${__capnp_cursor} EXT)
        get_filename_component (__name      ${__capnp_cursor} NAME_WE)
        set (__output_path "${__output_dir}/${__name}${__extension}${__output_extension}")

        if (NOT "${__extension}" STREQUAL ".capnp")
            message (WARNING "Is it really a CapnProto file? ${__capnp_cursor}")
        endif ()

        add_custom_command (
                OUTPUT  "${__output_path}"
                COMMAND ${CMAKE_COMMAND} -E env "${__path_env}" ${__capnp} compile ${__output_flag} ${__capnp_include} ${__absolute}
                DEPENDS ${__absolute}
                WORKING_DIRECTORY ${__output_dir}
                COMMENT "Running CapnProto compiler on ${__capnp_cursor}" VERBATIM)

        list (APPEND ${__result} "${__output_path}")
    endforeach ()

    set_source_files_properties (${${__result}} PROPERTIES GENERATED TRUE)

    # result files.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// CapnProto protocol generate function.
#///
#/// @param __target     [in] Custom target name.
#/// @param __capnp      [in] Capnp executable path.
#/// @param __output_dir [in] Prefix PATH to all generated files.
#/// @param __type       [in] Result file type (c++)
#/// @param __capnps     [in] List of capnp files.
function (capnp_generate_target __target __capnp __output_dir __type __capnps)
    set (__capnp_result)
    capnp_generate (__capnp_result ${__capnp} ${__output_dir} ${__type} ${__capnps})
    add_custom_target (${__target} SOURCES ${__capnp_result})
endfunction ()

#/// CapnProto protocol generate function.
#///
#/// @remarks
#///  Output to the directory where the '.capnp' files.
#///
#/// @param __target [in] Custom target name.
#/// @param __capnp  [in] Capnp executable path.
#/// @param __type   [in] Result file type (c++)
#/// @param __capnps [in] List of capnp files.
function (capnp_generate_target2 __target __capnp __type __capnps)
    set (__capnp_result)

    foreach (__capnp_cursor ${__capnps})
        set (__capnp_cursor_result)
        get_filename_component (__output_dir ${__capnp_cursor} DIRECTORY)
        capnp_generate (__capnp_cursor_result ${__capnp} ${__output_dir} ${__type} ${__capnp_cursor})
        list (APPEND __capnp_result ${__capnp_cursor_result})
        unset (__capnp_cursor_result)
    endforeach ()

    add_custom_target (${__target} SOURCES ${__capnp_result})
endfunction ()

function (capnp_generate_target3 __target __type __capnps)
    capnp_generate_target2 (${__target} ${Capnp_COMPILER} ${__type} ${__capnps})
endfunction ()

function (capnp_generate_target3_cpp __target __capnps)
    capnp_generate_target2 (${__target} ${Capnp_COMPILER} c++ ${__capnps})
endfunction ()

