## Tbag CMake project setting.

tbag_modules__include_project (Default)

## SFML settings.
if (SFML_FOUND)
    tbag_modules__include_project (SFML)
endif ()

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      -ltbag)

