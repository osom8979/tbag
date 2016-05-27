#/// @file   TbagModuleSQLite3.cmake
#/// @brief  SQLite3 tbag module.
#/// @author zer0
#/// @date   2016-05-27

#/// Assign SQLite3 libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module_sqlite3__assign)
    if (NOT SQLite3_FOUND)
        find_package (SQLite3)
    endif ()

    if (SQLite3_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_SQLITE3)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${SQLite3_INCLUDE_DIRS})
        list (APPEND TBAG_PROJECT_LDFLAGS      ${SQLite3_LIBRARIES})
    endif ()
endmacro ()

