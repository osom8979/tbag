#/// @file   sqlite3.cmake
#/// @brief  libtbag-sqlite3 library build project.
#/// @author zer0
#/// @date   2016-07-04
#///
#/// @remarks
#///  The following are set after configuration is done:
#///   - ${TBAG_DEP_SQLITE3_NAME}
#///   - ${TBAG_DEP_SQLITE3_LOG_PREFIX}
#///   - ${TBAG_DEP_SQLITE3_INCLUDE_DIRS}
#///   - ${TBAG_DEP_SQLITE3_ARCHIVES}
#///   - ${TBAG_DEP_SQLITE3_DEPENDENCIES}
#///   - ${TBAG_DEP_SQLITE3_LIBRARIES}: Archives + Dependencies

set (TBAG_DEP_SQLITE3_NAME          "libtbag-sqlite3")
set (TBAG_DEP_SQLITE3_LOG_PREFIX    "${PROJECT_BINARY_DIR}/CMakeFiles/${TBAG_DEP_SQLITE3_NAME}.dir/build.log")
set (TBAG_DEP_SQLITE3_INCLUDE_DIRS  "${PROJECT_SOURCE_DIR}/dep/sqlite3")
set (TBAG_DEP_SQLITE3_ARCHIVES      "${PROJECT_SOURCE_DIR}/dep/sqlite3/libsqlite3.a")
set (TBAG_DEP_SQLITE3_DEPENDENCIES  "")
set (TBAG_DEP_SQLITE3_LIBRARIES     ${TBAG_DEP_SQLITE3_ARCHIVES}
                                    ${TBAG_DEP_SQLITE3_DEPENDENCIES})

## SQLite3 custom target.
add_custom_target (${TBAG_DEP_SQLITE3_NAME} ALL DEPENDS ${TBAG_DEP_SQLITE3_ARCHIVES})

add_custom_command (
        OUTPUT ${TBAG_DEP_SQLITE3_ARCHIVES}
        COMMAND make > "${TBAG_DEP_SQLITE3_LOG_PREFIX}" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/sqlite3/Makefile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/sqlite3/")

