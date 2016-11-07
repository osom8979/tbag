## Find the C++ Obfuscator program.
#
# The following variables are optionally searched for defaults
#  CxxObfuscator_ROOT
#
# The following are set after configuration is done:
#  CxxObfuscator_FOUND
#  CxxObfuscator_COMPILER

if (CxxObfuscator_FOUND)
    return ()
endif ()

set (CxxObfuscator_SEARCH_PATHS
    "/usr/bin"
    "/usr/local/bin"
    "/Applications/Run\ C++\ Obfuscator.app/Contents/Resources/bin/"
    "${CxxObfuscator_ROOT}"
    "${CxxObfuscator_ROOT}/bin"
    "$ENV{CxxObfuscator_ROOT}"
    "$ENV{CxxObfuscator_ROOT}/bin")

find_program (CxxObfuscator_COMPILER
              NAMES "cxx-obfus"
              PATHS ${CxxObfuscator_SEARCH_PATHS})

set (LOOKED_FOR CxxObfuscator_COMPILER)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (CxxObfuscator DEFAULT_MSG ${LOOKED_FOR})

set (CxxObfuscator_FOUND ${CXXOBFUSCATOR_FOUND})
if (CxxObfuscator_FOUND)
    mark_as_advanced (${LOOKED_FOR})
    message (STATUS "Found C++ Obfuscator (Compiler: ${CxxObfuscator_COMPILER})")
endif ()

# ----------------
# Module function.
# ----------------

#/// Generate obfuscated c/c++ source file.
#///
#/// @param __obfus       [out] value name of result Obfuscated source files.
#/// @param __sources     [in]  original source files.
#/// @param __except_file [in]  file path of exception symbol list.
function (obfus_generate_cpp __obfus __sources __except_file)
    set (${__obfus})
    set (__flags)

    get_filename_component (_absolute_except ${__except_file} ABSOLUTE)

    if (EXISTS ${_absolute_except})
        list (APPEND __flags -x ${_absolute_except})
    endif ()

    foreach (__cursor ${__sources})
        get_filename_component (__absolute  ${__cursor}   ABSOLUTE)
        get_filename_component (__directory ${__absolute} DIRECTORY)
        get_filename_component (__extension ${__absolute} EXT)
        get_filename_component (__name_we   ${__absolute} NAME_WE)

        string (REPLACE "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}" __bin_path ${__directory})

        set (__obfus_extension ".obfus${__extension}")
        set (__cursor_of_obfus_path "${__bin_path}/${__name_we}${__obfus_extension}")

        get_filename_component (__cursor_of_output_dir ${__cursor_of_obfus_path} DIRECTORY)

        if (NOT EXISTS ${__cursor_of_output_dir})
            file (MAKE_DIRECTORY ${__cursor_of_output_dir})
        endif ()

        add_custom_command (
            OUTPUT  "${__cursor_of_obfus_path}"
            COMMAND "${CxxObfuscator_COMPILER}" ${__absolute} -o ${__cursor_of_obfus_path} ${__flags}
            DEPENDS "${__absolute}"
            COMMENT "Running C++ Obfuscator compiler on ${__cursor}" VERBATIM)

        list (APPEND ${__obfus} ${__cursor_of_obfus_path})
    endforeach ()

    set_source_files_properties (${${__obfus}} PROPERTIES GENERATED TRUE)

    # result output.
    set (${__obfus} ${${__obfus}} PARENT_SCOPE)
endfunction ()

