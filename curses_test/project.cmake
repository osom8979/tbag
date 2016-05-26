## CMake subproject properties.

set (TBAG_PROJECT_DEFINITIONS  -DCURSES_TEST)
set (TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
set (TBAG_PROJECT_LDFLAGS      -L${CMAKE_CURRENT_LIST_DIR})

# Curses setting.
if (USE_ncurses)
    list  (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_NCURSES)
    #list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CURSES_INCLUDE_DIR})
    #list (APPEND TBAG_PROJECT_LDFLAGS      ${CURSES_LIBRARIES})
    list  (APPEND TBAG_PROJECT_INCLUDE_DIRS ${NCURSES_INCLUDE_DIRS})
    list  (APPEND TBAG_PROJECT_LDFLAGS      ${NCURSES_LIBRARIES})
endif ()

