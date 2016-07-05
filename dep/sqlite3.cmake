#/// @file   sqlite3.cmake
#/// @brief  libtbag-sqlite3 library build project.
#/// @author zer0
#/// @date   2016-07-04
#///
#/// @remarks
#///  The following are set after configuration is done:
#///   - ${TBAG_DEP_SQLITE3_NAME}
#///   - ${TBAG_DEP_SQLITE3_INCLUDE_DIRS}
#///   - ${TBAG_DEP_SQLITE3_LIBRARIES}

set (TBAG_DEP_SQLITE3_NAME          "libtbag-sqlite3")
set (TBAG_DEP_SQLITE3_INCLUDE_DIRS  "${PROJECT_SOURCE_DIR}/dep/sqlite3")
set (TBAG_DEP_SQLITE3_LIBRARIES     "${PROJECT_SOURCE_DIR}/dep/sqlite3/.libs/libsqlite3.a")

## SQLite3 custom target.
add_custom_target (${TBAG_DEP_SQLITE3_NAME} ALL DEPENDS ${TBAG_DEP_SQLITE3_LIBRARIES})

add_custom_command (
        OUTPUT "${PROJECT_SOURCE_DIR}/dep/sqlite3/Makefile"
        COMMAND ./configure --disable-shared --enable-static
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/sqlite3/configure"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/sqlite3/")

add_custom_command (
        OUTPUT ${TBAG_DEP_SQLITE3_LIBRARIES}
               "${PROJECT_SOURCE_DIR}/dep/sqlite3/libsqlite3.la"
        COMMAND make
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/sqlite3/Makefile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/sqlite3/")

