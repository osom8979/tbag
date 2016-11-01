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
if (NOT BUILD_SHARED_LIBS)
    list (APPEND TBAG_PROJECT_DEFINITIONS TBAG_STATIC_API)
endif ()

if (USE_SHARED_DEPS)
    ## Dependencies.
    tbag_modules__apply_dep_icu     ()
    tbag_modules__apply_dep_lmdb    ()
    tbag_modules__apply_dep_lzma    ()
    tbag_modules__apply_dep_sqlite3 ()
    tbag_modules__apply_dep_uv      ()
    if (USE_LUA)
        tbag_modules__apply_dep_lua ()
        tbag_modules__apply_dep_luabridge ()
    endif ()
    if (USE_JSONCPP)
        tbag_modules__apply_dep_jsoncpp ()
    endif ()
endif ()


## Test project.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbshare_copy)
list (APPEND TBAG_PROJECT_DEPENDENCIES tbproc_copy)

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()

