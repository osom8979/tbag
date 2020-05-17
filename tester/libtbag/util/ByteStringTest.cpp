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

TEST(ByteStringTest, ToByteSize)
{
    ASSERT_EQ(100, toByteSize(" 100 "));
    ASSERT_EQ(10, toByteSize(" 10 byte"));
    ASSERT_EQ(5, toByteSize("  5b"));

    ASSERT_EQ(1024, toByteSize("1Kb"));
    ASSERT_EQ(1, toByteSize("1---"));
    ASSERT_EQ(1024, toByteSize("1 kbyte"));

    ASSERT_EQ(2048*1024*(std::size_t)1024, toByteSize("2048 mb"));
    ASSERT_EQ(9*1024*(std::size_t)1024, toByteSize("9  Mbyte"));

    ASSERT_EQ(2*1024*1024*(std::size_t)1024, toByteSize("2Gb"));
    ASSERT_EQ(6*1024*1024*(std::size_t)1024, toByteSize("6g"));
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

