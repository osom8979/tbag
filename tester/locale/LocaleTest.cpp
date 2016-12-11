/**
 * @file   LocaleTest.cpp
 * @brief  Locale tester.
 * @author zer0
 * @date   2016-12-05
 */

#include <gtest/gtest.h>
#include <libtbag/locale/Locale.hpp>
#include <iostream>

using namespace libtbag;
using namespace libtbag::locale;

TEST(LocaleTest, Information)
{
    std::cout << "Default locale name: " << libtbag::locale::icu::getDefaultLocaleName() << std::endl;

    //std::cout << "List of available locale:\n";
    //for (auto & name : getAvailableLocale()) {
    //    std::cout << " - " << name << std::endl;
    //}
}

