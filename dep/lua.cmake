#/// @file   lua.cmake
#/// @brief  libtbag-lua library build project.
#/// @author zer0
#/// @date   2016-07-05
#///
#/// @remarks
#///  The following are set after configuration is done:
#///   - ${TBAG_DEP_LUA_NAME}
#///   - ${TBAG_DEP_LUA_INCLUDE_DIRS}
#///   - ${TBAG_DEP_LUA_LIBRARIES}

set (TBAG_DEP_LUA_NAME          "libtbag-lua")
set (TBAG_DEP_LUA_INCLUDE_DIRS  "${PROJECT_SOURCE_DIR}/dep/lua")
set (TBAG_DEP_LUA_LIBRARIES     "${PROJECT_SOURCE_DIR}/dep/lua/liblua.a")
set (TBAG_DEP_LUA_LOG_PREFIX    "${PROJECT_BINARY_DIR}/CMakeFiles/${TBAG_DEP_LUA_NAME}.dir/build.log")

## LUA custom target.
add_custom_target (${TBAG_DEP_LUA_NAME} ALL DEPENDS ${TBAG_DEP_LUA_LIBRARIES})

add_custom_command (
        OUTPUT ${TBAG_DEP_LUA_LIBRARIES}
        COMMAND make posix > "${TBAG_DEP_LUA_LOG_PREFIX}" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/lua/Makefile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/lua/")

