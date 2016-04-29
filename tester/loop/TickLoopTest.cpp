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
    int const TEST_COUNT  = 10;
    int count = 0;

    TickLoop<> loop([&](){
        if (count == TEST_COUNT) {
            loop.exit();
            loop.setResultCode(RESULT_CODE);
        } else {
            ++count;
        }
    }, std::chrono::milliseconds(100));

    ASSERT_EQ(loop.run(), RESULT_CODE);
    ASSERT_EQ(count, TEST_COUNT);
}

