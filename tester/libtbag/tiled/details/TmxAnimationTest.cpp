/**
 * @file   TmxAnimationTest.cpp
 * @brief  TmxAnimation class tester.
 * @author zer0
 * @date   2019-07-23
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxAnimation.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxAnimationTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxAnimation, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxAnimation, obj2)
}

TEST(TmxAnimationTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<animation>
  <frame tileid="37" duration="10"/>
  <frame tileid="46" duration="20"/>
  <frame tileid="55" duration="30"/>
  <frame tileid="64" duration="40"/>
</animation>
)";

    TmxAnimation animation;
    ASSERT_EQ(E_SUCCESS, animation.read(TEST_XML));
    ASSERT_EQ(4, animation.frames.size());
    ASSERT_EQ(37, animation.frames[0].tileid);
    ASSERT_EQ(10, animation.frames[0].duration);
    ASSERT_EQ(46, animation.frames[1].tileid);
    ASSERT_EQ(20, animation.frames[1].duration);
    ASSERT_EQ(55, animation.frames[2].tileid);
    ASSERT_EQ(30, animation.frames[2].duration);
    ASSERT_EQ(64, animation.frames[3].tileid);
    ASSERT_EQ(40, animation.frames[3].duration);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, animation.write(xml));

    TmxAnimation animation2;
    ASSERT_EQ(E_SUCCESS, animation2.read(xml));
    ASSERT_EQ(4, animation2.frames.size());
    ASSERT_EQ(37, animation2.frames[0].tileid);
    ASSERT_EQ(10, animation2.frames[0].duration);
    ASSERT_EQ(46, animation2.frames[1].tileid);
    ASSERT_EQ(20, animation2.frames[1].duration);
    ASSERT_EQ(55, animation2.frames[2].tileid);
    ASSERT_EQ(30, animation2.frames[2].duration);
    ASSERT_EQ(64, animation2.frames[3].tileid);
    ASSERT_EQ(40, animation2.frames[3].duration);
}

