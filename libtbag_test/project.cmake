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

# User's settings.
tbag_modules__include_project (Default)

list (APPEND TBAG_PROJECT_DEFINITIONS -DUSE_LIBTEST_EXPORT)

