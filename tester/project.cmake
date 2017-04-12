## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## Dependencies.
tbag_modules__apply_dep_gtest ()

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_LDFLAGS      tbag)

if (NOT BUILD_SHARED_LIBS)
    tbag_modules__apply_tbag_static_api ()
endif ()

## Test project.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbshare)
list (APPEND TBAG_PROJECT_DEPENDENCIES tbproc)

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()
tbag_modules__install_target ()

