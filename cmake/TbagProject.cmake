#/// @file   TbagProject.cmak
#/// @brief  Find project files.
#/// @author zer0
#/// @date   2016-05-27

include (TbagUtils)
include (TbagModules)
include (TbagObject)

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

#/// Dependencies setting.
macro (tbag_project__update_dependencies_property)
    list (LENGTH TBAG_PROJECT_DEPENDENCIES __tbag_project_dependencies_length)
    if (${__tbag_project_dependencies_length} GREATER 0)
        add_dependencies (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_DEPENDENCIES})
    endif ()
endmacro ()

#/// Define setting.
macro (tbag_project__update_definitions_property)
    list (LENGTH TBAG_PROJECT_DEFINITIONS __project_definitions_length)
    if (${__project_definitions_length} GREATER 0)
        target_compile_definitions (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_DEFINITIONS})
    endif ()
endmacro ()

#/// Include directories settings.
macro (tbag_project__update_include_dirs_property)
    list (LENGTH TBAG_PROJECT_INCLUDE_DIRS __project_include_dirs_length)
    if (${__project_include_dirs_length} GREATER 0)
        target_include_directories (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_INCLUDE_DIRS})
    endif ()
endmacro ()

#/// C++ compiler flags.
macro (tbag_project__update_cxx_flags_property)
    list (LENGTH TBAG_PROJECT_CXXFLAGS __project_cxxflags_length)
    if (${__project_cxxflags_length} GREATER 0)
        target_compile_options (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_CXXFLAGS})
    endif ()
endmacro ()

#/// Linker flags.
macro (tbag_project__update_linker_flags_property)
    list (LENGTH TBAG_PROJECT_LDFLAGS __project_ldflags_length)
    if (${__project_ldflags_length} GREATER 0)
        target_link_libraries (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_LDFLAGS})
    endif ()
endmacro ()

#/// Target install.
macro (tbag_project__update_target_install_property)
    if (TBAG_PROJECT_FLAG_TARGET_INSTALL)
        install (TARGETS "${TBAG_PROJECT_CONST_NAME}"
                 RUNTIME DESTINATION bin
                 LIBRARY DESTINATION lib
                 ARCHIVE DESTINATION lib)
    endif ()
endmacro ()

#/// Update all of target.
macro (tbag_project__update_all_properties)
    tbag_project__update_dependencies_property   ()
    tbag_project__update_definitions_property    ()
    tbag_project__update_include_dirs_property   ()
    tbag_project__update_cxx_flags_property      ()
    tbag_project__update_linker_flags_property   ()
    tbag_project__update_target_install_property ()
endmacro ()

#/// Find & register object files.
#///
#/// @param __find_directory [in] find directory path.
macro (tbag_project__update_objects __find_directory)
    tbag_object__update (__find_compile_objs
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
#/// @param __root_dir          [in] Find root directory (Source code directory).
#/// @param __project_dir_name  [in] Project directory name.
macro (tbag_project__build __is_library __root_dir __project_dir_name)
    tbag_project__clear_property ()
    tbag_project__set_const_property ("${__root_dir}" "${__project_dir_name}")

    project (${TBAG_PROJECT_CONST_NAME})
    if (EXISTS "${TBAG_PROJECT_CONST_CMAKE_PATH}")
        include ("${TBAG_PROJECT_CONST_CMAKE_PATH}") # Read project.cmake files.
    else ()
        message (FATAL_ERROR "Not found ${TBAG_PROJECT_CONST_CMAKE_PATH}")
    endif ()

    # Flag variables.
    tbag_debug_variable (tbag_project__build TBAG_PROJECT_FLAG_TARGET_INSTALL)
    # List variables.
    tbag_debug_variable (tbag_project__build TBAG_PROJECT_OBJECTS)
    tbag_debug_variable (tbag_project__build TBAG_PROJECT_DEPENDENCIES)
    tbag_debug_variable (tbag_project__build TBAG_PROJECT_DEFINITIONS)
    tbag_debug_variable (tbag_project__build TBAG_PROJECT_INCLUDE_DIRS)
    tbag_debug_variable (tbag_project__build TBAG_PROJECT_CXXFLAGS)
    tbag_debug_variable (tbag_project__build TBAG_PROJECT_LDFLAGS)
    # Constant variables.
    tbag_debug_variable (tbag_project__build TBAG_PROJECT_CONST_DIR_NAME)
    tbag_debug_variable (tbag_project__build TBAG_PROJECT_CONST_TYPE)
    tbag_debug_variable (tbag_project__build TBAG_PROJECT_CONST_NAME)

    tbag_project__update_objects ("${TBAG_PROJECT_CONST_DIR_PATH}")

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

    tbag_project__update_all_properties ()
endmacro ()

#/// Run default build.
#///
#/// @param __libs     [in] List of library.
#/// @param __exes     [in] List of executable.
#/// @param __root_dir [in] Find root directory (Source code directory).
function (tbag_project__auto_build __libs __exes __root_dir)
    # Loop of library project.
    foreach (__cursor ${__libs})
        tbag_project__build (YES "${__root_dir}" "${__cursor}")
    endforeach ()

    # Loop of executable project.
    foreach (__cursor ${__exes})
        tbag_project__build (NO "${__root_dir}" "${__cursor}")
    endforeach ()
endfunction ()

## -------------
## Find project.
## -------------

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

#/// Find & Build tbag project.
#///
#/// @param __root_dir [in] Find root directory (Source code directory).
function (tbag_project__find_and_build __root_dir)
    tbag_project__find (__libs __exes "${__root_dir}")

    message ("** Find library project: ${__libs}")
    message ("** Find executable project: ${__exes}")

    tbag_project__auto_build ("${__libs}" "${__exes}" "${__root_dir}")
endfunction ()

#/// run default tbag project.
macro (tbag_project)
    tbag_project__find_and_build ("${PROJECT_SOURCE_DIR}")
endmacro ()

