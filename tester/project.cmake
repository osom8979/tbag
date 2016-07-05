## Tbag CMake project setting.
#
# List of project modules:
#  tbag_module__print_projects ()
#
# List of project module properties:
#  tbag_modules__include_project (Preview)
#
# Default project setting:
#  tbag_modules__include_project (Default)

tbag_modules__include_project (Default)

## TODO Remove this code.
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_UV_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_SPDLOG_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_SQLITE3_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_TINYXML2_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_ICU_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_LUA_INCLUDE_DIRS})

## libtbag-gtest
include (${PROJECT_SOURCE_DIR}/dep/gtest.cmake)
list (APPEND TBAG_PROJECT_DEPENDENCIES  ${TBAG_DEP_GTEST_NAME})
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_GTEST_INCLUDE_DIRS})
list (APPEND TBAG_PROJECT_LDFLAGS       ${TBAG_DEP_GTEST_LIBRARIES})

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      -ltbag)

