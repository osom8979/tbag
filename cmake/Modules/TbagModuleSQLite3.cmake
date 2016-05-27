#/// @file   TbagModuleSQLite3.cmake
#/// @brief  SQLite3 tbag cmake module.
#/// @author zer0
#/// @date   2016-05-27

#/// Assign SQLite3 libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module__assign_sqlite3)
    if (NOT SQLITE3_FOUND)
        find_package (SQLite3)
    endif ()

    if (SQLITE3_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_SQLITE3)
        list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${SQLITE3_INCLUDE_DIRS})
        list (APPEND TBAG_PROJECT_LDFLAGS      ${SQLITE3_LIBRARIES})
    endif ()
endmacro ()

