## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## Tbag debugging logger.
list (APPEND TBAG_PROJECT_DEFINITIONS ENABLE_TBAG_LIBRARY_DEBUGGING_LOG)

## Dependencies.
tbag_modules__apply_dep_gtest ()

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      tbag)
if (NOT BUILD_SHARED_LIBS)
    list (APPEND TBAG_PROJECT_DEFINITIONS TBAG_STATIC_API)
endif ()

## Test project.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbshare_copy)
list (APPEND TBAG_PROJECT_DEPENDENCIES tbproc_copy)

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()

