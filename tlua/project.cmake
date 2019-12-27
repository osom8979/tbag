## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## Dependencies.
tbag_modules__apply_dep_lua_import_macros ()

## tbag targets.
tbag_modules__append_dependencies (tbag)
tbag_modules__append_ldflags (tbag)

if (NOT BUILD_SHARED_LIBS)
    tbag_modules__apply_tbag_static_api ()
endif ()

if (NOT DISABLE_LUAJIT_EXE_FLAGS)
    tbag_modules__apply_luajit_embedding_exe_flags ()
endif ()

if (USE_RTTI)
    tbag_modules__apply_rtti ()
else ()
    tbag_modules__apply_no_rtti ()
endif ()

if (WITH_COVERAGE)
    tbag_modules__apply_gcov ()
endif ()

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()

