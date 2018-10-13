## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()
tbag_modules__update_default_objects ()

## TBAG EXPORT API.
tbag_modules__append_definitions (
    TBAG_EXPORT_API
    U_COMBINED_IMPLEMENTATION
    JSON_DLL_BUILD
    TINYXML2_EXPORT
    LMDB_EXPORT_API
    SQLITE_EXPORT_API)
tbag_modules__apply_dep_lua_export_macros ()

## Thread settings.
tbag_modules__apply_thread ()

## Tbag debugging logger.
if (USE_TBAG_DEBUGGING_LOGGER)
    tbag_modules__append_definitions (ENABLE_TBAG_LIBRARY_DEBUGGING_LOG)
endif ()

## External.
tbag_modules__apply_ext_uv    ()
tbag_modules__apply_ext_z     ()
tbag_modules__apply_ext_ressl ()

## Dependencies.
tbag_modules__apply_dep_http_parser ()
tbag_modules__apply_dep_icu         ()
tbag_modules__apply_dep_minizip     () ## And apply_dep_z
tbag_modules__apply_dep_stb         ()
tbag_modules__apply_dep_flatbuffers ()
tbag_modules__apply_dep_lemon       ()
tbag_modules__apply_dep_nng         ()

set (TPOT_FBS_SRC  "${CMAKE_SOURCE_DIR}/libtbag/proto/fbs/tpot.fbs")
set (TPOT_T2S_PATH "${CMAKE_SOURCE_DIR}/libtbag/proto/fbs/tpot_t2s.h")
tbag_modules__build_dep_flatc_cpp (tpot_fbs "${TPOT_FBS_SRC}")
tbag_modules__text_to_cpp11string (tpot_t2s "${TPOT_T2S_PATH}" tpot __tpot__ "${TPOT_FBS_SRC}")

if (CUDA_FOUND)
    tbag_modules__check_cuda ()
    tbag_modules__apply_cuda ()

    set (CUDA_PROPAGATE_HOST_FLAGS OFF)
    tbag_modules__update_cuda_objects ()
endif ()

if (WIN32)
    tbag_modules__append_ldflags (shlwapi.lib) # filesystem with windows.
endif ()

if (WITH_COVERAGE)
    tbag_modules__apply_gcov ()
endif ()

## Final process.
tbag_modules__add_target ()
tbag_modules__update_all_properties ()
tbag_modules__update_default_version ()
tbag_modules__install_target ()
tbag_modules__install_default_cxx_headers ()

