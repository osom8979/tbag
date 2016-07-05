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
set (TBAG_DEP_SQLITE3_LIBRARIES     "${PROJECT_SOURCE_DIR}/dep/sqlite3/libsqlite3.a")
set (TBAG_DEP_SQLITE3_LOG_PREFIX    "${PROJECT_BINARY_DIR}/CMakeFiles/${TBAG_DEP_SQLITE3_NAME}.dir/build.log")

## SQLite3 custom target.
add_custom_target (${TBAG_DEP_SQLITE3_NAME} ALL DEPENDS ${TBAG_DEP_SQLITE3_LIBRARIES})

add_custom_command (
        OUTPUT ${TBAG_DEP_SQLITE3_LIBRARIES}
        COMMAND make > "${TBAG_DEP_SQLITE3_LOG_PREFIX}" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/sqlite3/Makefile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/sqlite3/")

