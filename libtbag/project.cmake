## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## TBAG EXPORT API.
list (APPEND TBAG_PROJECT_DEFINITIONS TBAG_EXPORT_API U_COMBINED_IMPLEMENTATION)

## Thread settings.
list (APPEND TBAG_PROJECT_LDFLAGS ${CMAKE_THREAD_LIBS_INIT})

## Tbag debugging logger.
if (USE_TBAG_DEBUGGING_LOGGER)
    list (APPEND TBAG_PROJECT_DEFINITIONS ENABLE_TBAG_LIBRARY_DEBUGGING_LOG)
endif ()

## Dependencies.
tbag_modules__apply_dep_icu     ()
tbag_modules__apply_dep_lmdb    ()
tbag_modules__apply_dep_lzma    ()
tbag_modules__apply_dep_minizip () ## And apply_dep_z
tbag_modules__apply_dep_sqlite3 ()
tbag_modules__apply_dep_uv      ()
tbag_modules__apply_dep_z       ()
if (USE_LUA)
    tbag_modules__apply_dep_lua ()
    tbag_modules__apply_dep_luabridge ()
endif ()

if (WIN32)
    list (APPEND TBAG_PROJECT_LDFLAGS shlwapi.lib) # filesystem with windows.
endif ()

#tbag_modules__apply_pch (${CMAKE_SOURCE_DIR}/libtbag/config.h)

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()
tbag_modules__update_default_version ()
tbag_modules__install_target ()
tbag_modules__install_default_cxx_headers ()

