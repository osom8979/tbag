/**
 * @file   LoopTest.cpp
 * @brief  Loop class tester.
 * @author zer0
 * @date   2016-12-31
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Idle.hpp>

using namespace libtbag;
using namespace libtbag::uvpp;

TEST(LoopTest, Default)
{
    Loop loop;

    ASSERT_TRUE(loop.empty());
    ASSERT_EQ(0, loop.size());

    auto s1 = loop.newHandle<Idle>(loop);
    auto s2 = loop.newHandle<Idle>(loop);

    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    loop.forceClear();

    ASSERT_TRUE(loop.empty());
    ASSERT_EQ(0, loop.size());
}

