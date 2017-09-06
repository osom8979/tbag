## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## TBAG EXPORT API.
list (APPEND TBAG_PROJECT_DEFINITIONS TBAG_EXPORT_API)
list (APPEND TBAG_PROJECT_DEFINITIONS U_COMBINED_IMPLEMENTATION)
list (APPEND TBAG_PROJECT_DEFINITIONS JSON_DLL_BUILD)
list (APPEND TBAG_PROJECT_DEFINITIONS TINYXML2_EXPORT)

## Thread settings.
list (APPEND TBAG_PROJECT_LDFLAGS ${CMAKE_THREAD_LIBS_INIT})

## Tbag debugging logger.
if (USE_TBAG_DEBUGGING_LOGGER)
    list (APPEND TBAG_PROJECT_DEFINITIONS ENABLE_TBAG_LIBRARY_DEBUGGING_LOG)
endif ()

## External.
tbag_modules__apply_ext_z    ()
tbag_modules__apply_ext_ressl()

## Dependencies.
tbag_modules__apply_dep_http_parser ()
tbag_modules__apply_dep_icu         ()
tbag_modules__apply_dep_lmdb        ()
tbag_modules__apply_dep_minizip     () ## And apply_dep_z
tbag_modules__apply_dep_sqlite3     ()
tbag_modules__apply_dep_uv          ()
tbag_modules__apply_dep_lua         ()
tbag_modules__apply_dep_stb         ()
tbag_modules__apply_dep_flatbuffers ()

set (TBAG_FBS_SRC "${CMAKE_SOURCE_DIR}/libtbag/proto/fbs/tpot.fbs")
set (TPOT_FBS_SRC "${CMAKE_SOURCE_DIR}/libtbag/proto/fbs/tbag.fbs")
tbag_modules__build_dep_flatc_cpp (tbag_fbs ${TBAG_FBS_SRC})
tbag_modules__build_dep_flatc_cpp (tpot_fbs ${TPOT_FBS_SRC})

if (OpenCL_FOUND)
    list (APPEND TBAG_PROJECT_INCLUDE_DIRS ${OpenCL_INCLUDE_DIR})
    list (APPEND TBAG_PROJECT_LDFLAGS      ${OpenCL_LIBRARY})
endif ()

if (CUDA_FOUND)
    tbag_modules__check_cuda ()
    tbag_modules__apply_cuda ()

    set (CUDA_PROPAGATE_HOST_FLAGS OFF)
    tbag_modules__update_cuda_objects ()
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

