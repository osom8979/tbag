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

TEST(FontTest, CharToConsole)
{
    std::string const OUTPUT = drawTrueType(getProggyClean(), 'T');
    ASSERT_FALSE(OUTPUT.empty());
}

TEST(FontTest, TextToConsole)
{
    std::string const OUTPUT = drawTrueType(getProggyClean(), "TBAG");
    ASSERT_FALSE(OUTPUT.empty());

    std::cout << "---------------------------\n";
    std::cout << OUTPUT;
    std::cout << "---------------------------\n";
}

