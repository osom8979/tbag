## Tbag CMake project setting.

include (TbagModules)

tbag_modules__apply_default ()

tbag_modules__update_c_family_subdir_objects (
        3rd/date 3rd/demangle 3rd/entt 3rd/fmt 3rd/jsoncpp 3rd/lmdb
        3rd/lmdbxx 3rd/sol2 3rd/sqlite3 3rd/sqlite_orm 3rd/tinyxml2
        algorithm animation app archive bitwise codec common container
        crypto database debug dom dummy filesystem flow functional
        geometry gpu graph graphic gui http id io iterator lib locale
        lock lockfree log loop macro math memory mq net network pattern
        preprocessor process proto random res rstl science script
        security signal string system thread time tmp tpot tty type
        typography util uvpp uvxx)
tbag_modules__update_subdir_object (Err.cpp libtbag.cpp)

## TBAG EXPORT API.
tbag_modules__append_definitions (
    TBAG_EXPORT_API
    U_COMBINED_IMPLEMENTATION
    JSON_DLL_BUILD
    TINYXML2_EXPORT
    LMDB_EXPORT_API
    SQLITE_EXPORT_API)

## Thread settings.
tbag_modules__apply_thread ()

## External.
tbag_modules__apply_ext_uv       ()
tbag_modules__apply_ext_z        ()
tbag_modules__apply_ext_ressl    ()
tbag_modules__apply_ext_luajit   ()
tbag_modules__apply_ext_luajit_install_conf (luaconf_copy "${CMAKE_SOURCE_DIR}/libtbag/script/luajit-2.0.3/luaconf.h")
tbag_modules__apply_ext_ogg      ()
tbag_modules__apply_ext_flac     ()
tbag_modules__apply_ext_vorbis   ()
tbag_modules__apply_ext_freetype ()

## Dependencies.
tbag_modules__apply_dep_http_parser ()
tbag_modules__apply_dep_icu         ()
tbag_modules__apply_dep_minizip     () ## And apply_dep_z
tbag_modules__apply_dep_stb         ()
tbag_modules__apply_dep_flatbuffers ()
tbag_modules__apply_dep_lemon       ()
tbag_modules__apply_dep_lfds        ()

if (USE_GUI)
    tbag_modules__append_definitions (SFML_STATIC GL_SILENCE_DEPRECATION)
    tbag_modules__update_c_family_subdir_objects (3rd/imgui)
    tbag_modules__apply_ext_sfml_ignore_gl_al ()
    tbag_modules__check_opengl ()
    tbag_modules__apply_opengl ()
endif ()

set (BOX_FBS_SRC  "${CMAKE_SOURCE_DIR}/libtbag/proto/fbs/box.fbs")
set (BOX_T2S_PATH "${CMAKE_SOURCE_DIR}/libtbag/proto/fbs/box_t2s.h")
tbag_modules__build_dep_flatc_cpp (box_fbs "${BOX_FBS_SRC}")
tbag_modules__text_to_cpp11string (box_t2s "${BOX_T2S_PATH}" box __box__ "${BOX_FBS_SRC}")

set (MSG_FBS_SRC  "${CMAKE_SOURCE_DIR}/libtbag/proto/fbs/msg.fbs")
set (MSG_T2S_PATH "${CMAKE_SOURCE_DIR}/libtbag/proto/fbs/msg_t2s.h")
tbag_modules__build_dep_flatc_cpp (msg_fbs "${MSG_FBS_SRC}")
tbag_modules__text_to_cpp11string (msg_t2s "${MSG_T2S_PATH}" msg __msg__ "${MSG_FBS_SRC}")

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

