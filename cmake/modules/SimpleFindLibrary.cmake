## Find library module.

#! Find headers function.
#
# \param _header_prefix  [out] Variable result prefix.
# \param _search_paths   [in]  Search directories.
# \param _headers        [in]  Find header files.
function (find_headers _header_prefix _search_paths _headers)
    set (${_header_prefix})
    set (_index 0)

    foreach (_cursor ${_headers})
        set (_temp_name ${_header_prefix}_${_index})
        find_path (${_temp_name}
            NAMES ${_cursor}
            PATHS ${_search_paths})
        math (EXPR _index "${_index} + 1")

        if (IS_DIRECTORY "${${_temp_name}}")
            list (APPEND ${_header_prefix} "${${_temp_name}}")
        endif ()
    endforeach ()

    set (${_header_prefix} ${${_header_prefix}} PARENT_SCOPE)
endfunction ()

#! Find libraries function.
#
# \param _lib_prefix    [out] Variable result prefix.
# \param _search_paths  [in]  Search directories.
# \param _libs          [in]  Find library files.
function (find_libraries _lib_prefix _search_paths _libs)
    set (${_lib_prefix})
    set (_index 0)

    foreach (_cursor ${_libs})
        set (_temp_name ${_lib_prefix}_${_index})
        find_library (${_temp_name}
            NAMES "${_cursor}"
            PATHS ${_search_paths}
            PATH_SUFFIXES Debug Release)
        math (EXPR _index "${_index} + 1")

        if (EXISTS "${${_temp_name}}")
            list (APPEND ${_lib_prefix} "${${_temp_name}}")
        endif ()
    endforeach ()

    set (${_lib_prefix} ${${_lib_prefix}} PARENT_SCOPE)
endfunction ()

#! Find {Library} function.
#
# The following variables are optionally searched for defaults.
#  ${_prefix}_ROOT
#  ${_prefix}_ROOT_INCLUDE_PATHS
#  ${_prefix}_ROOT_LIBRARY_PATHS
#
# The following are set after configuration is done:
#  ${_prefix}_FOUND
#  ${_prefix}_INCLUDE_DIRS
#  ${_prefix}_LIBRARIES
#
# \param _prefix  [in] Variable result prefix.
# \param _headers [in] Find header files.
# \param _libs    [in] Find library files.
function (simple_find_library _prefix  _headers _libs)

    set (_root_dir           "${_prefix}_ROOT")
    set (_root_include_paths "${_prefix}_ROOT_INCLUDE_PATHS")
    set (_root_library_paths "${_prefix}_ROOT_LIBRARY_PATHS")

    # Check 64bit library directories.
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set (_lib64_paths "/usr/lib64"
                          "/usr/local/lib64"
                          "${${_root_dir}}/lib64"
                          "$ENV{${_root_dir}}/lib64")
    else ()
        set (_lib64_paths)
    endif ()

    set (_search_include_paths "/usr/include"
                               "/usr/local/include"
                               "${${_root_dir}}"
                               "${${_root_dir}}/include"
                               "$ENV{${_root_dir}}"
                               "$ENV{${_root_dir}}/include"
                               ${${_root_include_paths}})
    set (_search_library_paths "/usr/lib"
                               "/usr/local/lib"
                               "${${_root_dir}}"
                               "${${_root_dir}}/lib"
                               "$ENV{${_root_dir}}"
                               "$ENV{${_root_dir}}/lib"
                               ${${_root_library_paths}}
                               ${_lib64_paths})

    find_headers (${_prefix}_INCLUDE_DIRS "${_search_include_paths}" "${_headers}")
    find_libraries (${_prefix}_LIBRARIES "${_search_library_paths}" "${_libs}")

    set (LOOKED_FOR
        ${_prefix}_LIBRARIES
        ${_prefix}_INCLUDE_DIRS)

    include (FindPackageHandleStandardArgs)
    find_package_handle_standard_args (${_prefix} DEFAULT_MSG ${LOOKED_FOR})

    if (${_prefix}_FOUND)
        mark_as_advanced (${LOOKED_FOR})
        # message ("** ${_prefix}_INCLUDE_DIRS: ${${_prefix}_INCLUDE_DIRS}")
        # message ("** ${_prefix}_LIBRARIES: ${${_prefix}_LIBRARIES}")
    endif ()

    set (${_prefix}_FOUND        ${${_prefix}_FOUND}        PARENT_SCOPE)
    set (${_prefix}_INCLUDE_DIRS ${${_prefix}_INCLUDE_DIRS} PARENT_SCOPE)
    set (${_prefix}_LIBRARIES    ${${_prefix}_LIBRARIES}    PARENT_SCOPE)
endfunction ()

