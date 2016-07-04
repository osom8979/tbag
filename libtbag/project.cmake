## Tbag CMake project setting.

tbag_modules__include_project (Default)
tbag_modules__include_project (InstallTarget)

list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/dep/uv/include)

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

## libtbag-sqlite3
include (${PROJECT_SOURCE_DIR}/dep/sqlite3.cmake)
list (APPEND TBAG_PROJECT_DEPENDENCIES  ${TBAG_DEP_SQLITE3_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_SQLITE3_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS       ${TBAG_DEP_SQLITE3_LIBRARIES})

tbag_modules__include_project (WholeArchiveOff)

