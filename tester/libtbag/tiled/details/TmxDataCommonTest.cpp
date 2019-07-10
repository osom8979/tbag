/**
 * @file   TmxDataCommonTest.cpp
 * @brief  TmxDataCommon class tester.
 * @author zer0
 * @date   2019-07-10
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxDataCommon.hpp>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxDataCommonTest, Default)
{
    TmxDataCommon object;
    ASSERT_TRUE(true);
}

