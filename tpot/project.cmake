## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_LDFLAGS      tbag)

if (NOT BUILD_SHARED_LIBS)
    tbag_modules__apply_tbag_static_api ()
endif ()

if (USE_LUAJIT_EXE_FLAGS AND APPLE)
    tbag_modules__apply_luajit_embedding_exe_flags ()
endif ()

if (USE_RTTI)
    tbag_modules__apply_rtti ()
else ()
    tbag_modules__apply_no_rtti ()
endif ()

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()
tbag_modules__install_target ()

