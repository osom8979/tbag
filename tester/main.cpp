/**
 * @file   main.cpp
 * @brief  Tester project entry-point.
 * @author zer0
 * @date   2016-11-01
 */

#include <gtest/gtest.h>
#include <libtbag/loop/UvEventLoop.hpp>

static void __tester_initialize__()
{
    libtbag::loop::UvEventLoop loop;
    loop.runDefault();
}

int main(int argc, char **argv)
{
    __tester_initialize__();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

