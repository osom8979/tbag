/**
 * @file   TickLoopTest.cpp
 * @brief  TickLoop class tester.
 * @author zer0
 * @date   2016-04-29
 */

#include <gtest/gtest.h>
#include <libtbag/loop/TickLoop.hpp>

using namespace libtbag;
using namespace libtbag::loop;

TEST(TickLoopTest, Default)
{
    int const RESULT_CODE = 50;
    int const TEST_COUNT  = 2;
    int count = 0;

    TickLoop<> loop;
    loop.setCallback([&](){
        if (count == TEST_COUNT) {
            loop.exit();
            loop.setResultCode(RESULT_CODE);
        } else {
            ++count;
        }
    });
    loop.setDuration(std::chrono::milliseconds(1));

    ASSERT_EQ(RESULT_CODE, loop.run());
    ASSERT_EQ( TEST_COUNT, count);
}

