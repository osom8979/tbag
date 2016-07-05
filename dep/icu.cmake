#/// @file   icu.cmake
#/// @brief  libtbag-icu library build project.
#/// @author zer0
#/// @date   2016-07-05
#///
#/// @remarks
#///  The following are set after configuration is done:
#///   - ${TBAG_DEP_ICU_NAME}
#///   - ${TBAG_DEP_ICU_INCLUDE_DIRS}
#///   - ${TBAG_DEP_ICU_LIBRARIES}

set (TBAG_DEP_ICU_NAME          "libtbag-icu")
set (TBAG_DEP_ICU_INCLUDE_DIRS  "${PROJECT_SOURCE_DIR}/dep/icu/common"
                                "${PROJECT_SOURCE_DIR}/dep/icu/i18n")
set (TBAG_DEP_ICU_LIBRARIES     "${PROJECT_SOURCE_DIR}/dep/icu/lib/libicudata.a"
                                "${PROJECT_SOURCE_DIR}/dep/icu/lib/libicui18n.a"
                                "${PROJECT_SOURCE_DIR}/dep/icu/lib/libicuuc.a")
set (TBAG_DEP_ICU_LOG_PREFIX    "${PROJECT_BINARY_DIR}/CMakeFiles/${TBAG_DEP_ICU_NAME}.dir/build.log")

## icu4c custom target.
add_custom_target (${TBAG_DEP_ICU_NAME} ALL DEPENDS ${TBAG_DEP_ICU_LIBRARIES})

add_custom_command (
        OUTPUT "${PROJECT_SOURCE_DIR}/dep/icu/Makefile"
        COMMAND ./configure CFLAGS=-fPIC CXXFLAGS=-fPIC --enable-release --enable-static --disable-shared --disable-tests --disable-samples > "${TBAG_DEP_ICU_LOG_PREFIX}.1" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/icu/configure"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/icu/")

add_custom_command (
        OUTPUT ${TBAG_DEP_ICU_LIBRARIES}
        COMMAND make V=1 > "${TBAG_DEP_ICU_LOG_PREFIX}.2" 2>&1
        DEPENDS "${PROJECT_SOURCE_DIR}/dep/icu/Makefile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/dep/icu/")

