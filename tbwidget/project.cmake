## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## SFML settings.
if (SFML_FOUND)
    tbag_modules__apply_sfml ()
endif ()

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      tbag)

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()

