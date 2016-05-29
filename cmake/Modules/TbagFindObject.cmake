#/// @file   TbagFindObject.cmake
#/// @brief  tbag_find_object function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagFindObjectConfig)
include (TbagObjects)
include (TbagModules)

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
    tbag_debug__list (tbag_project_build__update_objects ${__config_files})
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
