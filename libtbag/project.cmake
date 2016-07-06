## Tbag CMake project setting.

tbag_modules__include_project (Default)
tbag_modules__include_project (InstallTarget)

## Thread settings.
list (APPEND TBAG_PROJECT_LDFLAGS ${CMAKE_THREAD_LIBS_INIT})

## -------------
## Dependencies.
## -------------

## Insert archive files.
tbag_modules__include_project (WholeArchiveOn)

## libtbag-uv
include (${PROJECT_SOURCE_DIR}/dep/uv.cmake)
list (APPEND TBAG_PROJECT_DEPENDENCIES  ${TBAG_DEP_UV_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_UV_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS       ${TBAG_DEP_UV_LIBRARIES})

## libtbag-spdlog
include (${PROJECT_SOURCE_DIR}/dep/spdlog.cmake)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_SPDLOG_INCLUDE_DIRS})

## libtbag-sqlite3
include (${PROJECT_SOURCE_DIR}/dep/sqlite3.cmake)
list (APPEND TBAG_PROJECT_DEPENDENCIES  ${TBAG_DEP_SQLITE3_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_SQLITE3_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS       ${TBAG_DEP_SQLITE3_LIBRARIES})

## libtbag-tinyxml2
include (${PROJECT_SOURCE_DIR}/dep/tinyxml2.cmake)
list (APPEND TBAG_PROJECT_DEPENDENCIES  ${TBAG_DEP_TINYXML2_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_TINYXML2_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS       ${TBAG_DEP_TINYXML2_LIBRARIES})

## libtbag-icu
include (${PROJECT_SOURCE_DIR}/dep/icu.cmake)
list (APPEND TBAG_PROJECT_DEPENDENCIES  ${TBAG_DEP_ICU_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_ICU_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS       ${TBAG_DEP_ICU_ARCHIVES})

## libtbag-lua
include (${PROJECT_SOURCE_DIR}/dep/lua.cmake)
list (APPEND TBAG_PROJECT_DEPENDENCIES  ${TBAG_DEP_LUA_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_LUA_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS       ${TBAG_DEP_LUA_LIBRARIES})

tbag_modules__include_project (WholeArchiveOff)

list (APPEND TBAG_PROJECT_LDFLAGS ${TBAG_DEP_UV_DEPENDENCIES})

