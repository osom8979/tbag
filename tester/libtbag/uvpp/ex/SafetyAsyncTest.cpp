/**
 * @file   SafetyAsyncTest.cpp
 * @brief  SafetyAsync class tester.
 * @author zer0
 * @date   2017-05-01
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>

using namespace libtbag;
using namespace libtbag::uvpp;
using namespace libtbag::uvpp::ex;

TEST(SafetyAsyncTest, Default)
{
    SafetyAsync object;
    ASSERT_TRUE(true);
}

