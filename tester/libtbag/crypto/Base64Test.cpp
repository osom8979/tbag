/**
 * @file   Base64Test.cpp
 * @brief  Base64 class tester.
 * @author zer0
 * @date   2017-12-07
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Base64.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(Base64Test, Default)
{
    std::string const ORIGINAL = "abcdefghijklmnopqrstuvwxyz";
    std::string const ENCODE   = "YWJjZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXo=";

    std::string encode;
    ASSERT_TRUE(encodeBase64(ORIGINAL, encode));
    ASSERT_EQ(ENCODE, encode);

    std::string decode;
    ASSERT_TRUE(decodeBase64(encode, decode));
    ASSERT_EQ(ORIGINAL, decode);
}

TEST(Base64Test, Default2)
{
    std::string const ORIGINAL = "EEE";
    std::string const ENCODE   = "RUVF";

    std::string encode;
    ASSERT_TRUE(encodeBase64(ORIGINAL, encode));
    ASSERT_EQ(ENCODE, encode);

    std::string decode;
    ASSERT_TRUE(decodeBase64(encode, decode));
    ASSERT_EQ(ORIGINAL, decode);
}

