#/// @file   FindTbagDoxygen.cmake
#/// @brief  Find the Doxygen program.
#/// @author zer0
#/// @date   2016-05-27
#///
#/// @remarks
#///  The following are set after configuration is done:
#///   - ${DOXYGEN_FOUND}
#///   - ${DOXYGEN_EXECUTABLE}

if (NOT DOXYGEN_FOUND)
    if (TbagDoxygen_FIND_QUIETLY)
        set (__tbag_doxygen_quiet QUIET)
    else ()
        set (__tbag_doxygen_quiet)
    endif ()

    find_package (Doxygen ${__tbag_doxygen_quiet})
endif ()

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
    #if (NOT DOXYGEN_FOUND)
    #    find_package (Doxygen)
    #endif ()

    if (DOXYGEN_FOUND)
        add_custom_target (doxygen ALL "${DOXYGEN_EXECUTABLE}" "${__doxyfile}"
                           WORKING_DIRECTORY "${__working}")
    endif ()
endfunction ()

#/// Use Doxygen to create the HTML based API documentation.
#///
#/// @param __output [in] Output result path.
#/// @param __source [in] Document directory path.
function (tbag_doxygen__target_zip __output __source)
    if (DOXYGEN_FOUND)
        add_custom_target (doxygen_zip ALL "${CMAKE_COMMAND}" -E tar czf "${__output}" "${__source}"
                           DEPENDS doxygen)
    endif ()
endfunction ()

