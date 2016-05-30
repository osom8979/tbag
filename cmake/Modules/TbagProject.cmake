#/// @file   TbagProject.cmake
#/// @brief  tbag_project function prototype.
#/// @author zer0
#/// @date   2016-05-27

include (TbagUtils)
include (TbagModules)
include (TbagFindObject)

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

## -------------
## Find project.
## -------------

#/// Find project list.
#///
#/// @param __lib_projs  [out] value name of library project list.
#/// @param __exe_projs  [out] value name of executable project list.
#/// @param __root_dir   [in]  Find root directory (Source code directory).
function (tbag_find_project __lib_projs __exe_projs __root_dir)
    set (__library_glob    "^${TBAG_PROJECT_LIBRARY_PREFIX}${TBAG_PROJECT_FILE_REGEX}$")
    set (__executable_glob "^${TBAG_PROJECT_EXECUTABLE_PREFIX}${TBAG_PROJECT_FILE_REGEX}$")

    # Find all project.
    file (GLOB_RECURSE __find_project_list RELATIVE "${__root_dir}" "${TBAG_PROJECT_FILE_NAME}")

    # Find library project.
    tbag_utils__list_regex (__find_lib_proj "${__library_glob}" "${__find_project_list}")
    list (LENGTH __find_lib_proj __find_lib_proj_length)

    # Remove library project in the all project.
    if (${__find_lib_proj_length} GREATER 0)
        list (REMOVE_ITEM __find_project_list ${__find_lib_proj})
    endif ()

    # Find executable project.
    tbag_utils__list_regex (__find_exe_proj "${__executable_glob}" "${__find_project_list}")

    string (REPLACE "/${TBAG_PROJECT_FILE_NAME}" "" ${__lib_projs}  "${__find_lib_proj}")
    string (REPLACE "/${TBAG_PROJECT_FILE_NAME}" "" ${__exe_projs}  "${__find_exe_proj}")

    # update result.
    set (${__lib_projs}  ${${__lib_projs}}  PARENT_SCOPE)
    set (${__exe_projs}  ${${__exe_projs}}  PARENT_SCOPE)
endfunction ()

## --------------------------
## Private modules operators.
## --------------------------

#/// Clear project properties.
macro (tbag_project_property__clear)
    set (TBAG_PROJECT_OBJECTS)
    set (TBAG_PROJECT_DEPENDENCIES)
    set (TBAG_PROJECT_DEFINITIONS)
    set (TBAG_PROJECT_INCLUDE_DIRS)
    set (TBAG_PROJECT_CXXFLAGS)
    set (TBAG_PROJECT_LDFLAGS)

    # constant variables.
    set (TBAG_PROJECT_CONST_DIR_NAME)
    set (TBAG_PROJECT_CONST_TYPE)
    set (TBAG_PROJECT_CONST_NAME)
endmacro ()

#/// Clear project properties.
macro (tbag_project_property__print)
    message (STATUS "TBAG_PROJECT_OBJECTS: ${TBAG_PROJECT_OBJECTS}")
    message (STATUS "TBAG_PROJECT_DEPENDENCIES: ${TBAG_PROJECT_DEPENDENCIES}")
    message (STATUS "TBAG_PROJECT_DEFINITIONS: ${TBAG_PROJECT_DEFINITIONS}")
    message (STATUS "TBAG_PROJECT_INCLUDE_DIRS: ${TBAG_PROJECT_INCLUDE_DIRS}")
    message (STATUS "TBAG_PROJECT_CXXFLAGS: ${TBAG_PROJECT_CXXFLAGS}")
    message (STATUS "TBAG_PROJECT_LDFLAGS: ${TBAG_PROJECT_LDFLAGS}")

    # constant variables.
    message (STATUS "TBAG_PROJECT_CONST_DIR_NAME: ${TBAG_PROJECT_CONST_DIR_NAME}")
    message (STATUS "TBAG_PROJECT_CONST_TYPE: ${TBAG_PROJECT_CONST_TYPE}")
    message (STATUS "TBAG_PROJECT_CONST_NAME: ${TBAG_PROJECT_CONST_NAME}")
endmacro ()

#/// Update constant variables.
#///
#/// @param __project_dir_name [in] Project directory name.
macro (tbag_project_property__set_const __project_dir_name)
    set (TBAG_PROJECT_CONST_DIR_NAME "${__project_dir_name}")
    tbag_project__get_type (TBAG_PROJECT_CONST_TYPE "${__project_dir_name}")
    tbag_project__get_name (TBAG_PROJECT_CONST_NAME "${__project_dir_name}")
endmacro ()

#/// Dependencies setting.
macro (tbag_project_property__update_dependencies)
    list (LENGTH TBAG_PROJECT_DEPENDENCIES __tbag_project_dependencies_length)
    if (${__tbag_project_dependencies_length} GREATER 0)
        add_dependencies (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_DEPENDENCIES})
    endif ()
endmacro ()

