#/// @file   TbagProjectGTest.cmake
#/// @brief  Google-gtest project module.
#/// @author zer0
#/// @date   2016-06-01

if (NOT GTEST_FOUND)
    message (WARNING "Not found Google-gtest.")
endif ()

list (APPEND TBAG_PROJECT_LDFLAGS ${GTEST_BOTH_LIBRARIES} -lpthread)

