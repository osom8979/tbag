#/// @file   TbagProjectNCurses.cmake
#/// @brief  NCurses project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT NCurses_FOUND)
    find_package (NCurses)
endif ()

if (NCurses_FOUND)
    list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_NCURSES)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${NCurses_INCLUDE_DIRS})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${NCurses_LIBRARIES})
endif ()

