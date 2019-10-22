/**
 * @file   PreferencesTest.cpp
 * @brief  Preferences class tester.
 * @author zer0
 * @date   2019-10-22
 */

#include <gtest/gtest.h>
#include <libtbag/res/Preferences.hpp>

using namespace libtbag;
using namespace libtbag::res;

TEST(PreferencesTest, Default)
{
    Preferences object;
    ASSERT_TRUE(true);
}

