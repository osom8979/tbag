#/// @file   uv.cmake
#/// @brief  libtbag-uv library build project.
#/// @author zer0
#/// @date   2016-07-04
#///
#/// @remarks
#///  The following are set after configuration is done:
#///   - ${TBAG_DEP_UV_NAME}
#///   - ${TBAG_DEP_UV_LOG_PREFIX}
#///   - ${TBAG_DEP_UV_INCLUDE_DIRS}
#///   - ${TBAG_DEP_UV_LIBRARIES}
#///   - ${TBAG_DEP_UV_ARCHIVES}
#///   - ${TBAG_DEP_UV_DEPENDENCIES}
#///   - ${TBAG_DEP_UV_LIBRARIES}: Archives + Dependencies

if (DEFINED TBAG_DEP_UV_NAME)
    return ()
endif ()

set (TBAG_DEP_UV_NAME         "libtbag-uv")
set (TBAG_DEP_UV_LOG_PREFIX   "${PROJECT_BINARY_DIR}/CMakeFiles/${TBAG_DEP_UV_NAME}.dir/build.log")
set (TBAG_DEP_UV_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/dep/uv/include")
set (TBAG_DEP_UV_ARCHIVES_PRE "${PROJECT_SOURCE_DIR}/dep/uv/.libs/libuv.a")
set (TBAG_DEP_UV_ARCHIVES     "${PROJECT_SOURCE_DIR}/dep/uv/libuv.a")

set (TBAG_DEP_UV_DEPENDENCIES "")
if (UNIX)
    list (APPEND TBAG_DEP_UV_DEPENDENCIES -lpthread)
    if (NOT APPLE)
        list (APPEND TBAG_DEP_UV_DEPENDENCIES -lrt -lnsl -ldl)
    endif ()
endif ()

set (TBAG_DEP_UV_LIBRARIES ${TBAG_DEP_UV_ARCHIVES}
                           ${TBAG_DEP_UV_DEPENDENCIES})

## libuv custom target.
add_custom_target (${TBAG_DEP_UV_NAME} ALL DEPENDS ${TBAG_DEP_UV_ARCHIVES})

add_custom_command (
        OUTPUT "${PROJECT_SOURCE_DIR}/dep/uv/configure"
        COMMAND ./autogen.sh > "${TBAG_DEP_UV_LOG_PREFIX}.1" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/uv/autogen.sh"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/uv/")

add_custom_command (
        OUTPUT "${PROJECT_SOURCE_DIR}/dep/uv/Makefile"
        COMMAND ./configure CFLAGS=-fPIC --disable-shared --enable-static > "${TBAG_DEP_UV_LOG_PREFIX}.2" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/uv/configure"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/uv/")

add_custom_command (
        OUTPUT ${TBAG_DEP_UV_ARCHIVES_PRE}
        COMMAND make V=1 > "${TBAG_DEP_UV_LOG_PREFIX}.3" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/uv/Makefile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/uv/")

add_custom_command (
        OUTPUT ${TBAG_DEP_UV_ARCHIVES}
        COMMAND ${CMAKE_COMMAND} -E copy ${TBAG_DEP_UV_ARCHIVES_PRE} ${TBAG_DEP_UV_ARCHIVES}
        DEPENDS "${TBAG_DEP_UV_ARCHIVES_PRE}"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/uv/")

