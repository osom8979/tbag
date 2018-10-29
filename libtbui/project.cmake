## Tbag CMake project setting.

if (NOT USE_TBAG_UI)
    message (STATUS "Disable libtbui library.")
    return ()
endif ()

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

set (TBUI_MAJOR_VERSION ${VERSION_MAJOR})
set (TBUI_MINOR_VERSION ${VERSION_MINOR})

set (TBUI_VERSION   "${TBUI_MAJOR_VERSION}.${TBUI_MINOR_VERSION}")
set (TBUI_SOVERSION "${SOVERSION}")

tbag_modules__append_definitions (
        TBUI_MAJOR_VERSION=${TBUI_MAJOR_VERSION}
        TBUI_MINOR_VERSION=${TBUI_MINOR_VERSION}
        TBUI_EXPORT_API)

## Thread settings.
tbag_modules__apply_thread ()

## External.
tbag_modules__apply_ext_ogg      ()
tbag_modules__apply_ext_flac     ()
tbag_modules__apply_ext_vorbis   ()
tbag_modules__apply_ext_freetype ()
tbag_modules__apply_ext_sfml     ()

## Dependencies.
tbag_modules__apply_dep_lua_import_macros ()

## Thread
tbag_modules__apply_thread ()

## Tbag
tbag_modules__append_dependencies (tbag)
tbag_modules__append_ldflags (tbag)

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()
tbag_modules__update_version (${TBUI_VERSION} ${TBUI_SOVERSION})
tbag_modules__install_target ()

