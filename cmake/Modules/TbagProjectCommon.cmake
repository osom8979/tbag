#/// @file   TbagProjectCommon.cmake
#/// @brief  Tbag project common variables & functions.
#/// @author zer0
#/// @date   2016-05-26

set (TBAG_PROJECT_FILE_PREFIX  "project")
set (TBAG_PROJECT_FILE_SUFFIX  "cmake")

set (TBAG_PROJECT_LIBRARY_PREFIX     "lib")
set (TBAG_PROJECT_EXECUTABLE_PREFIX  "")

set (TBAG_PROJECT_FILE_NAME    "${TBAG_PROJECT_FILE_PREFIX}.${TBAG_PROJECT_FILE_SUFFIX}")
set (TBAG_PROJECT_FILE_REGEX   "[^/]+/${TBAG_PROJECT_FILE_PREFIX}\\.${TBAG_PROJECT_FILE_SUFFIX}")

#/// Obtain the project type.
#///
#/// @param __result           [out] value name of result type string.
#/// @param __project_dir_name [in]  project directory name.
function (tbag_get_project_type __result __project_dir_name)
    set (${__result})

    if (${__project_dir_name} MATCHES "^${TBAG_PROJECT_LIBRARY_PREFIX}.+")
        set (${__result} "${TBAG_PROJECT_LIBRARY_PREFIX}" PARENT_SCOPE)
    else ()
        set (${__result} "${TBAG_PROJECT_EXECUTABLE_PREFIX}" PARENT_SCOPE)
    endif ()
endfunction()

#/// Obtain the project name.
#///
#/// @param __result           [out] value name of result type string.
#/// @param __project_dir_name [in]  project directory name.
function (tbag_get_project_name __result __project_dir_name)
    set (${__result})
    tbag_get_project_type (__project_type "${__project_dir_name}")

    if ("${__project_type}" STREQUAL "${TBAG_PROJECT_LIBRARY_PREFIX}")
        string (REPLACE "${TBAG_PROJECT_LIBRARY_PREFIX}" "" __temp_name "${__project_dir_name}")
        set (${__result} "${__temp_name}" PARENT_SCOPE)
    else ()
        set (${__result} "${__project_dir_name}" PARENT_SCOPE)
    endif ()

    unset (__project_type)
endfunction ()

## -----------------
## Module operators.
## -----------------

set (TBAG_MODULE_PREFIX "TbagModule")

#/// Include tbag module.
#///
#/// @param __module [in] module name.
macro (tbag_module __module)
    include (${TBAG_MODULE_PREFIX}${__module})
endmacro (tbag_module)

