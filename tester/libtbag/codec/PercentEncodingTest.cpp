/**
 * @file   PercentEncodingTest.cpp
 * @brief  PercentEncoding class tester.
 * @author zer0
 * @date   2018-11-22
 */

#include <gtest/gtest.h>
#include <libtbag/codec/PercentEncoding.hpp>

using namespace libtbag;
using namespace libtbag::codec;

TEST(PercentEncodingTest, Default)
{
    std::string const TEST_VAL = "http://hoho.com:8080/path/resource.html?query=true#fragment";
    ASSERT_EQ(TEST_VAL, encodePercent(TEST_VAL));
    ASSERT_EQ(TEST_VAL, decodePercent(TEST_VAL));
}

TEST(PercentEncodingTest, EncodeDecode)
{
    std::string const DECODED1 = "pipe://\\\\.\\pipe\\test.sock?query=true";
    std::string const ENCODED1 = "pipe://%5C%5C.%5Cpipe%5Ctest.sock?query=true";
    ASSERT_EQ(ENCODED1, encodePercent(DECODED1));
    ASSERT_EQ(DECODED1, decodePercent(ENCODED1));

    std::string const DECODED2 = "//@t2e2s2t@//";
    std::string const ENCODED2 = "//@t2e2s2t@//";
    ASSERT_EQ(ENCODED2, encodePercent(DECODED2));
    ASSERT_EQ(DECODED2, decodePercent(ENCODED2));

    std::string const DECODED3 = "TE MP";
    std::string const ENCODED3 = "TE%20MP";
    ASSERT_EQ(ENCODED3, encodePercent(DECODED3));
    ASSERT_EQ(DECODED3, decodePercent(ENCODED3));

    std::string const DECODED4 = "\\";
    std::string const ENCODED4 = "%5c";
    std::string const ENCODED4_2 = "%5C";
    ASSERT_EQ(ENCODED4_2, encodePercent(DECODED4));
    ASSERT_EQ(DECODED4, decodePercent(ENCODED4));
}

TEST(PercentEncodingTest, DecodeFailure)
{
    std::string const TEST_VAL = "h%tt%5p://%5";
    ASSERT_EQ(TEST_VAL, decodePercent(TEST_VAL));
}

