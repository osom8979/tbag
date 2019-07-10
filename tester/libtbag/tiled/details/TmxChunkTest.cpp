/**
 * @file   TmxChunkTest.cpp
 * @brief  TmxChunk class tester.
 * @author zer0
 * @date   2019-07-10
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxChunk.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxChunkTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxChunk, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxChunk, obj2)
}

TEST(TmxChunkTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(<chunk x="30" y="40" width="10" height="20" />)";

    TmxChunk chunk;
    ASSERT_EQ(E_SUCCESS, chunk.read(TEST_XML));
    ASSERT_EQ(30, chunk.x);
    ASSERT_EQ(40, chunk.y);
    ASSERT_EQ(10, chunk.width);
    ASSERT_EQ(20, chunk.height);

    std::string xml;
    ASSERT_EQ(E_SUCCESS, chunk.write(xml));

    TmxChunk chunk2;
    ASSERT_EQ(E_SUCCESS, chunk2.read(xml));
    ASSERT_EQ(30, chunk2.x);
    ASSERT_EQ(40, chunk2.y);
    ASSERT_EQ(10, chunk2.width);
    ASSERT_EQ(20, chunk2.height);
}

