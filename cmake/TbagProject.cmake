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

    if (${__project_dir_name} MATCHES "(^|.*/)${TBAG_PROJECT_LIBRARY_PREFIX}.+")
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
    set (__temp_regex)
    set (__temp_name)

    tbag_project__get_type (__project_type "${__project_dir_name}")

    if ("${__project_type}" STREQUAL "${TBAG_PROJECT_LIBRARY_PREFIX}")
        if ("${TBAG_PROJECT_LIBRARY_PREFIX}" STREQUAL "")
            set (__temp_regex ".*/")
        else ()
            set (__temp_regex "(^|.*/)${TBAG_PROJECT_LIBRARY_PREFIX}")
        endif ()
    else ()
        if ("${TBAG_PROJECT_EXECUTABLE_PREFIX}" STREQUAL "")
            set (__temp_regex ".*/")
        else ()
            set (__temp_regex "(^|.*/)${TBAG_PROJECT_EXECUTABLE_PREFIX}")
        endif ()
    endif ()

    string (REGEX REPLACE "${__temp_regex}" "" __temp_name "${__project_dir_name}")
    set (${__result} "${__temp_name}" PARENT_SCOPE)
endfunction ()

## --------------------------
## Private modules operators.
## --------------------------

#/// Clear project properties.
macro (tbag_project__clear_property)
    if (BUILD_SHARED_LIBS)
        set (TBAG_PROJECT_LIB_TYPE SHARED)
    else ()
        set (TBAG_PROJECT_LIB_TYPE STATIC)
    endif ()

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

    tbag_debug_variable (tbag_project__set_const_property TBAG_PROJECT_CONST_DIR_NAME)
    tbag_debug_variable (tbag_project__set_const_property TBAG_PROJECT_CONST_DIR_PATH)
    tbag_debug_variable (tbag_project__set_const_property TBAG_PROJECT_CONST_CMAKE_PATH)
    tbag_debug_variable (tbag_project__set_const_property TBAG_PROJECT_CONST_TYPE)
    tbag_debug_variable (tbag_project__set_const_property TBAG_PROJECT_CONST_NAME)
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
    set (__library_glob    "(^|.*/)${TBAG_PROJECT_LIBRARY_PREFIX}${TBAG_PROJECT_FILE_REGEX}$")
    set (__executable_glob "(^|.*/)${TBAG_PROJECT_EXECUTABLE_PREFIX}${TBAG_PROJECT_FILE_REGEX}$")

    tbag_debug_variable (tbag_project__find/library_glob ${__library_glob})
    tbag_debug_variable (tbag_project__find/executable_glob ${__executable_glob})

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
#/// @param ...                 [in] Languages.
macro (tbag_project__build __root_dir __project_dir_name)
    tbag_project__clear_property ()
    tbag_project__set_const_property ("${__root_dir}" "${__project_dir_name}")

    project (${TBAG_PROJECT_CONST_NAME} ${ARGN})
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

    foreach (__project_cursor ${__libs} ${__exes})
        tbag_project__build ("${__root_dir}" "${__project_cursor}")
    endforeach ()
endfunction ()

#/// run default tbag project.
macro (tbag_project__find_and_build_default)
    tbag_project__find_and_build ("${PROJECT_SOURCE_DIR}")
endmacro ()

#/// Create default tbag project and global solution target.
#///
#/// @param __name [in] Root project name.
#/// @param ...    [in] Languages.
macro (tbag_project__find_and_build_default_solution __name)
    tbag_project__find_and_build_default ()
    project (${__name} ${ARGN})
    tbag_project__find (__libs __exes "${__root_dir}")
    add_custom_target (${__name} DEPENDS ${__libs} ${__exes})
endmacro ()

#/// run tbag project.
#///
#/// @param NAME ... [in] Root project name.
#/// @param ROOT ... [in] Root source directory.
#/// @param LANG ... [in] Languages.
#/// @param PROJ ... [in] Sub project names.
macro (tbag_project)
    set (__name "${PROJECT_NAME}")
    set (__root_dir "${PROJECT_SOURCE_DIR}")
    set (__lang_list C CXX CXXPCH)
    set (__proj_list)
    set (__argn_switch)

    foreach (__cursor ${ARGN})
        if ("${__cursor}" STREQUAL "NAME")
            set (__argn_switch "NAME")
        elseif ("${__cursor}" STREQUAL "ROOT")
            set (__argn_switch "ROOT")
        elseif ("${__cursor}" STREQUAL "LANG")
            set (__argn_switch "LANG")
        elseif ("${__cursor}" STREQUAL "PROJ")
            set (__argn_switch "PROJ")
        else ()
            if ("${__argn_switch}" STREQUAL "NAME")
                set (__name ${__cursor})
            elseif ("${__argn_switch}" STREQUAL "ROOT")
                set (__root_dir ${__cursor})
            elseif ("${__argn_switch}" STREQUAL "LANG")
                list (APPEND __lang_list ${__cursor})
            elseif ("${__argn_switch}" STREQUAL "PROJ")
                list (APPEND __proj_list ${__cursor})
            else ()
                message (FATAL "tbag_project() argument error: ${ARGN}")
            endif ()
        endif ()
    endforeach ()

    tbag_debug_list (tbag_project/root_dir ${__root_dir})
    tbag_debug_list (tbag_project/lang_list ${__lang_list})
    tbag_debug_list (tbag_project/proj_list ${__proj_list})

    project (${__name} ${__lang_list})
    foreach (__cursor ${__proj_list})
        tbag_project__build ("${__root_dir}" "${__cursor}")
    endforeach ()
    add_custom_target (${__name} DEPENDS ${__proj_list})

    unset (__name)
    unset (__root_dir)
    unset (__lang_list)
    unset (__proj_list)
    unset (__argn_switch)
endmacro ()


