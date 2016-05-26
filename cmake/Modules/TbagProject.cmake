#/// @file   TbagProject.cmake
#/// @brief  Tbag project function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagProjectCommon)
include (TbagStrings)

#/// Find project list.
#///
#/// @param __lib_projs  [out] value name of library project list.
#/// @param __exe_projs  [out] value name of executable project list.
#/// @param __root_dir   [in]  Find root directory.
function (find_project __lib_projs __exe_projs __root_dir)
    set (__tbag_project_library_glob    "^${TBAG_PROJECT_LIBRARY_PREFIX}${TBAG_PROJECT_FILE_REGEX}$")
    set (__tbag_project_executable_glob "^${TBAG_PROJECT_EXECUTABLE_PREFIX}${TBAG_PROJECT_FILE_REGEX}$")

    # Find all project.
    file (GLOB_RECURSE __find_project_list RELATIVE "${__root_dir}" "${TBAG_PROJECT_FILE_NAME}")

    # Find library project.
    tbag_list_regex (__find_lib_proj "${__tbag_project_library_glob}" "${__find_project_list}")
    list (LENGTH __find_lib_proj __find_lib_proj_length)

    # Remove library project in the all project.
    if (${__find_lib_proj_length} GREATER 0)
        list (REMOVE_ITEM __find_project_list ${__find_lib_proj})
    endif ()

    # Find executable project.
    tbag_list_regex (__find_exe_proj "${__tbag_project_executable_glob}" "${__find_project_list}")

    string (REPLACE "/${TBAG_PROJECT_FILE_NAME}" "" ${__lib_projs}  "${__find_lib_proj}")
    string (REPLACE "/${TBAG_PROJECT_FILE_NAME}" "" ${__exe_projs}  "${__find_exe_proj}")

    unset (__tbag_project_library_glob)
    unset (__tbag_project_executable_glob)

    # result:
    set (${__lib_projs}  ${${__lib_projs}}  PARENT_SCOPE)
    set (${__exe_projs}  ${${__exe_projs}}  PARENT_SCOPE)
endfunction ()

