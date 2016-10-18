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
endif ()

## dep/icu
list (APPEND TBAG_PROJECT_DEPENDENCIES  icudata icuuc)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/lua/common)

## whole-archive files.
tbag_modules__include_project (WholeArchiveOn)
list (APPEND TBAG_PROJECT_LDFLAGS lua uv icudata icuuc)
tbag_modules__include_project (WholeArchiveOff)

#tbag_modules__include_project (PCH ${CMAKE_SOURCE_DIR}/libtbag/config.h)

