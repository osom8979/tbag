/**
 * @file   Md5Test.cpp
 * @brief  Md5 class tester.
 * @author zer0
 * @date   2017-12-07
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Md5.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(Md5Test, Default)
{
    std::string const TEST_STRING = "TEST MD5 STRING";
    std::string const TEST_MD5 = "7001438affef799414d78936dc034c82";

    std::string result;
    ASSERT_TRUE(encryptMd5(TEST_STRING, result));
    ASSERT_EQ(TEST_MD5, result);
}

