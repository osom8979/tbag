## Find the C++ Obfuscator program.
#
# The following variables are optionally searched for defaults
#  CXX_OBFUSCATOR_ROOT
#
# The following are set after configuration is done:
#  CXX_OBFUSCATOR_FOUND
#  CXX_OBFUSCATOR_COMPILER

set (CXX_OBFUSCATOR_SEARCH_PATHS
    "/usr/bin"
    "/usr/local/bin"
    "/Applications/Run\ C++\ Obfuscator.app/Contents/Resources/bin/"
    "${CXX_OBFUSCATOR_ROOT}"
    "${CXX_OBFUSCATOR_ROOT}/bin"
    "$ENV{CXX_OBFUSCATOR_ROOT}"
    "$ENV{CXX_OBFUSCATOR_ROOT}/bin")

find_program (CXX_OBFUSCATOR_COMPILER
    NAMES "cxx-obfus"
    PATHS ${CXX_OBFUSCATOR_SEARCH_PATHS})

set (LOOKED_FOR CXX_OBFUSCATOR_COMPILER)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (CXX_OBFUSCATOR DEFAULT_MSG ${LOOKED_FOR})

if (CXX_OBFUSCATOR_FOUND)
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found C++ Obfuscator (Compiler: ${CXX_OBFUSCATOR_COMPILER})")
endif ()


# ----------------
# Module function.
# ----------------

#! Generate obfuscated c/c++ source file.
#
# \param _obfus       [out] value name of result Obfuscated source files.
# \param _sources     [in]  original source files.
# \param _except_file [in]  file path of exception symbol list.
function (obfus_generate_cpp _obfus _sources _except_file)
    #message ("_obfus (${_obfus}): ${${_obfus}}")
    #message ("_sources (${_sources}): ${${_sources}}")

    set (${_obfus})
    set (_flags)

    get_filename_component (_absolute_except ${_except_file} ABSOLUTE)

    if (EXISTS ${_absolute_except})
        list (APPEND _flags -x ${_absolute_except})
    endif ()

    foreach (_cursor ${_sources})
        get_filename_component (_absolute  ${_cursor}   ABSOLUTE)
        get_filename_component (_directory ${_absolute} DIRECTORY)
        get_filename_component (_extension ${_absolute} EXT)
        get_filename_component (_name_we   ${_absolute} NAME_WE)

        string (REPLACE "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}" _bin_path ${_directory})

        set (_obfus_extension ".obfus${_extension}")
        set (_cursor_of_obfus_path "${_bin_path}/${_name_we}${_obfus_extension}")

        get_filename_component (_cursor_of_output_dir ${_cursor_of_obfus_path} DIRECTORY)

        if (NOT EXISTS ${_cursor_of_output_dir})
            file (MAKE_DIRECTORY ${_cursor_of_output_dir})
        endif ()

        add_custom_command (
            OUTPUT  ${_cursor_of_obfus_path}
            COMMAND "${CXX_OBFUSCATOR_COMPILER}" ${_absolute} -o ${_cursor_of_obfus_path} ${_flags}
            DEPENDS ${_absolute}
            COMMENT "Running C++ Obfuscator compiler on ${_cursor}" VERBATIM)

        list (APPEND ${_obfus} ${_cursor_of_obfus_path})
    endforeach ()

    set_source_files_properties (${${_obfus}} PROPERTIES GENERATED TRUE)

    # result output.
    set (${_obfus} ${${_obfus}} PARENT_SCOPE)
endfunction ()

