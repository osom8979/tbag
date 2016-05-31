#/// @file   TbagwxWidgets.cmake
#/// @brief  tbag_wxwidgets__find_package function prototype.
#/// @author zer0
#/// @date   2016-05-31
#///
#/// @remarks
#///  The following variables are optionally searched for defaults
#///    wxWidgets_ROOT

include (TbagUtils)

#/// Setup & find FindwxWidgets.cmake file.
macro (tbag_wxwidgets__find_package)
    if (NOT DEFINED wxWidgets_ROOT)
        set (wxWidgets_ROOT "${THIRD_PREFIX}")
    endif ()

    if (NOT DEFINED wxWidgets_ROOT_DIR)
        set (wxWidgets_ROOT_DIR "${wxWidgets_ROOT}")
    endif ()

    if (WIN32 AND NOT CYGWIN AND NOT MSYS)
        # Force setting: wxWidgets_FIND_STYLE = unix
        set (CMAKE_CROSSCOMPILING YES)
    endif ()

    find_package (wxWidgets)

    if (wxWidgets_FOUND)
        # Change: MSYS PATH -> WINDOWS PATH.
        if (WIN32 AND NOT CYGWIN AND NOT MSYS)
            tbag_utils__convert_msys_to_windows_path (__convert_path "${wxWidgets_INCLUDE_DIRS}")
            set (wxWidgets_INCLUDE_DIRS ${__convert_path})
        endif ()

        # compile time link dirs, useful for rpath on UNIX.
        # Typically an empty string in WIN32 environment.
        if (UNIX)
            set (ENV{LIBRARY_PATH} "${wxWidgets_LIBRARY_DIRS}${TBAG_PATH_SEPARATOR}$ENV{LIBRARY_PATH}")
            list (INSERT CMAKE_LIBRARY_PATH 0 "${wxWidgets_LIBRARY_DIRS}")
        endif ()
    endif ()
endmacro ()

