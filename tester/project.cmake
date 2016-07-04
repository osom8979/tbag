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
tbag_modules__include_project (TinyXML2)
tbag_modules__include_project (Spdlog)
tbag_modules__include_project (ICU)

list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      -ltbag)

