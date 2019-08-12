/**
 * @file   FourccTest.cpp
 * @brief  Fourcc class tester.
 * @author zer0
 * @date   2019-08-12
 */

#include <gtest/gtest.h>
#include <libtbag/media/Fourcc.hpp>

using namespace libtbag;
using namespace libtbag::media;

TEST(FourccTest, GetCanonicalFourcc)
{
    ASSERT_EQ(FOURCC_I420, getCanonicalFourcc(FOURCC_IYUV));
}

TEST(FourccTest, GetFourccName)
{
    ASSERT_STREQ("IYUV", getFourccName(FOURCC_IYUV).c_str());
}

