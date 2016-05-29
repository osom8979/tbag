#/// @file   TbagFindObjectConfig.cmake
#/// @brief  tbag_find_object_config function prototype.
#/// @author zer0
#/// @date   2016-05-29

include (TbagDebug)

set (TBAG_OBJECT_CONFIG_PREFIX  "object")
set (TBAG_OBJECT_CONFIG_SUFFIX  "cmake")

#/// Obtain object config name.
#///
#/// @param __result      [out] Value name of config file name.
#/// @param __filter_name [in]  filter name.
function (tbag_find_object_config__get_filter_name __result __filter_name)
    set (${__result} "${TBAG_OBJECT_CONFIG_PREFIX}.${__filter_name}.${TBAG_OBJECT_CONFIG_SUFFIX}" PARENT_SCOPE)
endfunction ()

#/// Obtain filter name.
#///
#/// @param __result    [out] Value name of filter name.
#/// @param __file_path [in]  config file path.
function (tbag_find_object_config__get_filter_name __result __file_path)
    set (${__result})

    get_filename_component (__file_name "${__file_path}"  NAME)
    if ("${__file_name}" MATCHES "${TBAG_OBJECT_CONFIG_PREFIX}\\.(.*)\\.${TBAG_OBJECT_CONFIG_SUFFIX}")
        set (__filter_name "${CMAKE_MATCH_1}")
    endif ()

    tbag_debug (tbag_find_object_config__get_filter_name "File name: ${__file_name}")
    tbag_debug (tbag_find_object_config__get_filter_name "Filter name: ${__filter_name}")

    # update result.
    set (${__result} ${__filter_name} PARENT_SCOPE)
endfunction ()

#/// find object config files.
#///
#/// @param __result      [out] Value name of result config list.
#/// @param __project_dir [in]  Project directory.
function (tbag_find_object_config __result __project_dir)
    set (${__result})

    get_filename_component (__absolute "${__project_dir}" ABSOLUTE)
    file (GLOB_RECURSE ${__result} "${__absolute}/${TBAG_OBJECT_CONFIG_PREFIX}*${TBAG_OBJECT_CONFIG_SUFFIX}")

    #list (LENGTH __find_files __find_files_length)
    #if (${__find_files_length} GREATER 0)
    #    foreach (__config_file_cursor ${__find_files})
    #        tbag_find_object_config__get_filter_name (__filter_name "${__config_file_cursor}")
    #        tbag_debug (tbag_find_object_config "File cursor: ${__config_file_cursor}")
    #        tbag_debug (tbag_find_object_config "Filter name: ${__filter_name}")
    #    endforeach ()
    #endif ()

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()
