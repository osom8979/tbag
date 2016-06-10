#/// @file   TbagProjectNCurses.cmake
#/// @brief  NCurses project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT NCurses_FOUND)
    message (WARNING "Not found NCurses.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${NCurses_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${NCurses_LIBRARIES})

