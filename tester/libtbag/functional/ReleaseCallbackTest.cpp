/**
 * @file   ReleaseCallbackTest.cpp
 * @brief  ReleaseCallback class tester.
 * @author zer0
 * @date   2018-06-13
 */

#include <gtest/gtest.h>
#include <libtbag/functional/ReleaseCallback.hpp>

using namespace libtbag;
using namespace libtbag::functional;

TEST(ReleaseCallbackTest, Default)
{
    bool flag = false;
    BRACE("RUN") {
        ReleaseCallback object([&](){
            flag = true;
        });
    }
    ASSERT_TRUE(flag);
}

TEST(ReleaseCallbackTest, Cancel)
{
    bool flag = false;
    BRACE("RUN") {
        ReleaseCallback object([&](){
            flag = true;
        });
        object.cancel();
    }
    ASSERT_FALSE(flag);
}

