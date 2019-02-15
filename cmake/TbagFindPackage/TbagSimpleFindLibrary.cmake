#/// @file   TbagSimpleFindLibrary.cmake
#/// @brief  tbag_simple_find_library function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagUtils)

#/// Find headers function.
#///
#/// @param __result     [out] Variable result prefix.
#/// @param __paths      [in]  Search directories.
#/// @param __headers    [in]  Find header files.
#/// @param __no_default [in]  Enable NO_DEFAULT_PATH flag.
function (tbag_find_headers __result __paths __headers __no_default)
    set (${__result})
    set (__name_index 0)

    foreach (__header_cursor ${__headers})
        set (__find_name "${__result}_${__name_index}")
        if (${__no_default})
            find_path (${__find_name}
                    NAMES ${__header_cursor}
                    PATHS ${__paths}
                    NO_DEFAULT_PATH)
        else ()
            find_path (${__find_name}
                    NAMES ${__header_cursor}
                    PATHS ${__paths})
        endif ()
        math (EXPR __name_index "${__name_index} + 1")

        if (IS_DIRECTORY "${${__find_name}}")
            list (APPEND ${__result} "${${__find_name}}")
        endif ()
    endforeach ()

    # Preview
    tbag_utils__list_to_string (__preview_result "," ${${__result}})
    tbag_debug (tbag_find_headers "Result variable name: ${__result} (${__preview_result})")
    tbag_utils__list_to_string (__preview_result "," ${__paths})
    tbag_debug (tbag_find_headers "Search directories: ${__preview_result}")
    tbag_utils__list_to_string (__preview_result "," ${__headers})
    tbag_debug (tbag_find_headers "Find header files: ${__preview_result}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Find libraries function.
#///
#/// @param __result     [out] Variable result prefix.
#/// @param __paths      [in]  Search directories.
#/// @param __libs       [in]  Find library files.
#/// @param __no_default [in]  Enable NO_DEFAULT_PATH flag.
function (tbag_find_libraries __result __paths __libs __no_default)
    set (${__result})
    set (__name_index 0)

    foreach (__lib_cursor ${__libs})
        set (__find_name ${__result}_${__name_index})
        if (${__no_default})
            find_library (${__find_name}
                    NAMES "${__lib_cursor}"
                    PATHS ${__paths}
                    PATH_SUFFIXES Debug Release
                    NO_DEFAULT_PATH)
        else ()
            find_library (${__find_name}
                    NAMES "${__lib_cursor}"
                    PATHS ${__paths}
                    PATH_SUFFIXES Debug Release)
        endif ()
        math (EXPR __name_index "${__name_index} + 1")

        if (EXISTS "${${__find_name}}")
            list (APPEND ${__result} "${${__find_name}}")
        endif ()
    endforeach ()

    # Preview
    tbag_utils__list_to_string (__preview_result "," ${${__result}})
    tbag_debug (tbag_find_libraries "Result variable name: ${__result} (${__preview_result})")
    tbag_utils__list_to_string (__preview_result "," ${__paths})
    tbag_debug (tbag_find_libraries "Search directories: ${__preview_result}")
    tbag_utils__list_to_string (__preview_result "," ${__libs})
    tbag_debug (tbag_find_libraries "Find library files: ${__preview_result}")

    # update result.
    set (${__result} ${${__result}} PARENT_SCOPE)
endfunction ()

#/// Find {Library} function.
#///
#/// @param __prefix  [in] Variable result prefix.
#/// @param __headers [in] Find header files.
#/// @param __libs    [in] Find library files.
#///
#/// @remarks
#///  The following variables are optionally searched for defaults.
#///   ${__prefix}_ROOT
#///   ${__prefix}_ROOT_INCLUDE_PATHS
#///   ${__prefix}_ROOT_LIBRARY_PATHS
#///   ${__prefix}_ROOT_INCLUDE_PATHS_ONLY:BOOL
#///   ${__prefix}_ROOT_LIBRARY_PATHS_ONLY:BOOL
#///
#///  The following are set after configuration is done:
#///   ${__prefix}_FOUND
#///   ${__prefix}_INCLUDE_DIRS
#///   ${__prefix}_LIBRARIES
#///
function (tbag_simple_find_library __prefix  __headers __libs)
    set (__root_dir                "${__prefix}_ROOT")
    set (__root_include_paths      "${__prefix}_ROOT_INCLUDE_PATHS")
    set (__root_library_paths      "${__prefix}_ROOT_LIBRARY_PATHS")

    if (${${__prefix}_ROOT_INCLUDE_PATHS_ONLY})
        set (__root_include_paths_only ON)
    else ()
        set (__root_include_paths_only OFF)
    endif ()
    if (${${__prefix}_ROOT_LIBRARY_PATHS_ONLY})
        set (__root_library_paths_only ON)
    else ()
        set (__root_library_paths_only OFF)
    endif ()

    # Check 64bit library directories:
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set (__lib64_paths "/usr/lib64" "/usr/local/lib64")
    else ()
        set (__lib64_paths)
    endif ()

    set (__search_include_paths  ${${__root_include_paths}}
                                 "${${__root_dir}}/include"
                                 "$ENV{${__root_dir}}/include"
                                 "/usr/local/include"
                                 "/usr/include")

    set (__search_library_paths  ${${__root_library_paths}}
                                 "${${__root_dir}}/lib"
                                 "$ENV{${__root_dir}}/lib"
                                 "/usr/local/lib"
                                 "/usr/lib"
                                 ${__lib64_paths})

    if (__root_include_paths_only)
        set (__search_include_paths ${${__root_include_paths}})
    endif ()
    if (__root_library_paths_only)
        set (__search_library_paths ${${__root_library_paths}})
    endif ()

    tbag_find_headers   (${__prefix}_INCLUDE_DIRS "${__search_include_paths}" "${__headers}" ${__root_include_paths_only})
    tbag_find_libraries (${__prefix}_LIBRARIES    "${__search_library_paths}" "${__libs}"    ${__root_library_paths_only})

    set (__looked_for ${__prefix}_INCLUDE_DIRS)
    if (NOT "${__libs}" STREQUAL "")
        list (INSERT __looked_for 0 ${__prefix}_LIBRARIES)
    endif ()

    include (FindPackageHandleStandardArgs)
    find_package_handle_standard_args (${__prefix} DEFAULT_MSG ${__looked_for})

    # If the variables <var1> to <varN> are all valid, then <UPPERCASED_NAME>_FOUND will be set to TRUE.
    string (TOUPPER "${__prefix}" __prefix_upper)
    set (${__prefix}_FOUND "${${__prefix_upper}_FOUND}")

    if (${__prefix}_FOUND)
        mark_as_advanced (${__looked_for})
    endif ()

    tbag_debug (tbag_simple_find_library ${__prefix}_FOUND        ":" ${${__prefix}_FOUND})
    tbag_debug (tbag_simple_find_library ${__prefix}_INCLUDE_DIRS ":" ${${__prefix}_INCLUDE_DIRS})
    tbag_debug (tbag_simple_find_library ${__prefix}_LIBRARIES    ":" ${${__prefix}_LIBRARIES})

    # update result.
    set (${__prefix}_FOUND        ${${__prefix}_FOUND}        PARENT_SCOPE)
    set (${__prefix}_INCLUDE_DIRS ${${__prefix}_INCLUDE_DIRS} PARENT_SCOPE)
    set (${__prefix}_LIBRARIES    ${${__prefix}_LIBRARIES}    PARENT_SCOPE)
endfunction ()

#/// Find {Library} function with module prefix.
#///
#/// @param __prefix  [in] Variable result prefix.
#/// @param __headers [in] Find header files.
macro (tbag_simple_find_library_with_module_prefix __headers __libs)
    tbag_utils__current_module_name (__current_module_name "${CMAKE_CURRENT_LIST_FILE}" "Find")
    tbag_simple_find_library ("${__current_module_name}" "${__headers}" "${__libs}")
endmacro ()

