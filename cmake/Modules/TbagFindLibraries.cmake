#/// @file   TbagFindLibraries.cmake
#/// @brief  tbag_find_libraries function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagUtils)

#/// Find libraries function.
#///
#/// @param __result [out] Variable result prefix.
#/// @param __paths  [in]  Search directories.
#/// @param __libs   [in]  Find library files.
function (tbag_find_libraries __result __paths __libs)
    set (${__result})
    set (__name_index 0)

    foreach (__lib_cursor ${__libs})
        set (__find_name ${__result}_${__name_index})
        find_library (${__find_name}
                NAMES "${__lib_cursor}"
                PATHS ${__paths}
                PATH_SUFFIXES Debug Release)
        math (EXPR __name_index "${__name_index} + 1")

        if (EXISTS "${${__find_name}}")
            list (APPEND ${__result} "${${__find_name}}")
        endif ()
    endforeach ()

    # Preview
    tabg_utils__arguments_to_string (__preview_result "," ${${__result}})
    tbag_debug (find_libraries "Result variable name: ${__result} (${__preview_result})")
    tabg_utils__arguments_to_string (__preview_result "," ${__paths})
    tbag_debug (find_libraries "Search directories: ${__preview_result}")
    tabg_utils__arguments_to_string (__preview_result "," ${__libs})
    tbag_debug (find_libraries "Find library files: ${__preview_result}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

