## Tbag CMake project setting.

tbag_modules__include_project (Default)
tbag_modules__include_project (InstallTarget)

## Thread settings.
list (APPEND TBAG_PROJECT_LDFLAGS ${CMAKE_THREAD_LIBS_INIT})

## -------------
## Dependencies.
## -------------

## libtbag-gtest
list (APPEND TBAG_PROJECT_DEPENDENCIES  lua)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/lua/include)

include (${PROJECT_SOURCE_DIR}/dep/icu.cmake)
include (${PROJECT_SOURCE_DIR}/dep/uv.cmake)

list (APPEND TBAG_PROJECT_DEPENDENCIES  ${TBAG_DEP_ICU_NAME}
                                        ${TBAG_DEP_UV_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_ICU_INCLUDE_DIRS}
                                        ${TBAG_DEP_UV_INCLUDE_DIRS})

## whole-archive files.
tbag_modules__include_project (WholeArchiveOn)
list (APPEND TBAG_PROJECT_LDFLAGS   ${TBAG_DEP_ICU_ARCHIVES}
                                    -llua
                                    ${TBAG_DEP_UV_ARCHIVES})
tbag_modules__include_project (WholeArchiveOff)

list (APPEND TBAG_PROJECT_LDFLAGS ${TBAG_DEP_UV_DEPENDENCIES})

#tbag_modules__include_project (PCH ${CMAKE_SOURCE_DIR}/libtbag/config.h)

