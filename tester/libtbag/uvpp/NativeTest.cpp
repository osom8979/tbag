/**
 * @file   NativeTest.cpp
 * @brief  Native class tester.
 * @author zer0
 * @date   2016-12-31
 */

#include <gtest/gtest.h>
#include <libtbag/uvpp/Native.hpp>

using namespace libtbag;
using namespace libtbag::uvpp;

TEST(NativeTest, TypeSupport)
{
    int const START = static_cast<int>(utype::__START_NUMBER__);
    int const SIZE  = static_cast<int>(utype::__SIZE__);

    ASSERT_EQ(0, START + 1);

    int user_data = 100;
    for (int index = 0; index < SIZE; ++index) {
        Native native(index);

        ASSERT_NE(nullptr, native.get());
        ASSERT_TRUE(static_cast<bool>(native));
        ASSERT_LT(START, static_cast<int>(native.getType()));
        ASSERT_GT(SIZE, static_cast<int>(native.getType()));

        native.setUserData(&user_data);
        ASSERT_EQ(&user_data, native.getUserData());

        ASSERT_TRUE(native.isHandle() || native.isRequest() || native.isEtc() || native.isStream());
    }
}

