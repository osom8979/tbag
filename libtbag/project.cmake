## Tbag CMake project setting.

tbag_modules__include_project (Default)
tbag_modules__include_project (InstallTarget)

## Thread settings.
list (APPEND TBAG_PROJECT_LDFLAGS ${CMAKE_THREAD_LIBS_INIT})

## -------------
## Dependencies.
## -------------

## dep/lua
list (APPEND TBAG_PROJECT_DEPENDENCIES  lua)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/lua/include)

## dep/uv
list (APPEND TBAG_PROJECT_DEPENDENCIES  uv)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/uv/include)
if (UNIX)
    list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
    if (APPLE)
        list (APPEND TBAG_PROJECT_LDFLAGS -ldl)
    else ()
        list (APPEND TBAG_PROJECT_LDFLAGS -lrt -lnsl -ldl)
    endif ()
else (WIN32)
    list (APPEND TBAG_PROJECT_LDFLAGS ws2_32.lib iphlpapi.lib winmm.lib psapi.lib userenv.lib)
endif ()

## dep/icu
list (APPEND TBAG_PROJECT_DEPENDENCIES  icudata icuuc icui18n)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/icu/common
                                        ${CMAKE_SOURCE_DIR}/dep/icu/i18n)

## whole-archive files.
tbag_modules__include_project (WholeArchiveOn)
list (APPEND TBAG_PROJECT_LDFLAGS lua uv icudata icuuc icui18n)
tbag_modules__include_project (WholeArchiveOff)

#tbag_modules__include_project (PCH ${CMAKE_SOURCE_DIR}/libtbag/config.h)

