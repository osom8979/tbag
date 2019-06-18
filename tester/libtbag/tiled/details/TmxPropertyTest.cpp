/**
 * @file   TmxPropertyTest.cpp
 * @brief  TmxProperty class tester.
 * @author zer0
 * @date   2019-06-10
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxProperty.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxPropertyTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxProperty, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxProperty, obj2)
}

TEST(TmxPropertyTest, ToInt)
{
    TmxProperty value("INT", 10);
    ASSERT_EQ("INT", value.name);
    ASSERT_EQ(TmxProperty::Type::INT, value.type);
    ASSERT_EQ(10, value.toInt());
}

TEST(TmxPropertyTest, ToFloat)
{
    TmxProperty value("FLOAT", 2.5f);
    ASSERT_EQ("FLOAT", value.name);
    ASSERT_EQ(TmxProperty::Type::FLOAT, value.type);
    ASSERT_EQ(2.5f, value.toFloat());
}

TEST(TmxPropertyTest, ToBool)
{
    TmxProperty value1("B1", true);
    ASSERT_EQ("B1", value1.name);
    ASSERT_EQ(TmxProperty::Type::BOOL, value1.type);
    ASSERT_EQ(true, value1.toBool());

    TmxProperty value2("B2", "true", TmxProperty::Type::BOOL);
    ASSERT_EQ("B2", value2.name);
    ASSERT_EQ(TmxProperty::Type::BOOL, value2.type);
    ASSERT_EQ(true, value2.toBool());
}

TEST(TmxPropertyTest, ToColor)
{
    TmxProperty value1("C1", libtbag::graphic::ORANGE_COLOR);
    ASSERT_EQ("C1", value1.name);
    ASSERT_EQ(TmxProperty::Type::COLOR, value1.type);
    auto color1 = value1.toColor();
    ASSERT_EQ(libtbag::graphic::ORANGE_COLOR.r, color1.r);
    ASSERT_EQ(libtbag::graphic::ORANGE_COLOR.g, color1.g);
    ASSERT_EQ(libtbag::graphic::ORANGE_COLOR.b, color1.b);
    ASSERT_EQ(libtbag::graphic::ORANGE_COLOR.a, color1.a);

    TmxProperty value2("C2", "#AABBCCDD", TmxProperty::Type::COLOR);
    ASSERT_EQ("C2", value2.name);
    ASSERT_EQ(TmxProperty::Type::COLOR, value2.type);
    auto color2 = value2.toColor();
    ASSERT_EQ(0xBB, color2.r);
    ASSERT_EQ(0xCC, color2.g);
    ASSERT_EQ(0xDD, color2.b);
    ASSERT_EQ(0xAA, color2.a);
}

TEST(TmxPropertyTest, ToFile)
{
    auto const WORK_DIR = TmxProperty::Path::getWorkDir();
    TmxProperty value("F1", WORK_DIR);
    ASSERT_EQ("F1", value.name);
    ASSERT_EQ(TmxProperty::Type::FILE, value.type);
    ASSERT_EQ(WORK_DIR, value.toFile());
}

