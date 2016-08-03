#/// @file   gtest.cmake
#/// @brief  libtbag-gtest library build project.
#/// @author zer0
#/// @date   2016-07-05
#///
#/// @remarks
#///  The following are set after configuration is done:
#///   - ${TBAG_DEP_GTEST_NAME}
#///   - ${TBAG_DEP_GTEST_LOG_PREFIX}
#///   - ${TBAG_DEP_GTEST_INCLUDE_DIRS}
#///   - ${TBAG_DEP_GTEST_ARCHIVES}
#///   - ${TBAG_DEP_GTEST_DEPENDENCIES}
#///   - ${TBAG_DEP_GTEST_LIBRARIES}: Archives + Dependencies

if (DEFINED TBAG_DEP_GTEST_NAME)
    return ()
endif ()

set (TBAG_DEP_GTEST_NAME          "libtbag-gtest")
set (TBAG_DEP_GTEST_LOG_PREFIX    "${PROJECT_BINARY_DIR}/CMakeFiles/${TBAG_DEP_GTEST_NAME}.dir/build.log")
set (TBAG_DEP_GTEST_INCLUDE_DIRS  "${PROJECT_SOURCE_DIR}/dep/gtest/include")
set (TBAG_DEP_GTEST_ARCHIVES      "${PROJECT_SOURCE_DIR}/dep/gtest/make/gtest.a"
                                  "${PROJECT_SOURCE_DIR}/dep/gtest/make/gtest_main.a")
set (TBAG_DEP_GTEST_DEPENDENCIES  -lpthread)
set (TBAG_DEP_GTEST_LIBRARIES     ${TBAG_DEP_GTEST_ARCHIVES}
                                  ${TBAG_DEP_GTEST_DEPENDENCIES})

## Google-gtest custom target.
add_custom_target (${TBAG_DEP_GTEST_NAME} ALL DEPENDS ${TBAG_DEP_GTEST_ARCHIVES})

add_custom_command (
        OUTPUT ${TBAG_DEP_GTEST_ARCHIVES}
        COMMAND make CXXFLAGS=-fPIC gtest.a gtest_main.a > "${TBAG_DEP_GTEST_LOG_PREFIX}" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/gtest/make/Makefile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/gtest/make/")

