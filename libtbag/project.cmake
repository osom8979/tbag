## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()

tbag_modules__update_c_family_subdir_objects (
        3rd/demangle 3rd/entt 3rd/fmt 3rd/imgui 3rd/jsoncpp 3rd/lmdb
        3rd/lmdbxx 3rd/sol2 3rd/sqlite_orm 3rd/tinyxml2
        algorithm animation app archive bitwise box codec common
        container crypto database debug dom dummy filesystem flow
        functional game geometry gpu graph graphic http id io iterator
        lib locale lock lockfree log loop macro math memory mq net
        network parallel pattern preprocessor process proto random
        ray res rstl scene science script security signal string system
        thread tiled time tmp tpot tty type typography util uvpp uvxx)
tbag_modules__update_subdir_object (Err.cpp libtbag.cpp)

## TBAG EXPORT API.
tbag_modules__append_definitions (
        TBAG_EXPORT_API
        U_COMBINED_IMPLEMENTATION
        JSON_DLL_BUILD
        TINYXML2_EXPORT
        LMDB_EXPORT_API
        GL_SILENCE_DEPRECATION)

## Thread settings.
tbag_modules__apply_thread ()

## External.
tbag_modules__apply_ext_uv       ()
tbag_modules__apply_ext_z        ()
tbag_modules__apply_ext_ressl    ()
tbag_modules__apply_ext_luajit   ()
tbag_modules__apply_ext_luajit_install_conf (luaconf_copy "${CMAKE_SOURCE_DIR}/libtbag/script/luajit-2.0.5/luaconf.h")
tbag_modules__apply_ext_freetype ()
tbag_modules__apply_ext_bzip2    ()
tbag_modules__apply_ext_lzma     ()
tbag_modules__apply_ext_archive  ()
tbag_modules__apply_ext_nng      ()
tbag_modules__apply_ext_glfw     ()
tbag_modules__apply_ext_sqlite3  ()
tbag_modules__apply_ext_date     ()

## Dependencies.
tbag_modules__apply_dep_http_parser ()
tbag_modules__apply_dep_icu         ()
tbag_modules__apply_dep_minizip     () ## And apply_dep_z
tbag_modules__apply_dep_stb         ()
tbag_modules__apply_dep_flatbuffers ()
tbag_modules__apply_dep_lemon       ()
tbag_modules__apply_dep_lfds        ()
tbag_modules__apply_dep_imgui       ()
tbag_modules__apply_dep_imnodes     ()
tbag_modules__apply_dep_raylib      ()

tbag_modules__check_opengl ()
tbag_modules__apply_opengl ()

set (BOX_FBS_SRC  "${CMAKE_SOURCE_DIR}/libtbag/box/fbs/box.fbs")
set (BOX_D2S_PATH "${CMAKE_SOURCE_DIR}/libtbag/box/fbs/box_d2s.h")
tbag_modules__build_dep_flatc_cpp (box_fbs "${BOX_FBS_SRC}")
tbag_modules__data_to_string (box_d2s "${BOX_D2S_PATH}" box "${BOX_FBS_SRC}" OFF)

## Nanum Gothic Coding Normal (NGCN)
set (NGCN_SRC "${CMAKE_SOURCE_DIR}/libtbag/typography/font/ngc/NanumGothicCoding.ttf")
set (NGCN_D2S "${CMAKE_SOURCE_DIR}/libtbag/typography/font/ngc/ngcn.h.inl")
tbag_modules__data_to_7z_to_string (ngcn_d2s "${NGCN_D2S}" ngcn "${NGCN_SRC}")

## Nanum Gothic Coding Bold (NGCB)
set (NGCB_SRC "${CMAKE_SOURCE_DIR}/libtbag/typography/font/ngc/NanumGothicCoding-Bold.ttf")
set (NGCB_D2S "${CMAKE_SOURCE_DIR}/libtbag/typography/font/ngc/ngcb.h.inl")
tbag_modules__data_to_7z_to_string (ngcb_d2s "${NGCB_D2S}" ngcb "${NGCB_SRC}")

if (USE_CUDA AND CUDA_FOUND)
    tbag_modules__check_cuda ()
    tbag_modules__apply_cuda ()

    set (CUDA_PROPAGATE_HOST_FLAGS OFF)
    tbag_modules__update_cuda_objects ()
endif ()

if (USE_OPENCL AND OpenCL_FOUND)
    tbag_modules__append_definitions (CL_SILENCE_DEPRECATION)
    tbag_modules__check_opencl ()
    tbag_modules__apply_opencl ()
endif ()

if (WIN32)
    tbag_modules__append_ldflags (shlwapi.lib) # filesystem with windows.
    if (MSVC)
        tbag_modules__apply_disable_warnings ()
    endif ()
endif ()

if (USE_RTTI)
    tbag_modules__apply_rtti ()
else ()
    tbag_modules__apply_no_rtti ()
endif ()

if (DISABLE_TDLOG)
    tbag_modules__append_definitions (DISABLE_TBAG_LOG)
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

