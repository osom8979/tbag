#/// @file   TbagModules.cmake
#/// @brief  List of tbag modules.
#/// @author zer0
#/// @date   2016-05-27

set (TBAG_MODULE_PREFIX "TbagModule")

#/// Include tbag module.
#///
#/// @param __module [in] module name.
macro (tbag_module __module)
    include (${TBAG_MODULE_PREFIX}${__module})
endmacro (tbag_module)

#/// Assign default module settings.
#///
#/// @param __define [in] Default define.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module__assign_default __define)
    set (TBAG_PROJECT_DEFINITIONS  -D${__define})
    set (TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
    set (TBAG_PROJECT_LDFLAGS      -L${CMAKE_CURRENT_LIST_DIR})
endmacro ()

# Extension modules.
tbag_module (GTest)
tbag_module (NCurses)

