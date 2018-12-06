/**
 * @file   UxAsyncTest.cpp
 * @brief  UxAsync class tester.
 * @author zer0
 * @date   2018-12-06
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxAsync.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxAsyncTest, Default)
{
    int async_counter = 0;
    int close_counter = 0;

    UxAsync async;
    UxLoop loop;
    ASSERT_TRUE(loop.empty());

    ASSERT_EQ(Err::E_SUCCESS, async.init(loop));
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(1, loop.size());

    ASSERT_EQ(Err::E_SUCCESS, async.init(loop));
    ASSERT_FALSE(loop.empty());
    ASSERT_EQ(2, loop.size());

    async.setOnAsync([&](){
        ++async_counter;
        async.close();
    });
    async.setOnClose([&](){
        ++close_counter;
    });

    ASSERT_EQ(Err::E_SUCCESS, async.send());
    ASSERT_EQ(Err::E_SUCCESS, loop.run());

    ASSERT_EQ(1, async_counter);
    ASSERT_EQ(1, close_counter);
}

