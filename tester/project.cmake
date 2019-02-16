## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## Dependencies.
tbag_modules__apply_dep_gtest ()
tbag_modules__apply_dep_lua_import_macros ()

## tbag targets.
tbag_modules__append_dependencies (tbag tbshare tbproc)
tbag_modules__append_ldflags (tbag)
tbag_modules__append_definitions ("DEMO_ASSET_DIR=\"${TBAG_PROJECT_CONST_DIR_PATH}/asset\"")
tbag_modules__append_definitions ("DEMO_TCP_PORT=8080")

if (NOT BUILD_SHARED_LIBS)
    tbag_modules__apply_tbag_static_api ()
endif ()

if (WITH_COVERAGE)
    tbag_modules__apply_gcov ()
endif ()

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()

