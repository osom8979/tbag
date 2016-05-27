#/// @file   TbagModuleTinyXML2.cmake
#/// @brief  TinyXML2 tbag cmake module.
#/// @author zer0
#/// @date   2016-05-27

#/// Assign TinyXML2 libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module__assign_tinyxml2)
    if (NOT TINYXML2_FOUND)
        find_package (TinyXML2)
    endif ()

    if (TINYXML2_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_TINYXML2)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${TINYXML2_INCLUDE_DIRS})
        list (APPEND TBAG_PROJECT_LDFLAGS      ${TINYXML2_LIBRARIES})
    endif ()
endmacro ()

