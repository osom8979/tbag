#/// @file   tinyxml2.cmake
#/// @brief  libtbag-tinyxml2 library build project.
#/// @author zer0
#/// @date   2016-07-04
#///
#/// @remarks
#///  The following are set after configuration is done:
#///   - ${TBAG_DEP_TINYXML2_NAME}
#///   - ${TBAG_DEP_TINYXML2_INCLUDE_DIRS}
#///   - ${TBAG_DEP_TINYXML2_LIBRARIES}

set (TBAG_DEP_TINYXML2_NAME          "libtbag-tinyxml2")
set (TBAG_DEP_TINYXML2_INCLUDE_DIRS  "${PROJECT_SOURCE_DIR}/dep/tinyxml2")
set (TBAG_DEP_TINYXML2_LIBRARIES     "${PROJECT_SOURCE_DIR}/dep/tinyxml2/libtinyxml2.a")

## TinyXML2 custom target.
add_custom_target (${TBAG_DEP_TINYXML2_NAME} ALL DEPENDS ${TBAG_DEP_TINYXML2_LIBRARIES})

add_custom_command (
        OUTPUT "${PROJECT_SOURCE_DIR}/dep/tinyxml2/Makefile"
        COMMAND ${CMAKE_COMMAND} -DCMAKE_MACOSX_RPATH=1 .
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/tinyxml2/CMakeLists.txt"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/tinyxml2/")

add_custom_command (
        OUTPUT ${TBAG_DEP_TINYXML2_LIBRARIES}
        COMMAND make
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/tinyxml2/Makefile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/tinyxml2/")

