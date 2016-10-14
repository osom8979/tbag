## Tbag CMake project setting.

tbag_modules__include_project (Default)

## libtbag-gtest
include (${PROJECT_SOURCE_DIR}/dep/gtest.cmake)
list (APPEND TBAG_PROJECT_DEPENDENCIES  ${TBAG_DEP_GTEST_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_GTEST_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS       ${TBAG_DEP_GTEST_LIBRARIES})

## libtbtest library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbtest)

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      -ltbag)

