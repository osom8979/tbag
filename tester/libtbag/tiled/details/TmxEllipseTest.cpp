/**
 * @file   TmxEllipseTest.cpp
 * @brief  TmxEllipse class tester.
 * @author zer0
 * @date   2019-06-23
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxEllipse.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxEllipseTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxEllipse, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxEllipse, obj2)
}

TEST(TmxEllipseTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(<ellipse width="10" height="20" x="30" y="40" />)";

    TmxEllipse ellipse;
    ASSERT_EQ(E_SUCCESS, ellipse.read(TEST_XML));
    ASSERT_EQ(10, ellipse.width);
    ASSERT_EQ(20, ellipse.height);
    ASSERT_EQ(30, ellipse.x);
    ASSERT_EQ(40, ellipse.y);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, ellipse.write(xml));

    TmxEllipse ellipse2;
    ASSERT_EQ(E_SUCCESS, ellipse2.read(xml));
    ASSERT_EQ(10, ellipse2.width);
    ASSERT_EQ(20, ellipse2.height);
    ASSERT_EQ(30, ellipse2.x);
    ASSERT_EQ(40, ellipse2.y);
}

