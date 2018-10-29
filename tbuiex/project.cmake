## Tbag CMake project setting.

if (NOT USE_TBAG_UI)
    message (STATUS "Disable tbuiex library.")
    return ()
endif ()

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## libtbag library.
tbag_modules__append_dependencies (tbag tbui)
tbag_modules__append_ldflags (tbag tbui)

if (NOT BUILD_SHARED_LIBS)
    tbag_modules__apply_tbag_static_api ()
endif ()

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()

