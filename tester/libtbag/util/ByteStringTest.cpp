/**
 * @file   ByteStringTest.cpp
 * @brief  ByteString class tester.
 * @author zer0
 * @date   2020-05-17
 */

#include <gtest/gtest.h>
#include <libtbag/util/ByteString.hpp>
#include <libtbag/Unit.hpp>

using namespace libtbag;
using namespace libtbag::util;

TEST(ByteStringTest, ParseByteSize)
{
    ASSERT_EQ(100, parseByteSize(" 100 ").val);
    ASSERT_EQ(10, parseByteSize(" 10 byte").val);
    ASSERT_EQ(5, parseByteSize("  5b").val);

    ASSERT_EQ(1024, parseByteSize("1Kb").val);
    ASSERT_EQ(1024, parseByteSize("1 kbyte").val);

    ASSERT_EQ(1llu*2048*1024*1024, parseByteSize("2048 mb").val);
    ASSERT_EQ(1llu*9*1024*1024, parseByteSize("9  Mbyte").val);

    ASSERT_EQ(1llu*2*1024*1024*1024, parseByteSize("2Gb").val);
    ASSERT_EQ(1llu*6*1024*1024*1024, parseByteSize("6g").val);

    ASSERT_NE(E_SUCCESS, parseByteSize("1---"));
}

TEST(ByteStringTest, ToUpperByteText)
{
#if 0
    // [WARNING] Outbound of 64bit size_t type.
    // ASSERT_STREQ("1y", toUpperByteText(YOTTA_BYTE_TO_BYTE).c_str());
    // ASSERT_STREQ("1z", toUpperByteText(ZETTA_BYTE_TO_BYTE).c_str());
#endif

    ASSERT_STREQ("1EB", toUpperByteText(EXA_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1PB", toUpperByteText(PETA_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1TB", toUpperByteText(TERA_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1GB", toUpperByteText(GIGA_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1MB", toUpperByteText(MEGA_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1KB", toUpperByteText(KILO_BYTE_TO_BYTE).c_str());
    ASSERT_STREQ("1B", toUpperByteText(1).c_str());
}

