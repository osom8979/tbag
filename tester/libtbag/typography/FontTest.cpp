/**
 * @file   FontTest.cpp
 * @brief  Font class tester.
 * @author zer0
 * @date   2017-11-16
 */

#include <gtest/gtest.h>
#include <libtbag/util/TestUtils.hpp>
#include <libtbag/typography/Font.hpp>
#include <libtbag/typography/font/ProggyClean.hpp>
#include <libtbag/crypto/Md5.hpp>

using namespace libtbag;
using namespace libtbag::typography;
using namespace libtbag::typography::font;

TEST(FontTest, ProggyClean)
{
    ASSERT_EQ(getProggyCleanMd5(), crypto::getMd5(getProggyClean()));
}

TEST(FontTest, CharToConsole)
{
    std::string const OUTPUT = getAsciiImage(getProggyClean(), 'T');
    ASSERT_FALSE(OUTPUT.empty());
}

TEST(FontTest, TextToConsole)
{
    std::string const OUTPUT = getAsciiImage(getProggyClean(), "TBAG");
    ASSERT_FALSE(OUTPUT.empty());

    std::cout << "---------------------------\n";
    std::cout << OUTPUT;
    std::cout << "---------------------------\n";
}

TEST(FontTest, ExportImage)
{
    tttDir_Automatic();
    auto const IMAGE_PATH = tttDir_Get() / "font.png";
    int  const LINE_HEIGHT = 32;

    TrueType tt;
    ASSERT_EQ(Err::E_SUCCESS, tt.loadFromMemory(getProggyClean()));

    std::string const DRAW_TEXT = "A";
    libtbag::graphic::ImageGray gray;
    ASSERT_EQ(Err::E_SUCCESS, tt.drawAscii(DRAW_TEXT, gray, LINE_HEIGHT));
    ASSERT_EQ(Err::E_SUCCESS, libtbag::graphic::saveImage(IMAGE_PATH, gray));
}

