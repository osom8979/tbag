#/// @file   TbagProject.cmake
#/// @brief  Tbag project function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagStrings)

set (__tbag_project_file_prefix  "project")
set (__tbag_project_file_suffix  "cmake")

set (__tbag_project_library_prefix     "lib")
set (__tbag_project_test_prefix        "test")
set (__tbag_project_executable_prefix  "")

set (__tbag_project_file_name    "${__tbag_project_file_prefix}.${__tbag_project_file_suffix}")
set (__tbag_project_file_regex   "[^/]+/${__tbag_project_file_prefix}\\.${__tbag_project_file_suffix}")

#! Find project list.
#
# \param _lib_projs  [out] value name of library project list.
# \param _test_projs [out] value name of test project list.
# \param _exe_projs  [out] value name of executable project list.
function (find_project _lib_projs _test_projs _exe_projs)

    set (__tbag_project_library_glob    "^${__tbag_project_library_prefix}${__tbag_project_file_regex}$")
    set (__tbag_project_test_glob       "^${__tbag_project_test_prefix}${__tbag_project_file_regex}$")
    set (__tbag_project_executable_glob "^${__tbag_project_executable_prefix}${__tbag_project_file_regex}$")

    # Find all project.
    file (GLOB_RECURSE _find_project_list RELATIVE "${PROJECT_SOURCE_DIR}" "${__tbag_project_file_name}")

    # Find library project.
    tbag_list_regex (_find_lib_proj "${__tbag_project_library_glob}" "${_find_project_list}")
    list (LENGTH _find_lib_proj _find_lib_proj_length)

    # Remove library project in the all project.
    if (${_find_lib_proj_length} GREATER 0)
        list (REMOVE_ITEM _find_project_list ${_find_lib_proj})
    endif ()

    # Find test executable project.
    tbag_list_regex (_find_test_proj "${__tbag_project_test_glob}" "${_find_project_list}")
    list (LENGTH _find_test_proj _find_test_proj_length)

    # Remove test project in the all project.
    if (${_find_test_proj_length} GREATER 0)
        list (REMOVE_ITEM _find_project_list ${_find_test_proj})
    endif ()

    # Find executable project.
    tbag_list_regex (_find_exe_proj "${__tbag_project_executable_glob}" "${_find_project_list}")

    string (REPLACE "/${__tbag_project_file_name}" "" ${_lib_projs}  "${_find_lib_proj}")
    string (REPLACE "/${__tbag_project_file_name}" "" ${_test_projs} "${_find_test_proj}")
    string (REPLACE "/${__tbag_project_file_name}" "" ${_exe_projs}  "${_find_exe_proj}")

    unset (__tbag_project_library_glob)
    unset (__tbag_project_test_glob)
    unset (__tbag_project_executable_glob)

    # result:
    set (${_lib_projs}  ${${_lib_projs}}  PARENT_SCOPE)
    set (${_test_projs} ${${_test_projs}} PARENT_SCOPE)
    set (${_exe_projs}  ${${_exe_projs}}  PARENT_SCOPE)
endfunction ()

set (BUILD_PROJECT_TYPE_LIB  "${__tbag_project_library_prefix}")
set (BUILD_PROJECT_TYPE_TEST "${__tbag_project_test_prefix}")
set (BUILD_PROJECT_TYPE_EXE  "${__tbag_project_executable_prefix}")

#! Check the project type.
#
# \param _value             [out] value name of result type string.
# \param _project_dir_name  [in]  project directory name.
function (get_project_type _value _project_dir_name)
    if (${_project_dir_name} MATCHES "^${__tbag_project_library_prefix}.+")
        set (${_value} "${BUILD_PROJECT_TYPE_LIB}" PARENT_SCOPE)
    elseif (${_project_dir_name} MATCHES "^${__tbag_project_test_prefix}.+")
        set (${_value} "${BUILD_PROJECT_TYPE_TEST}" PARENT_SCOPE)
    else ()
        set (${_value} "${BUILD_PROJECT_TYPE_EXE}" PARENT_SCOPE)
    endif ()
endfunction()

#! Check the project type.
#
# \param _value             [out] value name of result type string.
# \param _type              [in]  project type name.
# \param _project_dir_name  [in]  project directory name.
function (get_project_name _value _type _project_dir_name)
    if ("${_type}" STREQUAL "${BUILD_PROJECT_TYPE_LIB}")
        string (REPLACE "${BUILD_PROJECT_TYPE_LIB}" "" _temp_name "${_project_dir_name}")
        set (${_value} "${_temp_name}" PARENT_SCOPE)
    else ()
        set (${_value} "${_project_dir_name}" PARENT_SCOPE)
    endif ()
