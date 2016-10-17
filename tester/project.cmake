## Tbag CMake project setting.

tbag_modules__include_project (Default)

## libtbag-gtest
list (APPEND TBAG_PROJECT_DEPENDENCIES  gtest_main)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/dep/gtest/include)
list (APPEND TBAG_PROJECT_LDFLAGS       gtest_main)
if (UNIX AND CMAKE_USE_PTHREADS_INIT)
    list (APPEND TBAG_PROJECT_LDFLAGS   -lpthread)
endif ()

## libtbag library.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbag)
list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libtbag)
list (APPEND TBAG_PROJECT_LDFLAGS      tbag)

## Test project.
list (APPEND TBAG_PROJECT_DEPENDENCIES tbtest)
list (APPEND TBAG_PROJECT_DEPENDENCIES tbproc)
list (APPEND TBAG_PROJECT_DEPENDENCIES tbserver)
list (APPEND TBAG_PROJECT_DEPENDENCIES tbclient)

