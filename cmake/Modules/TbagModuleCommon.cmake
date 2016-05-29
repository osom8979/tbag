#/// @file   TbagModuleCommon.cmake
#/// @brief  Common tbag module.
#/// @author zer0
#/// @date   2016-05-29

#/// Clear project properties.
macro (tbag_project_build__clear)
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
macro (tbag_project_build__update_const __project_dir_name)
    set (TBAG_PROJECT_CONST_DIR_NAME "${__project_dir_name}")
    tbag_project_common__get_type (TBAG_PROJECT_CONST_TYPE "${__project_dir_name}")
    tbag_project_common__get_name (TBAG_PROJECT_CONST_NAME "${__project_dir_name}")
endmacro ()

#/// Dependencies setting.
macro (tbag_project_build__update_dependencies)
    list (LENGTH TBAG_PROJECT_DEPENDENCIES __tbag_project_dependencies_length)
    if (${__tbag_project_dependencies_length} GREATER 0)
        add_dependencies (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_DEPENDENCIES})
    endif ()
endmacro ()

#/// Define setting.
macro (tbag_project_build__update_definitions)
    list (LENGTH TBAG_PROJECT_DEFINITIONS __project_definitions_length)
    if (${__project_definitions_length} GREATER 0)
        target_compile_definitions (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_DEFINITIONS})
    endif ()
endmacro ()

#/// Include directories settings.
macro (tbag_project_build__update_include)
    list (LENGTH TBAG_PROJECT_INCLUDE_DIRS __project_include_dirs_length)
    if (${__project_include_dirs_length} GREATER 0)
        target_include_directories (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_INCLUDE_DIRS})
    endif ()
endmacro ()

#/// C++ compiler flags.
macro (tbag_project_build__update_cxx_flags)
    list (LENGTH TBAG_PROJECT_CXXFLAGS __project_cxxflags_length)
    if (${__project_cxxflags_length} GREATER 0)
        target_compile_options (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_CXXFLAGS})
    endif ()
endmacro ()

#/// Linker flags.
macro (tbag_project_build__update_linker_flags)
    list (APPEND TBAG_PROJECT_LDFLAGS ${TBAG_PROJECT_DEPENDENCIES})
    list (LENGTH TBAG_PROJECT_LDFLAGS __project_ldflags_length)
    if (${__project_ldflags_length} GREATER 0)
        target_link_libraries (${TBAG_PROJECT_CONST_NAME} PRIVATE ${TBAG_PROJECT_LDFLAGS})
    endif ()
endmacro ()

#/// Update all of target.
macro (tbag_project_build__update_all)
    tbag_project_build__update_dependencies ()
    tbag_project_build__update_definitions  ()
    tbag_project_build__update_include      ()
    tbag_project_build__update_cxx_flags    ()
    tbag_project_build__update_linker_flags ()
endmacro ()

#/// Register library target.
macro (tbag_project_build__register_object_of_library)
    list (LENGTH TBAG_PROJECT_OBJECTS __tbag_project_objects_length)
    if (${__tbag_project_objects_length} GREATER 0)
        add_library (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_OBJECTS})
    else ()
        message (FATAL_ERROR "Not found ${TBAG_PROJECT_CONST_NAME} object files.")
    endif ()
endmacro ()

#/// Register executable target.
macro (tbag_project_build__register_object_of_executable)
    list (LENGTH TBAG_PROJECT_OBJECTS __tbag_project_objects_length)
    if (${__tbag_project_objects_length} GREATER 0)
        add_executable (${TBAG_PROJECT_CONST_NAME} ${TBAG_PROJECT_OBJECTS})
    else ()
        message (FATAL_ERROR "Not found ${TBAG_PROJECT_CONST_NAME} object files.")
    endif ()
endmacro ()

