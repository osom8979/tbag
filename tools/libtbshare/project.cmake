## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()
tbag_modules__apply_shared_library ()

list (APPEND TBAG_PROJECT_DEFINITIONS USE_LIBTEST_EXPORT)

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()

