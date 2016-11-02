/**
 * @file   main.cpp
 * @brief  Tester project entry-point.
 * @author zer0
 * @date   2016-11-01
 */

#include <gtest/gtest.h>
#include <libtbag/libtbag.h>

int main(int argc, char **argv)
{
    tbInitialize();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

