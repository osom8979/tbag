#/// @file   TbagDoxygen.cmake
#/// @brief  tbag_doxygen_generate function prototype.
#/// @author zer0
#/// @date   2016-05-27

include (TbagUtils)

#/// Write Doxygen config file.
#///
#/// @param __output_path [in] Output Doxyfile path.
function (tbag_doxygen__generate_doxyfile __output_path)
    tbag_utils__write_configure (Doxyfile.in "${__output_path}")
endfunction ()

#/// Write doxygen index html file.
#///
#/// @param __output_path [in] Output html file path.
function (tbag_doxygen__generate_index __output_path)
    if (EXISTS "${__output_path}")
        file (REMOVE "${__output_path}")
    endif ()

    file (WRITE "${__output_path}" "<meta http-equiv=\"refresh\" content=\"0; url=doxygen/index.html\" />")
endfunction()

#/// Use Doxygen to create the HTML based API documentation.
#///
#/// @param __doxyfile [in] Doxyfile path.
#/// @param __working  [in] Working directory.
function (tbag_doxygen__target __doxyfile __working)
    if (NOT DOXYGEN_FOUND)
        find_package (Doxygen)
    endif ()

    if (DOXYGEN_FOUND)
        add_custom_target (doxygen ALL "${DOXYGEN_EXECUTABLE}" "${__doxyfile}"
                WORKING_DIRECTORY "${__working}")
    endif ()
endfunction ()

#/// Default doxygen generator.
macro (tbag_doxygen_generate)
    tbag_doxygen__generate_doxyfile ("${PROJECT_BINARY_DIR}/Doxyfile")
    tbag_doxygen__generate_index ("${PROJECT_BINARY_DIR}/doxygen.html")
    tbag_doxygen__target ("${PROJECT_BINARY_DIR}/Doxyfile" "${PROJECT_BINARY_DIR}")
endmacro()

