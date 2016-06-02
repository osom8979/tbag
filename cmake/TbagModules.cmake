#/// @file   TbagModules.cmake
#/// @brief  Modules utilities.
#/// @author zer0
#/// @date   2016-05-27

include (TbagUtils)

set (TBAG_MODULES_OBJECT_CMAKE_PREFIX  TbagObject)
set (TBAG_MODULES_PROJECT_CMAKE_PREFIX TbagProject)

#/// List of module files.
#///
#/// @param __result [out] output value name.
#/// @param __prefix [in]  Prefix of module category.
function (tbag_module__find __result __prefix)
    set (${__result})

    if (NOT IS_DIRECTORY "${TBAG_MODULE_DIR}")
        message (FATAL_ERROR "Not found module directory: ${TBAG_MODULE_DIR}")
    endif()

    get_filename_component (__module_dir_absolute "${TBAG_MODULE_DIR}" ABSOLUTE)
    file (GLOB_RECURSE __modules_absolute "${__module_dir_absolute}/${__prefix}*.cmake")
    if (NOT "${__modules_absolute}" STREQUAL "")
        foreach (__module_cursor ${__modules_absolute})
            get_filename_component (__module_name "${__module_cursor}" NAME)
            if ("${__module_name}" MATCHES "^${__prefix}(.*)\\.cmake$")
                list (APPEND ${__result} "${CMAKE_MATCH_1}")
            endif ()
        endforeach ()
    endif ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// List of object module files.
function (tbag_module__find_objects __result)
    set (${__result})
    tbag_module__find (${__result} ${TBAG_MODULES_OBJECT_CMAKE_PREFIX})
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// List of project module files.
function (tbag_module__find_projects __result)
    set (${__result})
    tbag_module__find (${__result} ${TBAG_MODULES_PROJECT_CMAKE_PREFIX})
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Print object module files.
function (tbag_module__print_objects)
    tbag_module__find_objects (__object_modules)
    message (STATUS "Tbag object modules: ${__object_modules}")
endfunction ()

#/// Print object module files.
function (tbag_module__print_projects)
    tbag_module__find_projects (__project_modules)
    message (STATUS "Tbag project modules: ${__project_modules}")
endfunction ()

#/// Include TbagObject cmake file.
macro (tbag_modules__include_object __file_name)
    include (${TBAG_MODULES_OBJECT_CMAKE_PREFIX}${__file_name})
endmacro ()

#/// Include TbagProject cmake file.
macro (tbag_modules__include_project __file_name)
    include (${TBAG_MODULES_PROJECT_CMAKE_PREFIX}${__file_name})
endmacro ()

