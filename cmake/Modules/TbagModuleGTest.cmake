#/// @file   TbagModuleGTest.cmake
#/// @brief  Google-test cmake module.
#/// @author zer0
#/// @date   2016-05-26

#/// Assign google-test libraries.
#///
#/// @remarsk
#///  Recommended apply to the test project.
macro (tbag_project_assign_gtest)
    find_package (GTest)
    if (GTEST_FOUND)
        list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_GTEST)
        list (APPEND TBAG_PROJECT_LDFLAGS      ${GTEST_BOTH_LIBRARIES} -lpthread)
    endif ()
endmacro ()

