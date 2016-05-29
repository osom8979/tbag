#/// @file   TbagProject.cmake
#/// @brief  tbag_project function prototype.
#/// @author zer0
#/// @date   2016-05-27

include (TbagFindProject)
include (TbagProjectBuild)

#/// run tbag project.
#///
#/// @param __root_dir [in] Find root directory (Source code directory).
function (tbag_project __root_dir)
    tbag_find_project (__libs __exes "${__root_dir}")

    message ("** Find library project: ${__libs}")
    message ("** Find executable project: ${__exes}")

    tbag_project_build_default ("${__libs}" "${__exes}" "${__root_dir}")
endfunction ()

#/// run default tbag project.
macro (tbag_project__default)
    tbag_project ("${PROJECT_SOURCE_DIR}")
endmacro ()

