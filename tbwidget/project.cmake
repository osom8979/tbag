## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()

## SFML settings.
if (SFML_FOUND)
    tbag_modules__apply_sfml ()
endif ()

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      tbag)

