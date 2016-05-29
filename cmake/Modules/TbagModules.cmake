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

#/// Assign soversion property.
#///
#/// @remarsk
#///  Recommended apply to the library project.
macro (tbag_module__assign_soversion)
    set_target_properties (${TBAG_PROJECT_CONST_NAME} PROPERTIES
                           VERSION   "${VERSION}"
                           SOVERSION "${SOVERSION}")
endmacro ()

# Default modules.
tbag_module (Common)
tbag_module (Install)

# Extension modules.
tbag_module (CxxObfuscator)
tbag_module (GTest)
tbag_module (NCurses)
tbag_module (Spdlog)
tbag_module (SQLite3)
tbag_module (TinyXML2)
tbag_module (UV)

