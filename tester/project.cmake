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

set (_project_definitions  -D__LIBTBAG__)
set (_project_include_dirs ${CMAKE_CURRENT_LIST_DIR})
set (_project_ldflags      -L${CMAKE_CURRENT_LIST_DIR})

# libuv setting.
if (USE_libuv)
    list (APPEND _project_include_dirs ${UV_INCLUDE_DIRS})
    list (APPEND _project_ldflags      ${UV_LIBRARIES})
endif ()

# TinyXML2 setting.
if (USE_tinyxml2)
    list (APPEND _project_include_dirs ${TINYXML2_INCLUDE_DIRS})
    list (APPEND _project_ldflags      ${TINYXML2_LIBRARIES})
endif ()

# Curses setting.
if (USE_ncurses)
    list (APPEND _project_include_dirs ${CURSES_INCLUDE_DIR})
    list (APPEND _project_ldflags      ${CURSES_LIBRARIES})
endif ()

