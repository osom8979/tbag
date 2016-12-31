/**
 * @file   LoopTest.cpp
 * @brief  Loop class tester.
 * @author zer0
 * @date   2016-12-31
 */

#include <gtest/gtest.h>
#include <libtbag/uv/Loop.hpp>

using namespace libtbag;
using namespace libtbag::uv;

TEST(LoopTest, Default)
{
    Loop loop;

    ASSERT_TRUE(loop.empty());
    ASSERT_EQ(0, loop.size());

    auto w1 = loop.createChildHandle(UvHandleType::IDLE);
    auto w2 = loop.createChildHandle(UvHandleType::IDLE);

    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    loop.clear();

    ASSERT_TRUE(loop.empty());
    ASSERT_EQ(0, loop.size());
}

