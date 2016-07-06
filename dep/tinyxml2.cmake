#/// @file   tinyxml2.cmake
#/// @brief  libtbag-tinyxml2 library build project.
#/// @author zer0
#/// @date   2016-07-04
#///
#/// @remarks
#///  The following are set after configuration is done:
#///   - ${TBAG_DEP_TINYXML2_NAME}
#///   - ${TBAG_DEP_TINYXML2_LOG_PREFIX}
#///   - ${TBAG_DEP_TINYXML2_INCLUDE_DIRS}
#///   - ${TBAG_DEP_TINYXML2_ARCHIVES}
#///   - ${TBAG_DEP_TINYXML2_DEPENDENCIES}
#///   - ${TBAG_DEP_TINYXML2_LIBRARIES}: Archives + Dependencies

set (TBAG_DEP_TINYXML2_NAME          "libtbag-tinyxml2")
set (TBAG_DEP_TINYXML2_LOG_PREFIX    "${PROJECT_BINARY_DIR}/CMakeFiles/${TBAG_DEP_TINYXML2_NAME}.dir/build.log")
set (TBAG_DEP_TINYXML2_INCLUDE_DIRS  "${PROJECT_SOURCE_DIR}/dep/tinyxml2")
set (TBAG_DEP_TINYXML2_ARCHIVES      "${PROJECT_SOURCE_DIR}/dep/tinyxml2/libtinyxml2.a")
set (TBAG_DEP_TINYXML2_DEPENDENCIES  "")
set (TBAG_DEP_TINYXML2_LIBRARIES     ${TBAG_DEP_TINYXML2_ARCHIVES}
                                     ${TBAG_DEP_TINYXML2_DEPENDENCIES})

## TinyXML2 custom target.
add_custom_target (${TBAG_DEP_TINYXML2_NAME} ALL DEPENDS ${TBAG_DEP_TINYXML2_ARCHIVES})

add_custom_command (
        OUTPUT ${TBAG_DEP_TINYXML2_ARCHIVES}
        COMMAND make > "${TBAG_DEP_TINYXML2_LOG_PREFIX}" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/tinyxml2/Makefile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/tinyxml2/")

