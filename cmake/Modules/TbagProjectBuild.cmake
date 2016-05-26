#/// @file   TbagProjectBuild.cmake
#/// @brief  Tbag project build function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagProjectCommon)

macro (tbag_project_clear_properties)
    set (TBAG_PROJECT_OBJECTS)
    set (TBAG_PROJECT_DEPENDENCIES)
    set (TBAG_PROJECT_DEFINITIONS)
    set (TBAG_PROJECT_INCLUDE_DIRS)
    set (TBAG_PROJECT_CXXFLAGS)
    set (TBAG_PROJECT_LDFLAGS)

    set (TBAG_PROJECT_CONST_DIR_NAME)
    set (TBAG_PROJECT_CONST_DIR_PATH)
    set (TBAG_PROJECT_CONST_CMAKE)
    set (TBAG_PROJECT_CONST_TYPE)
    set (TBAG_PROJECT_CONST_NAME)
endmacro ()

macro (tbag_project_update_const __project_dir_name)
    set (TBAG_PROJECT_CONST_DIR_NAME "${__project_dir_name}")
    tbag_get_project_type (TBAG_PROJECT_CONST_TYPE "${__project_dir_name}")
    tbag_get_project_name (TBAG_PROJECT_CONST_NAME "${__project_dir_name}")
endmacro ()

#/// Assign soversion property.
#///
#/// @remarsk
#///  Recommended apply to the library project.
macro (tbag_project_assign_soversion)
    set_target_properties (${TBAG_PROJECT_CONST_NAME} PROPERTIES
            VERSION   "${VERSION}"
            SOVERSION "${SOVERSION}")
endmacro ()

#/// Assign google-test libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_project_assign_gtest)
    find_package (GTest)
    if (GTEST_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_GTEST)
        list (APPEND TBAG_PROJECT_LDFLAGS      ${GTEST_BOTH_LIBRARIES} -lpthread)
    endif ()
endmacro ()

#/// Assign default install property.
#///
#/// @remarsk
#///  Recommended apply to the library & executable project.
macro (tbag_project_assign_default_install)
    # Target install.
    install (TARGETS "${TBAG_PROJECT_CONST_NAME}"
             RUNTIME DESTINATION bin
             LIBRARY DESTINATION lib
             ARCHIVE DESTINATION lib)

    # Header install.
    install (DIRECTORY "${TBAG_PROJECT_CONST_DIR_NAME}"
             DESTINATION "include"
             FILES_MATCHING REGEX ".*\\.[Hh]([Pp][Pp]|[Xx][Xx])?")
endmacro ()

macro (tbag_project_update_target)
    # Dependencies setting.
    list (LENGTH TBAG_PROJECT_DEPENDENCIES __tbag_project_dependencies_length)
    if (${__tbag_project_dependencies_length} GREATER 0)
        add_dependencies (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_DEPENDENCIES})
    endif ()

    # Define setting.
    list (LENGTH TBAG_PROJECT_DEFINITIONS __project_definitions_length)
    if (${__project_definitions_length} GREATER 0)
        target_compile_definitions (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_DEFINITIONS})
    endif ()

    # Include setting.
    list (LENGTH TBAG_PROJECT_INCLUDE_DIRS __project_include_dirs_length)
    if (${__project_include_dirs_length} GREATER 0)
        target_include_directories (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_INCLUDE_DIRS})
    endif ()

    # Compile options.
    list (LENGTH TBAG_PROJECT_CXXFLAGS __project_cxxflags_length)
    if (${__project_cxxflags_length} GREATER 0)
        target_compile_options (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_CXXFLAGS})
    endif ()

    # Linker options.
    list (APPEND TBAG_PROJECT_LDFLAGS ${TBAG_PROJECT_DEPENDENCIES})
    list (LENGTH TBAG_PROJECT_LDFLAGS __project_ldflags_length)
    if (${__project_ldflags_length} GREATER 0)
        target_link_libraries (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_LDFLAGS})
    endif ()
endmacro ()

macro (tbag_project_update_objects __find_directory)
    find_compile_object (__find_compile_objs
                         __find_compile_definitions
                         __find_compile_include_dirs
                         __find_compile_cxxflags
                         __find_compile_ldflags
                         "${__find_directory}")

    # Result assign.
    list (APPEND TBAG_PROJECT_OBJECTS      ${__find_compile_objs})
    list (APPEND TBAG_PROJECT_DEFINITIONS  ${__find_compile_definitions})
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${__find_compile_include_dirs})
    list (APPEND TBAG_PROJECT_CXXFLAGS     ${__find_compile_cxxflags})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${__find_compile_ldflags})

    unset (__find_compile_objs)
    unset (__find_compile_definitions)
    unset (__find_compile_include_dirs)
    unset (__find_compile_cxxflags)
    unset (__find_compile_ldflags)
endmacro ()

macro (tbag_project_register_object_of_executable)
    list (LENGTH TBAG_PROJECT_OBJECTS __tbag_project_objects_length)
    if (${__tbag_project_objects_length} GREATER 0)
        add_executable (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_OBJECTS})
    else ()
        message (FATAL_ERROR "Not found ${TBAG_PROJECT_CONST_NAME} object files.")
    endif ()
    unset (__tbag_project_objects_length)
endmacro ()

macro (tbag_project_register_object_of_library)
    # -------------------------------
    # Register library or executable.
    list (LENGTH TBAG_PROJECT_OBJECTS __tbag_project_objects_length)
    if (${__tbag_project_objects_length} GREATER 0)
        add_library (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_OBJECTS})
    else ()
        message (FATAL_ERROR "Not found ${TBAG_PROJECT_CONST_NAME} object files.")
    endif ()
    unset (__tbag_project_objects_length)
endmacro ()

#! Default build process.
#
# \param __libs  [in] List of library.
# \param __exes  [in] List of executable.
function (default_build __libs __exes __root_dir)
    # Loop of subproject builder.
    foreach (__cursor ${__libs})
        tbag_project_clear_properties ()
        tbag_project_update_const ("${__cursor}")

        project (${TBAG_PROJECT_CONST_NAME})
        include ("${__root_dir}/${TBAG_PROJECT_CONST_DIR_NAME}/${TBAG_PROJECT_FILE_NAME}")

        tbag_project_update_objects ("${__root_dir}/${TBAG_PROJECT_CONST_DIR_NAME}")
        tbag_project_register_object_of_library ()
        tbag_project_update_target ()
    endforeach ()

    # Loop of subproject builder.
    foreach (__cursor ${__exes})
        tbag_project_clear_properties ()
        tbag_project_update_const ("${__cursor}")

        project (${TBAG_PROJECT_CONST_NAME})
        include ("${__root_dir}/${TBAG_PROJECT_CONST_DIR_NAME}/${TBAG_PROJECT_FILE_NAME}")

        tbag_project_update_objects ("${__root_dir}/${TBAG_PROJECT_CONST_DIR_NAME}")
        tbag_project_register_object_of_executable ()
        tbag_project_update_target ()
    endforeach ()

endfunction ()

