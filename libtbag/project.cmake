## Tbag CMake project setting.

tbag_modules__include_project (Default)
tbag_modules__include_project (InstallTarget)

## Thread settings.
list (APPEND TBAG_PROJECT_LDFLAGS ${CMAKE_THREAD_LIBS_INIT})

## -------------
## Dependencies.
## -------------

include (${PROJECT_SOURCE_DIR}/dep/icu.cmake)
include (${PROJECT_SOURCE_DIR}/dep/lua.cmake)
include (${PROJECT_SOURCE_DIR}/dep/sqlite3.cmake)
include (${PROJECT_SOURCE_DIR}/dep/tinyxml2.cmake)
include (${PROJECT_SOURCE_DIR}/dep/uv.cmake)

list (APPEND TBAG_PROJECT_DEPENDENCIES  ${TBAG_DEP_ICU_NAME}
                                        ${TBAG_DEP_LUA_NAME}
                                        ${TBAG_DEP_SQLITE3_NAME}
                                        ${TBAG_DEP_TINYXML2_NAME}
                                        ${TBAG_DEP_UV_NAME})

list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_ICU_INCLUDE_DIRS}
                                        ${TBAG_DEP_LUA_INCLUDE_DIRS}
                                        ${TBAG_DEP_SQLITE3_INCLUDE_DIRS}
                                        ${TBAG_DEP_TINYXML2_INCLUDE_DIRS}
                                        ${TBAG_DEP_UV_INCLUDE_DIRS})

## whole-archive files.
tbag_modules__include_project (WholeArchiveOn)
list (APPEND TBAG_PROJECT_LDFLAGS   ${TBAG_DEP_ICU_ARCHIVES}
                                    ${TBAG_DEP_LUA_ARCHIVES}
                                    ${TBAG_DEP_SQLITE3_ARCHIVES}
                                    ${TBAG_DEP_TINYXML2_ARCHIVES}
                                    ${TBAG_DEP_UV_ARCHIVES})
tbag_modules__include_project (WholeArchiveOff)

list (APPEND TBAG_PROJECT_LDFLAGS ${TBAG_DEP_UV_DEPENDENCIES})

