## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()

## gtest
list (APPEND TBAG_PROJECT_DEPENDENCIES  gtest_main)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/gtest/include)
list (APPEND TBAG_PROJECT_LDFLAGS       gtest_main)

if (CMAKE_USE_PTHREADS_INIT)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        list (APPEND TBAG_PROJECT_LDFLAGS -lpthread)
    endif ()
endif ()

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      tbag)

## Test project.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbshare_copy)
list (APPEND TBAG_PROJECT_DEPENDENCIES tbproc_copy)

