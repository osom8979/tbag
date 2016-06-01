#/// @file   TbagModules.cmake
#/// @brief  List of tbag modules.
#/// @author zer0
#/// @date   2016-05-27

set (TBAG_MODULES_OBJECT_CMAKE_PREFIX  TbagObject)
set (TBAG_MODULES_PROJECT_CMAKE_PREFIX TbagProject)

#/// Include TbagProject cmake file.
macro (tbag_modules__include_project __file_name)
    include (${TBAG_MODULES_PROJECT_CMAKE_PREFIX}${__file_name})
endmacro ()
macro (tbag_include_project __file_name)
    tbag_modules__include_project (${__file_name})
endmacro ()

#/// Include TbagObject cmake file.
macro (tbag_modules__include_object __file_name)
    include (${TBAG_MODULES_OBJECT_CMAKE_PREFIX}${__file_name})
endmacro ()
macro (tbag_include_object __file_name)
    tbag_modules__include_object (${__file_name})
endmacro ()

