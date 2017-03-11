/**
 * @file   LcListTest.cpp
 * @brief  LcList tester.
 * @author zer0
 * @date   2017-03-11
 */

#include <gtest/gtest.h>
#include <libtbag/locale/LcList.hpp>

#include <iostream>
#include <algorithm>

using namespace libtbag;
using namespace libtbag::locale;

TEST(LcListTest, Default)
{
    auto const LOCALE_LIST = getAllLocale();
    ASSERT_EQ(LOCALE_LIST.size(), getAllLocaleCount());
    ASSERT_NE(LOCALE_LIST.end(), std::find(LOCALE_LIST.begin(), LOCALE_LIST.end(), std::string("ko_KR.UTF-8")));
}

