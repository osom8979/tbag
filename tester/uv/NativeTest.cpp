/**
 * @file   NativeTest.cpp
 * @brief  Native class tester.
 * @author zer0
 * @date   2016-12-31
 */

#include <gtest/gtest.h>
#include <libtbag/uv/Native.hpp>

using namespace libtbag;
using namespace libtbag::uv;

TEST(NativeTest, TypeSupport)
{
    UvPodType const START = static_cast<UvPodType>(UvType::__START_NUMBER__);
    UvPodType const SIZE  = static_cast<UvPodType>(UvType::__SIZE__);

    ASSERT_EQ(0, START + 1);

    int user_data = 100;
    for (UvPodType index = 0; index < SIZE; ++index) {
        Native native(index);

        ASSERT_NE(nullptr, native.get());
        ASSERT_TRUE(static_cast<bool>(native));
        ASSERT_LT(START, static_cast<UvPodType>(native.getType()));
        ASSERT_GT(SIZE, static_cast<UvPodType>(native.getType()));

        native.setUserData(&user_data);
        ASSERT_EQ(&user_data, native.getUserData());

        ASSERT_TRUE(native.isHandle() || native.isRequest() || native.isEtc() || native.isStream());
    }
}

