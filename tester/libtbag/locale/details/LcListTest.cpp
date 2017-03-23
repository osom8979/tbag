/**
 * @file   LcListTest.cpp
 * @brief  LcList class tester.
 * @author zer0
 * @date   2017-03-15
 */

#include <gtest/gtest.h>
#include <libtbag/locale/details/LcList.hpp>

#include <algorithm>

using namespace libtbag;
using namespace libtbag::locale;
using namespace libtbag::locale::details;

TEST(LcListTest, Default)
{
    std::vector<std::string> const LOCALE_LIST = getAllLocale();
    ASSERT_EQ(LOCALE_LIST.size(), getAllLocaleCount());
    ASSERT_NE(LOCALE_LIST.end(), std::find(LOCALE_LIST.begin(), LOCALE_LIST.end(), std::string("ko_KR.UTF-8")));
}


