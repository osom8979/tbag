#/// @file   TbagProjectBuild.cmake
#/// @brief  Tbag project build function prototype.
#/// @author zer0
#/// @date   2016-05-26

include (TbagProjectCommon)
include (TbagFindObject)
include (TbagModules)

#/// Clear project properties.
macro (tbag_project_clear_properties)
    set (TBAG_PROJECT_OBJECTS)
    set (TBAG_PROJECT_DEPENDENCIES)
    set (TBAG_PROJECT_DEFINITIONS)
    set (TBAG_PROJECT_INCLUDE_DIRS)
    set (TBAG_PROJECT_CXXFLAGS)
    set (TBAG_PROJECT_LDFLAGS)

    # constant variables.
    set (TBAG_PROJECT_CONST_DIR_NAME)
    set (TBAG_PROJECT_CONST_TYPE)
    set (TBAG_PROJECT_CONST_NAME)
endmacro ()

#/// Clear project properties.
#///
#/// @param __project_dir_name [in] Project directory name.
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

#/// Dependencies setting.
macro (tbag_project_update_dependencies)
    list (LENGTH TBAG_PROJECT_DEPENDENCIES __tbag_project_dependencies_length)
    if (${__tbag_project_dependencies_length} GREATER 0)
        add_dependencies (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_DEPENDENCIES})
    endif ()
    unset (__tbag_project_dependencies_length)
endmacro ()

#/// Define setting.
macro (tbag_project_update_definitions)
    list (LENGTH TBAG_PROJECT_DEFINITIONS __project_definitions_length)
    if (${__project_definitions_length} GREATER 0)
        target_compile_definitions (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_DEFINITIONS})
    endif ()
    unset (__project_definitions_length)
endmacro ()

#/// Include directories settings.
macro (tbag_project_update_include)
    list (LENGTH TBAG_PROJECT_INCLUDE_DIRS __project_include_dirs_length)
    if (${__project_include_dirs_length} GREATER 0)
        target_include_directories (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_INCLUDE_DIRS})
    endif ()
    unset (__project_include_dirs_length)
endmacro ()

#/// C++ compiler flags.
macro (tbag_project_update_cxx_flags)
    list (LENGTH TBAG_PROJECT_CXXFLAGS __project_cxxflags_length)
    if (${__project_cxxflags_length} GREATER 0)
        target_compile_options (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_CXXFLAGS})
    endif ()
    unset (__project_cxxflags_length)
endmacro ()

#/// Linker flags.
macro (tbag_project_update_linker_flags)
    list (APPEND TBAG_PROJECT_LDFLAGS ${TBAG_PROJECT_DEPENDENCIES})
    list (LENGTH TBAG_PROJECT_LDFLAGS __project_ldflags_length)
    if (${__project_ldflags_length} GREATER 0)
        target_link_libraries (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_LDFLAGS})
    endif ()
    unset (__project_ldflags_length)
endmacro ()

#/// Update all of target.
macro (tbag_project_update_all)
    tbag_project_update_dependencies ()
    tbag_project_update_definitions  ()
    tbag_project_update_include      ()
    tbag_project_update_cxx_flags    ()
    tbag_project_update_linker_flags ()
endmacro ()

#/// Register executable target.
macro (tbag_project_register_object_of_executable)
    list (LENGTH TBAG_PROJECT_OBJECTS __tbag_project_objects_length)
    if (${__tbag_project_objects_length} GREATER 0)
        add_executable (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_OBJECTS})
    else ()
        message (FATAL_ERROR "Not found ${TBAG_PROJECT_CONST_NAME} object files.")
    endif ()
    unset (__tbag_project_objects_length)
endmacro ()

#/// Register library target.
macro (tbag_project_register_object_of_library)
    list (LENGTH TBAG_PROJECT_OBJECTS __tbag_project_objects_length)
    if (${__tbag_project_objects_length} GREATER 0)
        add_library (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_OBJECTS})
    else ()
        message (FATAL_ERROR "Not found ${TBAG_PROJECT_CONST_NAME} object files.")
    endif ()
    unset (__tbag_project_objects_length)
endmacro ()

#/// Find & register object files.
#///
#/// @param __find_directory [in] find directory path.
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

#/// Default build process.
#///
#/// @param __is_library        [in] YES is library project, NO is executable project.
#/// @param __project_dir_name  [in] Project directory name.
#/// @param __root_dir          [in] Find root directory (Source code directory).
macro (tbag_project_default_build __is_library __project_dir_name __root_dir)
    set (__project_dir_path    "${__root_dir}/${__project_dir_name}")
    set (__project_cmake_path  "${__project_dir_path}/${TBAG_PROJECT_FILE_NAME}")

    tbag_project_clear_properties ()
    tbag_project_update_const ("${__cursor}")

    project (${TBAG_PROJECT_CONST_NAME})
    include ("${__project_cmake_path}")

    tbag_project_update_objects ("${__project_dir_path}")
    if (${__is_library})
        tbag_project_register_object_of_library ()
    else ()
        tbag_project_register_object_of_executable ()
    endif ()
    tbag_project_update_all ()

    unset (__project_dir_path)
    unset (__project_cmake_path)
endmacro ()

#/// Default build process.
#///
#/// @param __libs     [in] List of library.
#/// @param __exes     [in] List of executable.
#/// @param __root_dir [in] Find root directory (Source code directory).
function (tbag_project_default_build_all __libs __exes __root_dir)
    # Loop of library project.
    foreach (__cursor ${__libs})
        tbag_project_default_build (YES "${__cursor}" "${__root_dir}")
    endforeach ()

    # Loop of executable project.
    foreach (__cursor ${__exes})
        tbag_project_default_build (NO "${__cursor}" "${__root_dir}")
    endforeach ()
endfunction ()

