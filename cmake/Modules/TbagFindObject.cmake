#/// @file   TbagFindObject.cmake
#/// @brief  tbag_find_object function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagUtils)
include (TbagModules)

## ---------------------------------
## Common object property operators.
## ---------------------------------

#/// Clear object properties.
macro (tbag_object__clear)
    set (TBAG_OBJECT_OBJECTS)
    set (TBAG_OBJECT_DEPENDENCIES)
    set (TBAG_OBJECT_DEFINITIONS)
    set (TBAG_OBJECT_INCLUDE_DIRS)
    set (TBAG_OBJECT_CXXFLAGS)
    set (TBAG_OBJECT_LDFLAGS)

    # constant variables.
    set (TBAG_OBJECT_CONST_FILTER)
    set (TBAG_OBJECT_CONST_NAME)
    set (TBAG_OBJECT_CONST_DIR)
    set (TBAG_OBJECT_CONST_SOURCES)
endmacro ()

#/// Print object properties.
macro (tbag_object__print)
    message (STATUS "TBAG_OBJECT_OBJECTS: ${TBAG_OBJECT_OBJECTS}")
    message (STATUS "TBAG_OBJECT_DEPENDENCIES: ${TBAG_OBJECT_DEPENDENCIES}")
    message (STATUS "TBAG_OBJECT_DEFINITIONS: ${TBAG_OBJECT_DEFINITIONS}")
    message (STATUS "TBAG_OBJECT_INCLUDE_DIRS: ${TBAG_OBJECT_INCLUDE_DIRS}")
    message (STATUS "TBAG_OBJECT_CXXFLAGS: ${TBAG_OBJECT_CXXFLAGS}")
    message (STATUS "TBAG_OBJECT_LDFLAGS: ${TBAG_OBJECT_LDFLAGS}")

    # constant variables.
    message (STATUS "TBAG_OBJECT_CONST_FILTER: ${TBAG_OBJECT_CONST_FILTER}")
    message (STATUS "TBAG_OBJECT_CONST_NAME: ${TBAG_OBJECT_CONST_NAME}")
    message (STATUS "TBAG_OBJECT_CONST_DIR: ${TBAG_OBJECT_CONST_DIR}")
    message (STATUS "TBAG_OBJECT_CONST_SOURCES: ${TBAG_OBJECT_CONST_SOURCES}")
endmacro ()

#/// Update constant variables.
#///
#/// @param __project_dir_name [in] Project directory name.
macro (tbag_object__update_const __config_file_path)
    tbag_find_object_config__get_filter_name (TBAG_OBJECT_CONST_FILTER "${__config_file_path}")
    get_filename_component (TBAG_OBJECT_CONST_NAME "${__config_file_path}" NAME)
    get_filename_component (TBAG_OBJECT_CONST_DIR  "${__config_file_path}" DIRECTORY)

    # List of filtered files.
    get_filename_component (__find_dir_absolute "${TBAG_OBJECT_CONST_DIR}" ABSOLUTE)
    file (GLOB_RECURSE TBAG_OBJECT_CONST_SOURCES "${__find_dir_absolute}/*${TBAG_OBJECT_CONST_FILTER}")
endmacro ()

#/// By-pass object files.
macro (tbag_object__by_pass)
    set (TBAG_OBJECT_OBJECTS ${TBAG_OBJECT_CONST_SOURCES})
endmacro ()

## --------------------
## Find Object filters.
## --------------------

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

## --------------
## Main function.
## --------------

set (TABG_DEFAULT_CXX_SUFFIX ".cpp")

#/// Find compile object.
#///
#/// @param __objs         [out] value name of result object files.
#/// @param __dependencies [out] value name of result dependencies.
#/// @param __definitions  [out] value name of result definitions.
#/// @param __include_dirs [out] value name of result include directories.
#/// @param __cxxflags     [out] value name of result cxx flags.
#/// @param __ldflags      [out] value name of result linker flags.
#/// @param __find_dir     [in]  find directory.
function (tbag_find_object __objs __dependencies __definitions __include_dirs __cxxflags __ldflags __find_dir)
    set (${__objs})
    set (${__dependencies})
    set (${__definitions})
    set (${__include_dirs})
    set (${__cxxflags})
    set (${__ldflags})

    tbag_find_object_config (__config_files "${__find_dir}")
    tbag_debug_list (tbag_project_build__update_objects ${__config_files})
    if ("${__config_files}" STREQUAL "")
        get_filename_component (__find_dir_absolute "${__find_dir}" ABSOLUTE)
        file (GLOB_RECURSE ${__objs} "${__find_dir_absolute}/*${TABG_DEFAULT_CXX_SUFFIX}")
    else ()
        foreach (__config_file_cursor ${__config_files})
            if (EXISTS "${__config_file_cursor}")
                tbag_object__clear ()
                tbag_object__update_const ("${__config_file_cursor}")

                # Call object config file.
                include ("${__config_file_cursor}")

                list (APPEND ${__objs}         ${TBAG_OBJECT_OBJECTS})
                list (APPEND ${__dependencies} ${TBAG_OBJECT_DEPENDENCIES})
                list (APPEND ${__definitions}  ${TBAG_OBJECT_DEFINITIONS})
                list (APPEND ${__include_dirs} ${TBAG_OBJECT_INCLUDE_DIRS})
                list (APPEND ${__cxxflags}     ${TBAG_OBJECT_CXXFLAGS})
                list (APPEND ${__ldflags}      ${TBAG_OBJECT_LDFLAGS})
            else ()
                message (WARNING "Not found ${__config_file_cursor}")
            endif ()
        endforeach()
    endif ()

    # update result.
    set (${__objs}         ${${__objs}}         PARENT_SCOPE)
    set (${__dependencies} ${${__dependencies}} PARENT_SCOPE)
    set (${__definitions}  ${${__definitions}}  PARENT_SCOPE)
    set (${__include_dirs} ${${__include_dirs}} PARENT_SCOPE)
    set (${__cxxflags}     ${${__cxxflags}}     PARENT_SCOPE)
    set (${__ldflags}      ${${__ldflags}}      PARENT_SCOPE)
endfunction ()

