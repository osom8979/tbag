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
    TmxProperty value1("C1", libtbag::graphic::ORANGE_FLAT_COLOR);
    ASSERT_EQ("C1", value1.name);
    ASSERT_EQ(TmxProperty::Type::COLOR, value1.type);
    auto color1 = value1.toColor();
    ASSERT_EQ(libtbag::graphic::ORANGE_FLAT_COLOR.r, color1.r);
    ASSERT_EQ(libtbag::graphic::ORANGE_FLAT_COLOR.g, color1.g);
    ASSERT_EQ(libtbag::graphic::ORANGE_FLAT_COLOR.b, color1.b);
    ASSERT_EQ(libtbag::graphic::ORANGE_FLAT_COLOR.a, color1.a);

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

TEST(TmxPropertyTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(<property name="test" value="#AABBCCDD" type="color" />)";

    TmxProperty prop;
    ASSERT_EQ(E_SUCCESS, prop.read(TEST_XML));
    ASSERT_STREQ("test", prop.name.c_str());
    ASSERT_STREQ("#AABBCCDD", prop.value.c_str());
    ASSERT_EQ(TmxProperty::Type::COLOR, prop.type);
    auto const COLOR = prop.toColor();
    ASSERT_EQ(0xBB, COLOR.r);
    ASSERT_EQ(0xCC, COLOR.g);
    ASSERT_EQ(0xDD, COLOR.b);
    ASSERT_EQ(0xAA, COLOR.a);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, prop.write(xml));

    TmxProperty prop2;
    ASSERT_EQ(E_SUCCESS, prop2.read(xml));
    ASSERT_STREQ("test", prop2.name.c_str());
    ASSERT_STREQ("#AABBCCDD", prop2.value.c_str());
    ASSERT_EQ(TmxProperty::Type::COLOR, prop2.type);
    auto const COLOR2 = prop2.toColor();
    ASSERT_EQ(0xBB, COLOR2.r);
    ASSERT_EQ(0xCC, COLOR2.g);
    ASSERT_EQ(0xDD, COLOR2.b);
    ASSERT_EQ(0xAA, COLOR2.a);
}

TEST(TmxPropertyTest, Validate)
{
    TmxProperty prop1("I", 20);
    ASSERT_EQ(TmxProperty::Type::INT, prop1.type);
    ASSERT_FALSE(prop1.validate("0.15"));
    ASSERT_TRUE(prop1.validate("0.16"));
    ASSERT_TRUE(prop1.validate("0.17"));

    TmxProperty prop2("C", libtbag::graphic::ORANGE_FLAT_COLOR);
    ASSERT_EQ(TmxProperty::Type::COLOR, prop2.type);
    ASSERT_FALSE(prop2.validate(0, 15));
    ASSERT_FALSE(prop2.validate(0, 16));
    ASSERT_TRUE(prop2.validate(0, 17));
}

