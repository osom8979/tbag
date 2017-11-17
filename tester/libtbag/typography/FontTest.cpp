/**
 * @file   FontTest.cpp
 * @brief  Font class tester.
 * @author zer0
 * @date   2017-11-16
 */

#include <gtest/gtest.h>
#include <libtbag/typography/Font.hpp>
#include <libtbag/encrypt/Md5.hpp>

using namespace libtbag;
using namespace libtbag::typography;

TEST(FontTest, ProggyClean)
{
    ASSERT_EQ(getProggyCleanMd5(), encrypt::getMd5(getProggyClean()));
}

