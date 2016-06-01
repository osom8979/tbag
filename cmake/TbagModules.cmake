#/// @file   TbagModules.cmake
#/// @brief  List of tbag modules.
#/// @author zer0
#/// @date   2016-05-27

## ---------------
## Common modules.
## ---------------

set (TBAG_MODULES_PROJECT_CMAKE_PREFIX TbagProject)
macro (tbag_modules__include_project __file_name)
    include (${TBAG_MODULES_PROJECT_CMAKE_PREFIX}${__file_name})
endmacro ()

set (TBAG_MODULES_OBJECT_CMAKE_PREFIX TbagObject)
macro (tbag_modules__include_object __file_name)
    include (${TBAG_MODULES_OBJECT_CMAKE_PREFIX}${__file_name})
endmacro ()

#/// Assign soversion property.
#///
#/// @remarsk
#///  Recommended apply to the library project.
macro (tbag_module_soversion)
    set_target_properties (${TBAG_PROJECT_CONST_NAME} PROPERTIES
                           VERSION   "${VERSION}"
                           SOVERSION "${SOVERSION}")
endmacro ()

#/// Assign header install.
#///
#/// @param __parent_path [in] parent of include directory.
#///
#/// @remarsk
#///  Recommended apply to the library project.
macro (tbag_module_install_headers __parent_path)
    # Header install.
    install (DIRECTORY "${__parent_path}"
             DESTINATION include
             FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
endmacro ()

#/// Assign default install properties.
#///
#/// @remarsk
#///  Recommended apply to the library & executable project.
macro (tbag_module_install_targets)
    set (TBAG_PROJECT_FLAG_TARGET_INSTALL ON)
endmacro ()

