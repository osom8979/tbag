#/// @file   TbagModuleGTest.cmake
#/// @brief  Google-test tbag module.
#/// @author zer0
#/// @date   2016-05-26

#/// Assign google-test libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_module_gtest__assign)
    if (NOT GTEST_FOUND)
        find_package (GTest)
    endif ()

    if (GTEST_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_GTEST)
        list (APPEND TBAG_PROJECT_LDFLAGS      ${GTEST_BOTH_LIBRARIES} -lpthread)
    endif ()
endmacro ()

