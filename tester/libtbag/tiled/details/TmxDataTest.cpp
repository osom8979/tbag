/**
 * @file   TmxDataTest.cpp
 * @brief  TmxData class tester.
 * @author zer0
 * @date   2019-07-09
 */

#include <gtest/gtest.h>
#include <libtbag/tiled/details/TmxData.hpp>
#include <libtbag/util/TestUtils.hpp>
#include <string>

using namespace libtbag;
using namespace libtbag::tiled;
using namespace libtbag::tiled::details;

TEST(TmxDataTest, Default)
{
    TEST_DEFAULT_CONSTRUCTOR(TmxData, obj1)
    TEST_DEFAULT_ASSIGNMENT(TmxData, obj2)
}

TEST(TmxDataTest, ReadAndWrite)
{
    char const * const TEST_XML = R"(
<data encoding="base64" compression="zlib">
abcd
</data>
)";

    TmxData data;
    ASSERT_EQ(E_SUCCESS, data.read(TEST_XML));
    ASSERT_EQ(TmxData::Encoding::BASE64, data.encoding);
    ASSERT_EQ(TmxData::Compression::ZLIB, data.compression);
    ASSERT_STREQ("abcd", data.data.c_str());

    std::string xml;
    ASSERT_EQ(E_SUCCESS, data.write(xml));

    TmxData data2;
    ASSERT_EQ(E_SUCCESS, data2.read(xml));
    ASSERT_EQ(TmxData::Encoding::BASE64, data2.encoding);
    ASSERT_EQ(TmxData::Compression::ZLIB, data2.compression);
    ASSERT_STREQ("abcd", data2.data.c_str());
}

