#/// @file   TbagProjectBuild.cmake
#/// @brief  tbag_project_build function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagDebug)
include (TbagProjectCommon)
include (TbagFindObject)
include (TbagModules)

#/// Find & register object files.
#///
#/// @param __find_directory [in] find directory path.
macro (tbag_project_build__update_objects __find_directory)
    find_compile_object (__find_compile_objs
                         __find_compile_definitions
                         __find_compile_include_dirs
                         __find_compile_cxxflags
                         __find_compile_ldflags
                         "${__find_directory}")

    # Result assign.
    list (APPEND TBAG_PROJECT_OBJECTS      ${__find_compile_objs})
    list (APPEND TBAG_PROJECT_DEFINITIONS  ${__find_compile_definitions})
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${__find_compile_include_dirs})
    list (APPEND TBAG_PROJECT_CXXFLAGS     ${__find_compile_cxxflags})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${__find_compile_ldflags})
endmacro ()

#/// Build process.
#///
#/// @param __is_library        [in] YES is library project, NO is executable project.
#/// @param __project_dir_name  [in] Project directory name.
#/// @param __root_dir          [in] Find root directory (Source code directory).
macro (tbag_project_build __is_library __project_dir_name __root_dir)
    set (__project_dir_path    "${__root_dir}/${__project_dir_name}")
    set (__project_cmake_path  "${__project_dir_path}/${TBAG_PROJECT_FILE_NAME}")

    tbag_project_build__clear ()
    tbag_project_build__update_const ("${__cursor}")

    project (${TBAG_PROJECT_CONST_NAME})
    include ("${__project_cmake_path}")

    tbag_project_build__update_objects ("${__project_dir_path}")
    if (${__is_library})
        tbag_project_build__register_object_of_library ()
    else ()
        tbag_project_build__register_object_of_executable ()
    endif ()
    tbag_project_build__update_all ()
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

