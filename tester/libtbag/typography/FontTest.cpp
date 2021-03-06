/**
 * @file   FontTest.cpp
 * @brief  Font class tester.
 * @author zer0
 * @date   2017-11-16
 */

#include <gtest/gtest.h>
#include <libtbag/util/TestUtils.hpp>
#include <libtbag/typography/Font.hpp>
#include <libtbag/typography/font/Ngc.hpp>
#include <libtbag/crypto/Md5.hpp>

using namespace libtbag;
using namespace libtbag::typography;
using namespace libtbag::typography::font;

TEST(FontTest, GetAsciiImage)
{
    std::string const NORMAL = getAsciiImage(getNgcNormal(), 'T');
    ASSERT_FALSE(NORMAL.empty());

    std::string const BOLD = getAsciiImage(getNgcBold(), 'T');
    ASSERT_FALSE(BOLD.empty());
}

TEST(FontTest, TextToConsole)
{
    std::string const OUTPUT = getAsciiImage(getNgcBold(), "TBAG");
    ASSERT_FALSE(OUTPUT.empty());

    std::cout << "------------------------\n";
    std::cout << OUTPUT;
    std::cout << "------------------------\n";
}

//TEST(FontTest, ExportImage)
//{
//    tttDir_Automatic();
//    auto const IMAGE_PATH = tttDir_Get() / "font.png";
//    int  const LINE_HEIGHT = 32;
//
//    TrueType tt;
//    ASSERT_EQ(E_SUCCESS, tt.loadFromMemory(getNgcNormal()));
//
//    std::string const DRAW_TEXT = "A";
//    libtbag::graphic::ImageGray gray;
//    ASSERT_EQ(E_SUCCESS, tt.drawAscii(DRAW_TEXT, gray, LINE_HEIGHT));
//    ASSERT_EQ(E_SUCCESS, libtbag::graphic::writeImage(IMAGE_PATH, gray));
//}

