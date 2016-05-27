#/// @file   TbagModuleNCurses.cmake
#/// @brief  NCurses tbag cmake module.
#/// @author zer0
#/// @date   2016-05-27

#/// Assign NCurses libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module__assign_ncurses)
    find_package (NCurses)
    if (NCURSES_FOUND)
        list  (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_NCURSES)
        list  (APPEND TBAG_PROJECT_INCLUDE_DIRS ${NCURSES_INCLUDE_DIRS})
        list  (APPEND TBAG_PROJECT_LDFLAGS      ${NCURSES_LIBRARIES})
    endif ()
endmacro ()

