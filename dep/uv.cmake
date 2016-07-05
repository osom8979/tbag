#/// @file   uv.cmake
#/// @brief  libtbag-uv library build project.
#/// @author zer0
#/// @date   2016-07-04
#///
#/// @remarks
#///  The following are set after configuration is done:
#///   - ${TBAG_DEP_UV_NAME}
#///   - ${TBAG_DEP_UV_INCLUDE_DIRS}
#///   - ${TBAG_DEP_UV_LIBRARIES}

set (TBAG_DEP_UV_NAME          "libtbag-uv")
set (TBAG_DEP_UV_INCLUDE_DIRS  "${PROJECT_SOURCE_DIR}/dep/uv/include")
set (TBAG_DEP_UV_LIBRARIES     "${PROJECT_SOURCE_DIR}/dep/uv/.libs/libuv.a")
set (TBAG_DEP_UV_LOG_PREFIX    "${PROJECT_BINARY_DIR}/CMakeFiles/${TBAG_DEP_UV_NAME}.dir/build.log")

## libuv custom target.
add_custom_target (${TBAG_DEP_UV_NAME} ALL DEPENDS ${TBAG_DEP_UV_LIBRARIES})

add_custom_command (
        OUTPUT "${PROJECT_SOURCE_DIR}/dep/uv/configure"
        COMMAND ./autogen.sh > "${TBAG_DEP_UV_LOG_PREFIX}.1" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/uv/autogen.sh"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/uv/")

add_custom_command (
        OUTPUT "${PROJECT_SOURCE_DIR}/dep/uv/Makefile"
        COMMAND ./configure --disable-shared --enable-static > "${TBAG_DEP_UV_LOG_PREFIX}.2" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/uv/configure"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/uv/")

add_custom_command (
        OUTPUT ${TBAG_DEP_UV_LIBRARIES}
               "${PROJECT_SOURCE_DIR}/dep/uv/libuv.la"
        COMMAND make "CFLAGS=-w -O2 -fvisibility=hidden -fPIC" V=1 > "${TBAG_DEP_UV_LOG_PREFIX}.3" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/uv/Makefile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/uv/")

