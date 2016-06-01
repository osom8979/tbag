#/// @file   TbagProjectUV.cmake
#/// @brief  libuv project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT GTEST_FOUND)
    find_package (GTest)
endif ()

if (GTEST_FOUND)
    list (APPEND TBAG_PROJECT_DEFINITIONS  -DUSE_GTEST)
    list (APPEND TBAG_PROJECT_LDFLAGS      ${GTEST_BOTH_LIBRARIES} -lpthread)
endif ()

