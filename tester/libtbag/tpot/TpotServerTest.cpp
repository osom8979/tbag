/**
 * @file   TpotServerTest.cpp
 * @brief  TpotServer class tester.
 * @author zer0
 * @date   2017-09-06
 */

#include <gtest/gtest.h>
#include <libtbag/tpot/TpotServer.hpp>

using namespace libtbag;
using namespace libtbag::tpot;

TEST(TpotServerTest, Default)
{
    TpotServer object;
    ASSERT_TRUE(true);
}

