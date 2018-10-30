## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

if (USE_TBAG_UI)
    tbag_modules__append_dependencies (tbui)
    tbag_modules__append_ldflags (tbui)
endif ()

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()
tbag_modules__install_target ()

