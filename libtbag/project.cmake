## CMake subproject properties.

if (_project_is_verbose)
    message ("++ _project_dir: ${_project_dir}")
    message ("++ _project_type: ${_project_type}")
    message ("++ _project_name: ${_project_name}")
    message ("++ _project_objects: ${_project_objects}")
    message ("++ _project_libraries: ${_project_libraries}")

    # Empty variables:
    message ("++ _project_dependencies: ${_project_dependencies}")
    message ("++ _project_definitions: ${_project_definitions}")
    message ("++ _project_include_dirs: ${_project_include_dirs}")
    message ("++ _project_cxxflags: ${_project_cxxflags}")
    message ("++ _project_ldflags: ${_project_ldflags}")
endif ()

set (_project_definitions  -D__LIBTBAG__)
set (_project_include_dirs ${CMAKE_CURRENT_LIST_DIR})
set (_project_ldflags      -L${CMAKE_CURRENT_LIST_DIR})
set (_project_cxxflags     -fPIC)

# libuv setting.
set (_project_include_dirs ${UV_INCLUDE_DIRS})
set (_project_ldflags      ${UV_LIBRARIES})

