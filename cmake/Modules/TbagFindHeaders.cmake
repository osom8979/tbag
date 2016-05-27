#/// @file   TbagFindHeaders.cmake
#/// @brief  tbag_find_headers function prototype.
#/// @author zer0
#/// @date   2016-05-25

include (TbagDebug)
include (TbagStrings)

#/// Find headers function.
#///
#/// @param __result  [out] Variable result prefix.
#/// @param __paths   [in]  Search directories.
#/// @param __headers [in]  Find header files.
function (tbag_find_headers __result __paths __headers)
    set (${__result})
    set (__name_index 0)

    foreach (__header_cursor ${__headers})
        set (__find_name "${__result}_${__name_index}")
        find_path (${__find_name}
                NAMES ${__header_cursor}
                PATHS ${__paths})
        math (EXPR __name_index "${__name_index} + 1")

        if (IS_DIRECTORY "${${__find_name}}")
            list (APPEND ${__result} "${${__find_name}}")
        endif ()
    endforeach ()

    # Preview
    tabg_strings__arguments_to_string (__preview_result "," ${${__result}})
    tbag_debug (find_headers "Result variable name: ${__result} (${__preview_result})")
    tabg_strings__arguments_to_string (__preview_result "," ${__paths})
    tbag_debug (find_headers "Search directories: ${__preview_result}")
    tabg_strings__arguments_to_string (__preview_result "," ${__headers})
    tbag_debug (find_headers "Find header files: ${__preview_result}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

