#/// @file   TbagObjects.cmake
#/// @brief  List of tbag object.
#/// @author zer0
#/// @date   2016-05-29

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

