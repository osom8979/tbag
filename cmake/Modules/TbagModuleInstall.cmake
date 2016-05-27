#/// @file   TbagModulesInstall.cmake
#/// @brief  Module installer tbag module.
#/// @author zer0
#/// @date   2016-05-27

#/// Assign header install.
#///
#/// @remarsk
#///  Recommended apply to the library project.
macro (tbag_module_install__assign_library __directory)
    # Header install.
    install (DIRECTORY "${__directory}"
             DESTINATION include
             FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
endmacro ()

#/// Assign default install properties.
#///
#/// @remarsk
#///  Recommended apply to the library & executable project.
macro (tbag_module_install__assign_default)
    # Target install.
    install (TARGETS "${TBAG_PROJECT_CONST_NAME}"
             RUNTIME DESTINATION bin
             LIBRARY DESTINATION lib
             ARCHIVE DESTINATION lib)

    # Header install.
    tbag_module_install__assign_library ("${TBAG_PROJECT_CONST_DIR_NAME}")
endmacro ()

