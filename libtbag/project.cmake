## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## TBAG EXPORT API.
list (APPEND TBAG_PROJECT_DEFINITIONS TBAG_EXPORT_API U_COMBINED_IMPLEMENTATION)

## Thread settings.
list (APPEND TBAG_PROJECT_LDFLAGS ${CMAKE_THREAD_LIBS_INIT})

## -------------
## Dependencies.
## -------------

## dep/icu
list (APPEND TBAG_PROJECT_DEPENDENCIES  icuuc icui18n)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/icu/common
                                        ${CMAKE_SOURCE_DIR}/dep/icu/i18n)

## dep/lua
list (APPEND TBAG_PROJECT_DEPENDENCIES  lua)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/lua/include)

## dep/sqlite3
list (APPEND TBAG_PROJECT_DEPENDENCIES  sqlite3)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/sqlite3)

## dep/uv
list (APPEND TBAG_PROJECT_DEPENDENCIES  uv)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/uv/include)

## whole-archive files.
tbag_modules__add_whole_archive ($<TARGET_FILE:icuuc>)
tbag_modules__add_whole_archive ($<TARGET_FILE:icui18n>)
tbag_modules__add_whole_archive ($<TARGET_FILE:lua>)
tbag_modules__add_whole_archive ($<TARGET_FILE:sqlite3>)
tbag_modules__add_whole_archive ($<TARGET_FILE:uv>)

## external libraries.
if (WIN32)
    list (APPEND TBAG_PROJECT_LDFLAGS advapi32.lib iphlpapi.lib psapi.lib shell32.lib
                                      user32.lib userenv.lib winmm.lib ws2_32.lib)
else ()
    list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
    if (APPLE)
        list (APPEND TBAG_PROJECT_LDFLAGS -ldl)
    else ()
        list (APPEND TBAG_PROJECT_LDFLAGS -lnsl -ldl -lrt)
    endif ()
endif ()

#tbag_modules__apply_pch (${CMAKE_SOURCE_DIR}/libtbag/config.h)

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()
tbag_modules__update_default_version ()
tbag_modules__install_target ()
tbag_modules__install_default_cxx_headers ()

