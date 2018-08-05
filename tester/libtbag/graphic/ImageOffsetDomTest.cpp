/**
 * @file   ImageOffsetDomTest.cpp
 * @brief  ImageOffsetDom class tester.
 * @author zer0
 * @date   2018-08-04
 */

#include <gtest/gtest.h>
#include <libtbag/graphic/ImageOffsetDom.hpp>
#include <libtbag/util/TestUtils.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(ImageOffsetDomDefault, Coverage)
{
    TEST_DEFAULT_CONSTRUCTOR(ImageOffsetDom, obj);
    TEST_DEFAULT_ASSIGNMENT(ImageOffsetDom, obj);
}

struct ImageOffsetDomTest : public testing::Test
{
    TBAG_CONSTEXPR static char const * const TEST_XML =
R"<xml>(
<?xml version="1.0" encoding="utf-8" ?>
<images>
    <image file="a.png" name="a1" x="10" y="20" width="30" height="40" origin_x="5" origin_y="6" weight="1" />
    <image file="b.png" name="b1" x="50" y="60" width="70" height="80" origin_x="2" origin_y="3" weight="2">
        { "test": "json_string" }
    </image>
</images>
)<xml>";

    ImageOffsetDom dom;

    virtual void SetUp() override
    {
        ASSERT_TRUE(dom.loadFromMemory(TEST_XML));
    }

    virtual void TearDown() override
    {
        dom.clear();
    }
};

TEST_F(ImageOffsetDomTest, Default)
{
    ASSERT_EQ(2, dom.size());

    ImageOffsetDom::Item item1;
    ASSERT_TRUE(dom.find("a1", &item1));
    ASSERT_STREQ("a.png", item1.file.c_str());
    ASSERT_EQ(10, item1.x);
    ASSERT_EQ(20, item1.y);
    ASSERT_EQ(30, item1.width);
    ASSERT_EQ(40, item1.height);
    ASSERT_EQ( 5, item1.origin_x);
    ASSERT_EQ( 6, item1.origin_y);
    ASSERT_EQ( 1, item1.weight);
    ASSERT_TRUE(item1.body.empty());

    ImageOffsetDom::Item item2;
    ASSERT_TRUE(dom.find("b1", &item2));
    ASSERT_STREQ("b.png", item2.file.c_str());
    ASSERT_EQ(50, item2.x);
    ASSERT_EQ(60, item2.y);
    ASSERT_EQ(70, item2.width);
    ASSERT_EQ(80, item2.height);
    ASSERT_EQ( 2, item2.origin_x);
    ASSERT_EQ( 3, item2.origin_y);
    ASSERT_EQ( 2, item2.weight);
    ASSERT_FALSE(item2.body.empty());
}

TEST_F(ImageOffsetDomTest, ToXmlString)
{
    auto temp = ImageOffsetDom::readXml(dom.toXmlString());
    ASSERT_EQ(2, temp.size());
}

