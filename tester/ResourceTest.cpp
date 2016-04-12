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

public:
    ResourceTest() = default;
    virtual ~ResourceTest() = default;

public:
    virtual void SetUp() override {
        xml = "<?xml version=\"1.0\"?>"
              "<resource>"
              "<property name=\"title1\">test&amp;title1</property>"
              "<property name=\"title2\">test&amp;title2</property>"
              "<property name=\"number\">100</property>"
              "<property name=\"number\">101</property>"
              "</resource>";
        tag = "property";
        attribute1 = "title1";
        attribute2 = "title2";
        attribute3 = "number";
        attribute4 = "number";
    }

    virtual void TearDown() override {
        __EMPTY_BLOCK__
    }
};

TEST_F(ResourceTest, readFromString)
{
    Resource::Map map;
    map = Resource::readFromString(xml, tag);

    ASSERT_EQ(map.size(), 3U);
    ASSERT_STREQ(map.find(attribute1)->second.c_str(), "test&title1");
    ASSERT_STREQ(map.find(attribute2)->second.c_str(), "test&title2");
    ASSERT_STREQ(map.find(attribute3)->second.c_str(), "100");
}

TEST_F(ResourceTest, save)
{
    Resource::Map map;
    map = Resource::readFromString(xml, tag);
    ASSERT_TRUE(Resource::save("Resource_Test_save.xml", tag, map));
}

