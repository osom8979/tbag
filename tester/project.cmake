## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## Dependencies.
tbag_modules__apply_dep_gtest ()

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      tbag)

## Test project.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbshare_copy)
list (APPEND TBAG_PROJECT_DEPENDENCIES tbproc_copy)

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()