endfunction ()

#! Default build process.
#
# \param _libs  [in] List of library.
# \param _tests [in] List of test.
# \param _exes  [in] List of executable.
function (default_build _libs _tests _exes)
    # Build project.
    foreach (_cursor ${_libs} ${_tests} ${_exes})
        set (_project_is_verbose OFF)
        set (_project_default_install OFF)
        set (_project_objects)
        set (_project_dependencies)
        set (_project_definitions)
        set (_project_include_dirs)
        set (_project_cxxflags)
        set (_project_ldflags)

        set (_project_dir ${_cursor})
        get_project_type (_project_type "${_cursor}")
        get_project_name (_project_name "${_project_type}" "${_cursor}")
        project (${_project_name})

        # ----------------------------------
        # User defined sub-project settings.
        include ("${PROJECT_SOURCE_DIR}/${_project_dir}/${__tbag_project_file_name}")

        # -------------------------------------
        # Find & Create, compiled object files.
        find_compile_object (_find_objs
                _find_definitions
                _find_include_dirs
                _find_cxxflags
                _find_ldflags
                "${PROJECT_SOURCE_DIR}/${_project_dir}")

        # Result assign.
        list (APPEND _project_objects      ${_find_objs})
        list (APPEND _project_definitions  ${_find_definitions})
        list (APPEND _project_include_dirs ${_find_include_dirs})
        list (APPEND _project_cxxflags     ${_find_cxxflags})
        list (APPEND _project_ldflags      ${_find_ldflags})

        # -------------------------------
        # Register library or executable.
        list (LENGTH _project_objects _project_objects_length)
        if (${_project_objects_length} GREATER 0)
            if ("${_project_type}" STREQUAL "${BUILD_PROJECT_TYPE_LIB}")
                add_library (${_project_name} ${_project_objects})
            else ()
                add_executable (${_project_name} ${_project_objects})
            endif ()
        else ()
            message (FATAL_ERROR "Not found ${_project_name} object files.")
        endif ()

        if ("${_project_type}" STREQUAL "${BUILD_PROJECT_TYPE_LIB}")
            # Library project settings.
            set_target_properties (${_project_name} PROPERTIES
                    VERSION   "${VERSION}"
                    SOVERSION "${SOVERSION}")

        elseif ("${_project_type}" STREQUAL "${BUILD_PROJECT_TYPE_EXE}")
            # Executable project settings.
        elseif ("${_project_type}" STREQUAL "${BUILD_PROJECT_TYPE_TEST}")
            # Test project settings.
            find_package (GTest)
            if (GTEST_FOUND)
                list (APPEND _project_ldflags "${GTEST_BOTH_LIBRARIES}" "-lpthread")
            endif ()
        endif ()

        # Dependencies setting.
        list (LENGTH _project_dependencies _project_dependencies_length)
        if (${_project_dependencies_length} GREATER 0)
            add_dependencies (${_project_name} ${_project_dependencies})
        endif ()

        # Define setting.
        list (LENGTH _project_definitions _project_definitions_length)
        if (${_project_definitions_length} GREATER 0)
            target_compile_definitions (${_project_name} PRIVATE ${_project_definitions})
        endif ()

        # Include setting.
        list (LENGTH _project_include_dirs _project_include_dirs_length)
        if (${_project_include_dirs_length} GREATER 0)
            target_include_directories (${_project_name} PRIVATE ${_project_include_dirs})
        endif ()

        # Compile options.
        list (LENGTH _project_cxxflags _project_cxxflags_length)
        if (${_project_cxxflags_length} GREATER 0)
            target_compile_options (${_project_name} PRIVATE ${_project_cxxflags})
        endif ()

        # Linker options.
        list (APPEND _project_ldflags ${_project_dependencies})
        list (LENGTH _project_ldflags _project_ldflags_length)
        if (${_project_ldflags_length} GREATER 0)
            target_link_libraries (${_project_name} PRIVATE ${_project_ldflags})
        endif ()

        # ----------------
        # Insatll setting.
        if (_project_default_install)
            install (TARGETS ${_project_name}
                    RUNTIME DESTINATION bin
                    LIBRARY DESTINATION lib
                    ARCHIVE DESTINATION lib)
            install (DIRECTORY ${_project_dir}
                    DESTINATION "include"
                    FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
        endif ()
    endforeach ()
endfunction ()

