## CMake subproject properties.

if (_project_is_verbose)
    message ("++ _project_default_install: ${_project_default_install}")
    message ("++ _project_dir: ${_project_dir}")
    message ("++ _project_type: ${_project_type}")
    message ("++ _project_name: ${_project_name}")

    # Empty variables:
    message ("++ _project_objects: ${_project_objects}")
    message ("++ _project_dependencies: ${_project_dependencies}")
    message ("++ _project_definitions: ${_project_definitions}")
    message ("++ _project_include_dirs: ${_project_include_dirs}")
    message ("++ _project_cxxflags: ${_project_cxxflags}")
    message ("++ _project_ldflags: ${_project_ldflags}")
endif ()

set (_project_definitions  -DCURSES_TEST)
set (_project_include_dirs ${CMAKE_CURRENT_LIST_DIR})
set (_project_ldflags      -L${CMAKE_CURRENT_LIST_DIR})

# Curses setting.
if (USE_ncurses)
    list (APPEND _project_definitions  -DUSE_NCURSES)
    list (APPEND _project_include_dirs ${CURSES_INCLUDE_DIR})
    list (APPEND _project_ldflags      ${CURSES_LIBRARIES})
endif ()

