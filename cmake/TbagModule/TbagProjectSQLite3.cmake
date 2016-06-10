#/// @file   TbagProjectSQLite3.cmake
#/// @brief  SQLite3 project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT SQLite3_FOUND)
    message (WARNING "Not found SQLite3.")
endif ()

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${SQLite3_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS      ${SQLite3_LIBRARIES})

