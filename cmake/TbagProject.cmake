#/// @file   TbagProject.cmak
#/// @brief  Find project files.
#/// @author zer0
#/// @date   2016-05-27

include (TbagUtils)
include (TbagModules)

## -------------------
## Project properties.
## -------------------

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
function (tbag_project__get_type __result __project_dir_name)
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
function (tbag_project__get_name __result __project_dir_name)
    set (${__result})
    tbag_project__get_type (__project_type "${__project_dir_name}")

    if ("${__project_type}" STREQUAL "${TBAG_PROJECT_LIBRARY_PREFIX}")
        string (REPLACE "${TBAG_PROJECT_LIBRARY_PREFIX}" "" __temp_name "${__project_dir_name}")
        set (${__result} "${__temp_name}" PARENT_SCOPE)
    else ()
        set (${__result} "${__project_dir_name}" PARENT_SCOPE)
    endif ()
endfunction ()

## --------------------------
## Private modules operators.
## --------------------------

#/// Clear project properties.
macro (tbag_project__clear_property)
    # Flag variables.
    set (TBAG_PROJECT_FLAG_TARGET_INSTALL OFF)

    # List variables.
    set (TBAG_PROJECT_OBJECTS)
    set (TBAG_PROJECT_DEPENDENCIES)
    set (TBAG_PROJECT_DEFINITIONS)
    set (TBAG_PROJECT_INCLUDE_DIRS)
    set (TBAG_PROJECT_CXXFLAGS)
    set (TBAG_PROJECT_LDFLAGS)

    # Constant variables.
    set (TBAG_PROJECT_CONST_CMAKE_PATH)
    set (TBAG_PROJECT_CONST_DIR_PATH)
    set (TBAG_PROJECT_CONST_DIR_NAME)
    set (TBAG_PROJECT_CONST_TYPE)
    set (TBAG_PROJECT_CONST_NAME)
endmacro ()

#/// Update constant variables.
#///
#/// @param __root_dir          [in] Find root directory (Source code directory).
#/// @param __project_dir_name [in] Project directory name.
macro (tbag_project__set_const_property __root_dir __project_dir_name)
    set (TBAG_PROJECT_CONST_DIR_NAME    "${__project_dir_name}")
    set (TBAG_PROJECT_CONST_DIR_PATH    "${__root_dir}/${__project_dir_name}")
    set (TBAG_PROJECT_CONST_CMAKE_PATH  "${TBAG_PROJECT_CONST_DIR_PATH}/${TBAG_PROJECT_FILE_NAME}")

    tbag_project__get_type (TBAG_PROJECT_CONST_TYPE "${__project_dir_name}")
    tbag_project__get_name (TBAG_PROJECT_CONST_NAME "${__project_dir_name}")
endmacro ()

## ---------------------
## Project find & build.
## ---------------------

#/// Find project list.
#///
#/// @param __lib_projs  [out] value name of library project list.
#/// @param __exe_projs  [out] value name of executable project list.
#/// @param __root_dir   [in]  Find root directory (Source code directory).
function (tbag_project__find __lib_projs __exe_projs __root_dir)
    set (__library_glob    "^${TBAG_PROJECT_LIBRARY_PREFIX}${TBAG_PROJECT_FILE_REGEX}$")
    set (__executable_glob "^${TBAG_PROJECT_EXECUTABLE_PREFIX}${TBAG_PROJECT_FILE_REGEX}$")

    # Find all project.
    file (GLOB_RECURSE __find_project_list RELATIVE "${__root_dir}" "${TBAG_PROJECT_FILE_NAME}")

    tbag_debug_list (tbag_project__find/find_project_list ${__find_project_list})

    # Find library project.
    tbag_utils__list_of_regex (__find_lib_proj "${__library_glob}" ${__find_project_list})
    list (LENGTH __find_lib_proj __find_lib_proj_length)

    # Remove library project in the all project.
    if (${__find_lib_proj_length} GREATER 0)
        list (REMOVE_ITEM __find_project_list ${__find_lib_proj})
    endif ()

    # Find executable project.
    tbag_utils__list_of_regex (__find_exe_proj "${__executable_glob}" ${__find_project_list})

    string (REPLACE "/${TBAG_PROJECT_FILE_NAME}" "" ${__lib_projs}  "${__find_lib_proj}")
    string (REPLACE "/${TBAG_PROJECT_FILE_NAME}" "" ${__exe_projs}  "${__find_exe_proj}")

    # update result.
    set (${__lib_projs}  ${${__lib_projs}}  PARENT_SCOPE)
    set (${__exe_projs}  ${${__exe_projs}}  PARENT_SCOPE)
endfunction ()

#/// Build process.
#///
#/// @param __root_dir          [in] Find root directory (Source code directory).
#/// @param __project_dir_name  [in] Project directory name.
macro (tbag_project__build __root_dir __project_dir_name)
    tbag_project__clear_property ()
    tbag_project__set_const_property ("${__root_dir}" "${__project_dir_name}")

    project (${TBAG_PROJECT_CONST_NAME})
    if (EXISTS "${TBAG_PROJECT_CONST_CMAKE_PATH}")
        include ("${TBAG_PROJECT_CONST_CMAKE_PATH}") # Read project.cmake files.
    else ()
        message (FATAL_ERROR "Not found ${TBAG_PROJECT_CONST_CMAKE_PATH}")
    endif ()
endmacro ()

#/// Find & Build tbag project.
#///
#/// @param __root_dir [in] Find root directory (Source code directory).
function (tbag_project__find_and_build __root_dir)
    tbag_project__find (__libs __exes "${__root_dir}")

    message ("** Find library project: ${__libs}")
    message ("** Find executable project: ${__exes}")

    foreach (__project_cursor ${__libs} ${__exes})
        tbag_project__build ("${__root_dir}" "${__project_cursor}")
    endforeach ()
endfunction ()

#/// run default tbag project.
macro (tbag_project)
    tbag_project__find_and_build ("${PROJECT_SOURCE_DIR}")
endmacro ()

