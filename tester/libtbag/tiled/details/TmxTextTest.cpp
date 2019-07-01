/**
 * @file   TmxTextTest.cpp
 * @brief  TmxText class tester.
 * @author zer0
 * @date   2019-06-23
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxText.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxTextTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxText, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxText, obj2)
}

TEST(TmxTextTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<text fontfamily="gothic"
      pixelsize="32"
      wrap="1"
      color="#99887766"
      bold="1"
      italic="1"
      underline="1"
      strikeout="1"
      kerning="1"
      halign="justify"
      valign="bottom">TEST_DATA</text>
)";

    TmxText text;
    ASSERT_EQ(E_SUCCESS, text.read(TEST_XML));
    ASSERT_STREQ("gothic", text.fontfamily.c_str());
    ASSERT_EQ(32, text.pixelsize);
    ASSERT_EQ(1, text.wrap);
    ASSERT_EQ(0x99, text.color.a);
    ASSERT_EQ(0x88, text.color.r);
    ASSERT_EQ(0x77, text.color.g);
    ASSERT_EQ(0x66, text.color.b);
    ASSERT_EQ(1, text.bold);
    ASSERT_EQ(1, text.italic);
    ASSERT_EQ(1, text.underline);
    ASSERT_EQ(1, text.strikeout);
    ASSERT_EQ(1, text.kerning);
    ASSERT_EQ(TmxText::HorizontalAlignment::JUSTIFY, text.halign);
    ASSERT_EQ(TmxText::VerticalAlignment::BOTTOM, text.valign);
    ASSERT_STREQ("TEST_DATA", text.data.c_str());

    std::string xml;
    ASSERT_EQ(E_SUCCESS, text.write(xml));

    TmxText text2;
    ASSERT_EQ(E_SUCCESS, text2.read(xml));
    ASSERT_STREQ("gothic", text2.fontfamily.c_str());
    ASSERT_EQ(32, text2.pixelsize);
    ASSERT_EQ(1, text2.wrap);
    ASSERT_EQ(0x99, text2.color.a);
    ASSERT_EQ(0x88, text2.color.r);
    ASSERT_EQ(0x77, text2.color.g);
    ASSERT_EQ(0x66, text2.color.b);
    ASSERT_EQ(1, text2.bold);
    ASSERT_EQ(1, text2.italic);
    ASSERT_EQ(1, text2.underline);
    ASSERT_EQ(1, text2.strikeout);
    ASSERT_EQ(1, text2.kerning);
    ASSERT_EQ(TmxText::HorizontalAlignment::JUSTIFY, text2.halign);
    ASSERT_EQ(TmxText::VerticalAlignment::BOTTOM, text2.valign);
    ASSERT_STREQ("TEST_DATA", text2.data.c_str());
}

