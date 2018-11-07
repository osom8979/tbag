/**
 * @file   UxPollTest.cpp
 * @brief  UxPoll class tester.
 * @author zer0
 * @date   2018-11-07
 */

#include <gtest/gtest.h>
#include <libtbag/uvxx/UxPoll.hpp>

using namespace libtbag;
using namespace libtbag::uvxx;

TEST(UxPollTest, Default)
{
    UxPoll object;
    ASSERT_TRUE(true);
}

