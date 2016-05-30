#/// @file   TbagSimpleFindLibrary.cmake
#/// @brief  tbag_simple_find_library function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagFindHeaders)
include (TbagFindLibraries)
include (TbagUtils)

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
#///
#///  The following are set after configuration is done:
#///   ${__prefix}_FOUND
#///   ${__prefix}_INCLUDE_DIRS
#///   ${__prefix}_LIBRARIES
#///
function (tbag_simple_find_library __prefix  __headers __libs)
    set (__root_dir           "${__prefix}_ROOT")
    set (__root_include_paths "${__prefix}_ROOT_INCLUDE_PATHS")
    set (__root_library_paths "${__prefix}_ROOT_LIBRARY_PATHS")

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

    tbag_find_headers   (${__prefix}_INCLUDE_DIRS "${__search_include_paths}" "${__headers}")
    tbag_find_libraries (${__prefix}_LIBRARIES    "${__search_library_paths}" "${__libs}")

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
    tbag_utils__current_module_name (__current_module_name ${CMAKE_CURRENT_LIST_FILE} "Find")
    tbag_simple_find_library ("${__current_module_name}" "${__headers}" "${__libs}")
    unset (__current_module_name)
endmacro ()

