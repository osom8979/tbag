/**
 * @file   TmxFrameTest.cpp
 * @brief  TmxFrame class tester.
 * @author zer0
 * @date   2019-07-23
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxFrame.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxFrameTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxFrame, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxFrame, obj2)
}

TEST(TmxFrameTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(<frame tileid="148" duration="1000"/>)";

    TmxFrame frame;
    ASSERT_EQ(E_SUCCESS, frame.read(TEST_XML));
    ASSERT_EQ(148, frame.tileid);
    ASSERT_EQ(1000, frame.duration);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, frame.write(xml));

    TmxFrame frame2;
    ASSERT_EQ(E_SUCCESS, frame2.read(xml));
    ASSERT_EQ(148, frame2.tileid);
    ASSERT_EQ(1000, frame2.duration);
}

