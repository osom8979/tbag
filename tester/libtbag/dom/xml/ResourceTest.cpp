/**
 * @file   ResourceTest.cpp
 * @brief  Resource class tester.
 * @author zer0
 * @date   2016-04-12
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/dom/xml/Resource.hpp>

using namespace libtbag;
using namespace libtbag::dom;
using namespace libtbag::dom::xml;

TEST(ResourceTest, Utf8)
{
    unsigned char hangul [] = {
            0xea, 0xb0, 0x80, 0xeb, 0x82, 0x98, 0xeb, 0x8b, 0xa4, 0xeb
          , 0x9d, 0xbc, 0xeb, 0xa7, 0x88, 0xeb, 0xb0, 0x94, 0xec, 0x82
          , 0xac, 0xec, 0x95, 0x84, 0xec, 0x9e, 0x90, 0xec, 0xb0, 0xa8
          , 0xec, 0xb9, 0xb4, 0xed, 0x83, 0x80, 0xed, 0x8c, 0x8c, 0xed
          , 0x95, 0x98, '\0' };

    // TEST STRING: "가나다라마바사아자차카타파하"
    std::string const HANGUL_UTF8 =
            "\xea\xb0\x80\xeb\x82\x98\xeb\x8b\xa4\xeb\x9d\xbc\xeb\xa7\x88\xeb"
            "\xb0\x94\xec\x82\xac\xec\x95\x84\xec\x9e\x90\xec\xb0\xa8\xec\xb9"
            "\xb4\xed\x83\x80\xed\x8c\x8c\xed\x95\x98"; // Don't use u8 literal.

    char const * const TEST_NAME = "hangul";
    char const * const TEST_TAG = "utf8";

    tttDir(true, true);
    auto const PATH = tttDir_Get() / "utf8.xml";

    Resource res;
    res.set(TEST_NAME, (char*)hangul);
    res.setTag(TEST_TAG);
    ASSERT_TRUE(res.save(PATH.toString()));

    res.clear();
    ASSERT_TRUE(res.readFile(PATH.toString(), TEST_TAG));
    ASSERT_EQ(HANGUL_UTF8, res.getString(TEST_NAME));
}

class ResourceFixtureTest : public ::testing::Test
{
public:
    std::string xml;
    std::string tag;

    std::string attribute1;
    std::string attribute2;
    std::string attribute3;
    std::string attribute4;
    std::string attribute5;

    std::string value1;
    std::string value2;
    std::string value3;
    std::string value4;
    std::string value5;

    std::string value1_2;
    std::string value2_2;
    int         value3_2;
    int         value4_2;
    float       value5_2;

    Resource res;

public:
    ResourceFixtureTest() = default;
    ~ResourceFixtureTest() = default;

public:
    virtual void SetUp() override
    {
        attribute1 = "title1";
        attribute2 = "title2";
        attribute3 = "number";
        attribute4 = "number";
        attribute5 = "floating";

        value1 = "test&amp;title1";
        value2 = "test&amp;title2";
        value3 = "100";
        value4 = "50";
        value5 = "55.555555";

        value1_2 = "test&title1";
        value2_2 = "test&title2";
        value3_2 = 100;
        value4_2 = 50;
        value5_2 = 55.555555f;

        xml = std::string() + "<?xml version=\"1.0\"?>" + "<resource>"
              + "<property name=\"" + attribute1 + "\">" + value1 + "</property>"
              + "<property name=\"" + attribute2 + "\">" + value2 + "</property>"
              + "<property name=\"" + attribute3 + "\">" + value3 + "</property>"
              + "<property name=\"" + attribute4 + "\">" + value4 + "</property>"
              + "<property name=\"" + attribute5 + "\">" + value5 + "</property>"
              + "</resource>";
        tag = "property";

        res.readString(xml, tag);
    }

    virtual void TearDown() override
    {
        // EMPTY.
    }
};

TEST_F(ResourceFixtureTest, Clear)
{
    ASSERT_LT(0U, res.size());
    res.clear();
    ASSERT_EQ(0U, res.size());
}

TEST_F(ResourceFixtureTest, GetTag)
{
    ASSERT_EQ(tag, res.getTag());
}

TEST_F(ResourceFixtureTest, ReadFromXmlString)
{
    Resource::Map map;
    map = Resource::readFromXmlString(xml, tag);

    ASSERT_EQ(4U, map.size());
    ASSERT_EQ(value1_2, map.find(attribute1)->second);
    ASSERT_EQ(value2_2, map.find(attribute2)->second);
    ASSERT_EQ(value3  , map.find(attribute3)->second);
}

TEST_F(ResourceFixtureTest, Save)
{
    tttDir(true, true);
    auto const PATH = tttDir_Get() / "utf8.xml";

    Resource::Map map;
    map = Resource::readFromXmlString(xml, tag);
    ASSERT_TRUE(Resource::save(PATH.toString(), tag, map));
}

TEST_F(ResourceFixtureTest, GetValue)
{
    ASSERT_EQ(value1_2, res.getString(attribute1));

    ASSERT_EQ(value3_2, res.getInteger(attribute3));
    ASSERT_EQ(0, res.getInteger(attribute1));

    ASSERT_EQ(static_cast<unsigned int>(value3_2), res.getUnInteger(attribute3));
    ASSERT_EQ(0U, res.getUnInteger(attribute1));

    ASSERT_EQ(value3_2, res.getLongLong(attribute3));
    ASSERT_EQ(0, res.getLongLong(attribute1));

    ASSERT_EQ(static_cast<unsigned long long>(value3_2), res.getUnLongLong(attribute3));
    ASSERT_EQ(0U, res.getUnLongLong(attribute1));

    ASSERT_FLOAT_EQ(value5_2, res.getFloat(attribute5));
    ASSERT_FLOAT_EQ(0.0, res.getFloat(attribute1));

    float abs_error = 0.0001f;

    ASSERT_NEAR(value5_2, res.getFloat(attribute5), abs_error);
    ASSERT_NEAR(     0.0, res.getFloat(attribute1), abs_error);

    ASSERT_NEAR(value5_2, res.getDouble(attribute5), abs_error);
    ASSERT_NEAR(     0.0, res.getDouble(attribute1), abs_error);

    ASSERT_NEAR(value5_2, res.getLongDouble(attribute5), abs_error);
    ASSERT_NEAR(     0.0, res.getLongDouble(attribute1), abs_error);
}

TEST_F(ResourceFixtureTest, Get)
{
    ASSERT_EQ(value1_2, res.get(attribute1, ""));

    ASSERT_EQ(value3_2, res.get(attribute3, static_cast<      int>(0)));
    ASSERT_EQ(value3_2, res.get(attribute3, static_cast<long long>(0)));

    ASSERT_EQ(static_cast<unsigned       int>(value3_2), res.get(attribute3, static_cast<unsigned       int>(0)));
    ASSERT_EQ(static_cast<unsigned long long>(value3_2), res.get(attribute3, static_cast<unsigned long long>(0)));

    float abs_error = 0.0001f;
    ASSERT_NEAR(value5_2, res.get(attribute5, static_cast<      float>(0)), abs_error);
    ASSERT_NEAR(value5_2, res.get(attribute5, static_cast<     double>(0)), abs_error);
    ASSERT_NEAR(value5_2, res.get(attribute5, static_cast<long double>(0)), abs_error);
}

TEST_F(ResourceFixtureTest, Set)
{
    Resource res;
    res.set(attribute3, value3_2);
    ASSERT_EQ(value3, res.getString(attribute3));

    res.set(attribute4, value4_2);
    ASSERT_EQ(value4, res.getString(attribute4));
}

TEST_F(ResourceFixtureTest, At)
{
    ASSERT_EQ(value1_2, res.at(attribute1));

    std::string const TEST = "TEST";
    res.at(attribute1) = TEST;
    ASSERT_EQ(TEST, res.at(attribute1));
}

TEST_F(ResourceFixtureTest, ToXmlString)
{
    auto const XML_STRING = res.getXmlString();
    ASSERT_FALSE(XML_STRING.empty());
}

