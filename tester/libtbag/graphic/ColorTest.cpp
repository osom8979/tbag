/**
 * @file   ColorTest.cpp
 * @brief  Color class tester.
 * @author zer0
 * @date   2016-04-21
 */

#include <gtest/gtest.h>
#include <libtbag/graphic/Color.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(ColorTest, Constructor)
{
    Color c1;
    Color c2 = {0, 1, 2};
    Color c3 = c2;

    c1 = Color{8, 9, 10};

    ASSERT_EQ( 8, c1.r);
    ASSERT_EQ( 9, c1.g);
    ASSERT_EQ(10, c1.b);
    ASSERT_EQ(c2, c3);
}

TEST(ColorTest, FromString)
{
    Color c;
    ASSERT_EQ(E_SUCCESS, c.fromString("#8C6BF0FF"));
    ASSERT_EQ(0x8C, c.r);
    ASSERT_EQ(0x6B, c.g);
    ASSERT_EQ(0xF0, c.b);
    ASSERT_EQ(0xFF, c.a);
}

TEST(ColorTest, String)
{
    ASSERT_EQ(BLACK_COLOR, Color("#000"));
    ASSERT_EQ(BLACK_COLOR, Color("#000000"));
    ASSERT_EQ(BLACK_COLOR, Color("000"));
    ASSERT_EQ(BLACK_COLOR, Color("000000"));

    ASSERT_EQ(WHITE_COLOR, Color("#FFF"));
    ASSERT_EQ(WHITE_COLOR, Color("#FFFFFF"));
    ASSERT_EQ(WHITE_COLOR, Color("FFF"));
    ASSERT_EQ(WHITE_COLOR, Color("FFFFFF"));

    ASSERT_EQ(RED_COLOR, Color("#F00"));
    ASSERT_EQ(RED_COLOR, Color("#FF0000"));
    ASSERT_EQ(RED_COLOR, Color("F00"));
    ASSERT_EQ(RED_COLOR, Color("FF0000"));

    ASSERT_EQ(LIME_COLOR, Color("#0F0"));
    ASSERT_EQ(LIME_COLOR, Color("#00FF00"));
    ASSERT_EQ(LIME_COLOR, Color("0F0"));
    ASSERT_EQ(LIME_COLOR, Color("00FF00"));

    ASSERT_EQ(BLUE_COLOR, Color("#00F"));
    ASSERT_EQ(BLUE_COLOR, Color("#0000FF"));
    ASSERT_EQ(BLUE_COLOR, Color("00F"));
    ASSERT_EQ(BLUE_COLOR, Color("0000FF"));

    ASSERT_EQ(Color(17, 34, 51), Color("#123"));
    ASSERT_EQ(Color(170, 153, 221), Color("#AA99DD"));

    ASSERT_EQ(Rgb32(17, 34, 51, 68), Rgb32("#1234"));
    ASSERT_EQ(Rgb32(170, 153, 221, 85), Rgb32("#AA99DD55"));

    ASSERT_STREQ(Color(17, 34, 51).toString().c_str(), "#112233FF");
    ASSERT_STREQ(Color(170, 153, 221).toString().c_str(), "#AA99DDFF");

    ASSERT_STREQ(Rgb32(17, 34, 51, 68).toString().c_str(), "#11223344");
    ASSERT_STREQ(Rgb32(170, 153, 221, 85).toString().c_str(), "#AA99DD55");

    Rgb32 color1;
    color1.fromArgbString("#4123");
    ASSERT_EQ(Rgb32(17, 34, 51, 68), color1);

    Rgb32 color2;
    color2.fromArgbString("#55AA99DD");
    ASSERT_EQ(Rgb32(170, 153, 221, 85), color2);

    ASSERT_STREQ(Rgb32(17, 34, 51, 68).toArgbString().c_str(), "#44112233");
    ASSERT_STREQ(Rgb32(170, 153, 221, 85).toArgbString().c_str(), "#55AA99DD");
}