#/// Define setting.
macro (tbag_project_property__update_definitions)
    list (LENGTH TBAG_PROJECT_DEFINITIONS __project_definitions_length)
    if (${__project_definitions_length} GREATER 0)
        target_compile_definitions (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_DEFINITIONS})
    endif ()
endmacro ()

#/// Include directories settings.
macro (tbag_project_property__update_include)
    list (LENGTH TBAG_PROJECT_INCLUDE_DIRS __project_include_dirs_length)
    if (${__project_include_dirs_length} GREATER 0)
        target_include_directories (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_INCLUDE_DIRS})
    endif ()
endmacro ()

#/// C++ compiler flags.
macro (tbag_project_property__update_cxx_flags)
    list (LENGTH TBAG_PROJECT_CXXFLAGS __project_cxxflags_length)
    if (${__project_cxxflags_length} GREATER 0)
        target_compile_options (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_CXXFLAGS})
    endif ()
endmacro ()

#/// Linker flags.
macro (tbag_project_property__update_linker_flags)
    list (APPEND TBAG_PROJECT_LDFLAGS ${TBAG_PROJECT_DEPENDENCIES})
    list (LENGTH TBAG_PROJECT_LDFLAGS __project_ldflags_length)
    if (${__project_ldflags_length} GREATER 0)
        target_link_libraries (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_LDFLAGS})
    endif ()
endmacro ()

#/// Update all of target.
macro (tbag_project_property__update_all)
    tbag_project_property__update_dependencies ()
    tbag_project_property__update_definitions  ()
    tbag_project_property__update_include      ()
    tbag_project_property__update_cxx_flags    ()
    tbag_project_property__update_linker_flags ()
endmacro ()

#/// Find & register object files.
#///
#/// @param __find_directory [in] find directory path.
macro (tbag_project_property__update_objects __find_directory)
    tbag_find_object (__find_compile_objs
                      __find_compile_dependencies
                      __find_compile_definitions
                      __find_compile_include_dirs
                      __find_compile_cxxflags
                      __find_compile_ldflags
                      "${__find_directory}")

    # Result assign.
    list (APPEND TBAG_PROJECT_OBJECTS      ${__find_compile_objs})
    list (APPEND TBAG_PROJECT_DEPENDENCIES ${__find_compile_dependencies})
    list (APPEND TBAG_PROJECT_DEFINITIONS  ${__find_compile_definitions})
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${__find_compile_include_dirs})
    list (APPEND TBAG_PROJECT_CXXFLAGS     ${__find_compile_cxxflags})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${__find_compile_ldflags})
endmacro ()

## --------------
## Project build.
## --------------

#/// Build process.
#///
#/// @param __is_library        [in] YES is library project, NO is executable project.
#/// @param __project_dir_name  [in] Project directory name.
#/// @param __root_dir          [in] Find root directory (Source code directory).
macro (tbag_project_build __is_library __project_dir_name __root_dir)
    set (__project_dir_path    "${__root_dir}/${__project_dir_name}")
    set (__project_cmake_path  "${__project_dir_path}/${TBAG_PROJECT_FILE_NAME}")

    tbag_project_property__clear ()
    tbag_project_property__set_const ("${__cursor}")

    project (${TBAG_PROJECT_CONST_NAME})
    include ("${__project_cmake_path}") # Read project.cmake files.

    tbag_project_property__update_objects ("${__project_dir_path}")

    # Exists objects.
    if ("${TBAG_PROJECT_OBJECTS}" STREQUAL "")
        message (FATAL_ERROR "Not found ${TBAG_PROJECT_CONST_NAME} object files.")
    endif ()

    # Register object files.
    if (${__is_library})
        add_library (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_OBJECTS})
    else ()
        add_executable (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_OBJECTS})
    endif ()

    tbag_project_property__update_all ()
endmacro ()

#/// Run default build.
#///
#/// @param __libs     [in] List of library.
#/// @param __exes     [in] List of executable.
#/// @param __root_dir [in] Find root directory (Source code directory).
function (tbag_project_build_default __libs __exes __root_dir)
    # Loop of library project.
    foreach (__cursor ${__libs})
        tbag_project_build (YES "${__cursor}" "${__root_dir}")
    endforeach ()

    # Loop of executable project.
    foreach (__cursor ${__exes})
        tbag_project_build (NO "${__cursor}" "${__root_dir}")
    endforeach ()
endfunction ()

#/// Find & Build tbag project.
#///
#/// @param __root_dir [in] Find root directory (Source code directory).
function (tbag_project__find_and_build __root_dir)
    tbag_find_project (__libs __exes "${__root_dir}")

    message ("** Find library project: ${__libs}")
    message ("** Find executable project: ${__exes}")

    tbag_project_build_default ("${__libs}" "${__exes}" "${__root_dir}")
endfunction ()

#/// run default tbag project.
macro (tbag_project)
    tbag_project__find_and_build ("${PROJECT_SOURCE_DIR}")
endmacro ()

