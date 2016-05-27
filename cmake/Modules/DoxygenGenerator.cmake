#/// @file   DoxygenGenerator.cmake
#/// @brief  doxygen_generate function prototype.
#/// @author zer0
#/// @date   2016-05-27

#/// Use Doxygen to create the HTML based API documentation.
#///
#/// @param __doxyfile [in] Doxyfile path.
#/// @param __working  [in] Working directory.
function (doxygen_generate __doxyfile __working)
    if (NOT DOXYGEN_FOUND)
        find_package (Doxygen)
    endif ()

    if (DOXYGEN_FOUND)
        add_custom_target (doxygen ALL "${DOXYGEN_EXECUTABLE}" "${_doxyfile}"
                           WORKING_DIRECTORY "${__working}")
    endif ()
endfunction ()

