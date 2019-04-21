cmake_minimum_required (VERSION 2.8)
PROJECT (miniz C)

if (CMAKE_BUILD_TYPE STREQUAL "")
    # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
    # differentiation between debug and release builds.
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING
         "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or \
          CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif ()

set (SOURCE miniz.c miniz_zip.c miniz_tinfl.c miniz_tdef.c)
add_library (miniz STATIC ${SOURCE})

