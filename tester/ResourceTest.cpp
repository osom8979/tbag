/**
 * @file   ResourceTest.cpp
 * @brief  Resource class tester.
 * @author zer0
 * @date   2016-04-12
 */

#include <gtest/gtest.h>
#include <libtbag/Resource.hpp>

using namespace libtbag;

class ResourceTest : public ::testing::Test
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
    ResourceTest() = default;
    virtual ~ResourceTest() = default;

public:
    virtual void SetUp() override {
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
        value5_2 = 55.555555;

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

    virtual void TearDown() override {
        __EMPTY_BLOCK__
    }
};

TEST_F(ResourceTest, clear_and_size)
{
    ASSERT_GT(res.size(), 0U);

    res.clear();
    ASSERT_EQ(res.size(), 0U);
}

TEST_F(ResourceTest, get_tag)
{
    ASSERT_EQ(res.get_tag(), tag);
}

TEST_F(ResourceTest, readFromXmlString)
{
    Resource::Map map;
    map = Resource::readFromXmlString(xml, tag);

    ASSERT_EQ(map.size(), 4U);
    ASSERT_EQ(map.find(attribute1)->second, value1_2);
    ASSERT_EQ(map.find(attribute2)->second, value2_2);
    ASSERT_EQ(map.find(attribute3)->second, value3);
}

TEST_F(ResourceTest, save)
{
    Resource::Map map;
    map = Resource::readFromXmlString(xml, tag);
    ASSERT_TRUE(Resource::save("Resource_Test_save.xml", tag, map));
}

TEST_F(ResourceTest, get)
{
    ASSERT_EQ(res.get(attribute1), value1_2);

    ASSERT_EQ(res.getInteger(attribute3), value3_2);
    ASSERT_EQ(res.getInteger(attribute1), 0);

    ASSERT_EQ(res.getUnInteger(attribute3), static_cast<unsigned int>(value3_2));
    ASSERT_EQ(res.getUnInteger(attribute1), 0U);

    ASSERT_EQ(res.getLongLong(attribute3), value3_2);
    ASSERT_EQ(res.getLongLong(attribute1), 0);

    ASSERT_EQ(res.getUnLongLong(attribute3), static_cast<unsigned long long>(value3_2));
    ASSERT_EQ(res.getUnLongLong(attribute1), 0U);

    ASSERT_FLOAT_EQ(res.getFloat(attribute5), value5_2);
    ASSERT_FLOAT_EQ(res.getFloat(attribute1), 0.0);

    float abs_error = 0.0001;

    ASSERT_NEAR(res.getFloat(attribute5), value5_2, abs_error);
    ASSERT_NEAR(res.getFloat(attribute1),      0.0, abs_error);

    ASSERT_NEAR(res.getDouble(attribute5), value5_2, abs_error);
    ASSERT_NEAR(res.getDouble(attribute1),      0.0, abs_error);

    ASSERT_NEAR(res.getLongDouble(attribute5), value5_2, abs_error);
    ASSERT_NEAR(res.getLongDouble(attribute1),      0.0, abs_error);
}

TEST_F(ResourceTest, set)
{
    Resource res;
    res.setInteger(attribute3, value3_2);
    ASSERT_EQ(res.get(attribute3), value3);

    res.setInteger(attribute4, value4_2);
    ASSERT_EQ(res.get(attribute4), value4);
}

TEST_F(ResourceTest, at)
{
    ASSERT_EQ(res.at(attribute1), value1_2);

    std::string const TEST = "TEST";
    res.at(attribute1) = TEST;
    ASSERT_EQ(res.at(attribute1), TEST);
}

