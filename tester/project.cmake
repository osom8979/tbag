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

# Extension.
tbag_modules__include_project (GTest)
tbag_modules__include_project (ICU)

## libtbag-spdlog
include (${PROJECT_SOURCE_DIR}/dep/spdlog.cmake)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${TBAG_DEP_SPDLOG_INCLUDE_DIRS})

list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      -ltbag)

