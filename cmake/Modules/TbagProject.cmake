#/// @file   TbagProject.cmake
#/// @brief  tbag_project function prototype.
#/// @author zer0
#/// @date   2016-05-27

include (TbagUtils)
include (TbagProjectCommon)
include (TbagFindObject)
include (TbagModules)

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

## --------------
## Project build.
## --------------

#/// Find & register object files.
#///
#/// @param __find_directory [in] find directory path.
macro (tbag_project_build__update_objects __find_directory)
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

    unset (__find_compile_objs)
    unset (__find_compile_dependencies)
    unset (__find_compile_definitions)
    unset (__find_compile_include_dirs)
    unset (__find_compile_cxxflags)
    unset (__find_compile_ldflags)
endmacro ()

#/// Build process.
#///
#/// @param __is_library        [in] YES is library project, NO is executable project.
#/// @param __project_dir_name  [in] Project directory name.
#/// @param __root_dir          [in] Find root directory (Source code directory).
macro (tbag_project_build __is_library __project_dir_name __root_dir)
    set (__project_dir_path    "${__root_dir}/${__project_dir_name}")
    set (__project_cmake_path  "${__project_dir_path}/${TBAG_PROJECT_FILE_NAME}")

    tbag_module__clear ()
    tbag_module__update_const ("${__cursor}")

    project (${TBAG_PROJECT_CONST_NAME})
    include ("${__project_cmake_path}")

    tbag_project_build__update_objects ("${__project_dir_path}")
    if (${__is_library})
        tbag_module__register_object_of_library ()
    else ()
        tbag_module__register_object_of_executable ()
    endif ()
    tbag_module__update_all ()
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

## -----------------
## Export interface.
## -----------------

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

