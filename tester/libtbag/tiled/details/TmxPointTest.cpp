/**
 * @file   TmxPointTest.cpp
 * @brief  TmxPoint class tester.
 * @author zer0
 * @date   2019-06-23
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxPoint.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxPointTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxPoint, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxPoint, obj2)
}

TEST(TmxPointTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(<point x="30" y="40" />)";

    TmxPoint ellipse;
    ASSERT_EQ(E_SUCCESS, ellipse.read(TEST_XML));
    ASSERT_EQ(30, ellipse.x);
    ASSERT_EQ(40, ellipse.y);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, ellipse.write(xml));

    TmxPoint ellipse2;
    ASSERT_EQ(E_SUCCESS, ellipse2.read(xml));
    ASSERT_EQ(30, ellipse2.x);
    ASSERT_EQ(40, ellipse2.y);
}

