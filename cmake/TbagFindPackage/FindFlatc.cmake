## Find the FlatBuffers compiler program.
#
# The following variables are optionally searched for defaults
#  Flatc_ROOT
#  Flatc_SKIP_FOUND
#  Flatc_INCLUDE_DIRS
#
# The following are set after configuration is done:
#  Flatc_FOUND
#  Flatc_COMPILER

if (NOT Flatc_SKIP_FOUND)
    if (Flatc_FOUND)
        return ()
    endif ()

    set (Flatc_SEARCH_PATHS
            "/usr/bin"
            "/usr/local/bin"
            "${Flatc_ROOT}"
            "${Flatc_ROOT}/bin"
            "${Flatc_ROOT}/cmake-build-debug"
            "${Flatc_ROOT}/cmake-build-release"
            "${Flatc_ROOT}/build"
            "$ENV{Flatc_ROOT}"
            "$ENV{Flatc_ROOT}/bin"
            "$ENV{Flatc_ROOT}/cmake-build-debug"
            "$ENV{Flatc_ROOT}/cmake-build-release"
            "$ENV{Flatc_ROOT}/build")

    find_program (Flatc_COMPILER
            NAMES "flatc" "libtbag_flatc"
            PATHS ${Flatc_SEARCH_PATHS})

    set (LOOKED_FOR Flatc_COMPILER)

    include (FindPackageHandleStandardArgs)
    find_package_handle_standard_args (Flatc DEFAULT_MSG ${LOOKED_FOR})

    set (Flatc_FOUND ${FLATC_FOUND})
    if (Flatc_FOUND)
        mark_as_advanced (${LOOKED_FOR})
        if (NOT Flatc_FIND_QUIETLY)
            message (STATUS "Found flatc (Compiler: ${Flatc_COMPILER})")
        endif ()
    endif ()
endif ()

# ----------------
# Module function.
# ----------------

#/// FlatBuffers protocol generate function.
#///
#/// @param __result     [out] Value name of result files.
#/// @param __flatc      [in]  Flatc executable path.
#/// @param __output_dir [in]  Prefix PATH to all generated files.
#/// @param __type       [in]  Result file type (cpp, js)
#/// @param __fbs        [in]  List of fbs files.
function (flatc_generate __result __flatc __output_dir __type __fbs)
    set (${__result})

    set (__output_flag)
    set (__output_extension)
    set (__flatc_include)

    if (NOT EXISTS ${__output_dir})
        file (MAKE_DIRECTORY ${__output_dir})
    endif ()

    # Select flatc type.
    if ("${__type}" STREQUAL "cpp")
        set (__output_extension "_generated.h")
    elseif ("${__type}" STREQUAL "js")
        set (__output_extension "_generated.js")
    else ()
        message (FATAL_ERROR "Unknown FlatBuffers type: ${__type}")
    endif ()
    set (__output_flag "--${__type}")

    # loop of flatc_path list.
    foreach (__dir_cursor ${Flatc_INCLUDE_DIRS})
        get_filename_component (__dir_abs_cursor ${__dir_cursor} ABSOLUTE)
        list (APPEND __flatc_include -I ${__dir_abs_cursor})
    endforeach ()

    # loop of flatc command.
    foreach (__fbs_cursor ${__fbs})
        get_filename_component (__absolute  ${__fbs_cursor} ABSOLUTE)
        get_filename_component (__extension ${__fbs_cursor} EXT)
        get_filename_component (__name      ${__fbs_cursor} NAME_WE)
        set (__output_path "${__output_dir}/${__name}${__output_extension}")

        if (NOT "${__extension}" STREQUAL ".fbs")
            message (WARNING "Is it really a FlatBuffers file? ${__fbs_cursor}")
        endif ()

        add_custom_command (
                OUTPUT  "${__output_path}"
                COMMAND ${__flatc} ${__output_flag} -o ${__output_dir} ${__flatc_include} ${__absolute}
                DEPENDS ${__absolute}
                WORKING_DIRECTORY ${__output_dir}
                COMMENT "Running FlatBuffers compiler on ${__fbs_cursor}" VERBATIM)

        list (APPEND ${__result} "${__output_path}")
    endforeach ()

    set_source_files_properties (${${__result}} PROPERTIES GENERATED TRUE)

    # result files.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// FlatBuffers protocol generate function.
#///
#/// @param __target     [in] Custom target name.
#/// @param __flatc      [in] Flatc executable path.
#/// @param __output_dir [in] Prefix PATH to all generated files.
#/// @param __type       [in] Result file type (cpp, js)
#/// @param __fbs        [in] List of fbs files.
function (flatc_generate_target __target __flatc __output_dir __type __fbs)
    set (__fbs_result)
    flatc_generate (__fbs_result ${__flatc} ${__output_dir} ${__type} ${__fbs})
    add_custom_target (${__target} SOURCES ${__fbs_result})
endfunction ()

#/// FlatBuffers protocol generate function.
#///
#/// @remarks
#///  Output to the directory where the '.fbs' files.
#///
#/// @param __target [in] Custom target name.
#/// @param __flatc  [in] Flatc executable path.
#/// @param __type   [in] Result file type (cpp, js)
#/// @param __fbs    [in] List of fbs files.
function (flatc_generate_target2 __target __flatc __type __fbs)
    set (__fbs_result)

    foreach (__fbs_cursor ${__fbs})
        set (__fbs_cursor_result)
        get_filename_component (__output_dir ${__fbs_cursor} DIRECTORY)
        flatc_generate (__fbs_cursor_result ${__flatc} ${__output_dir} ${__type} ${__fbs_cursor})
        list (APPEND __fbs_result ${__fbs_cursor_result})
        unset (__fbs_cursor_result)
    endforeach ()

    add_custom_target (${__target} SOURCES ${__fbs_result})
endfunction ()

function (flatc_generate_target3 __target __type __fbs)
    flatc_generate_target2 (${__target} ${Flatc_COMPILER} ${__type} ${__fbs})
endfunction ()

function (flatc_generate_target3_cpp __target __fbs)
    flatc_generate_target2 (${__target} ${Flatc_COMPILER} cpp ${__fbs})
endfunction ()

function (flatc_generate_target3_js __target __fbs)
    flatc_generate_target2 (${__target} ${Flatc_COMPILER} js ${__fbs})
endfunction ()

