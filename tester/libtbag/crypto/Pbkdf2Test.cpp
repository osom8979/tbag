/**
 * @file   Pbkdf2Test.cpp
 * @brief  Pbkdf2 class tester.
 * @author zer0
 * @date   2017-12-07
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Pbkdf2.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(Pbkdf2Test, GeneratePbkdf2String)
{
    std::size_t const BYTE_LENGTH = 512;
    std::string const PASSWORD = "password";
    std::string const SALT = "salt";

    std::string const PASSWORD1 = generatePbkdf2String(PASSWORD, SALT, BYTE_LENGTH, Pbkdf2Type::PT_SHA512);
    std::string const PASSWORD2 = generatePbkdf2String(PASSWORD, SALT, BYTE_LENGTH, Pbkdf2Type::PT_SHA512);
    ASSERT_FALSE(PASSWORD1.empty());
    ASSERT_FALSE(PASSWORD2.empty());
    ASSERT_EQ(BYTE_LENGTH, PASSWORD1.size() / 2);
    ASSERT_EQ(BYTE_LENGTH, PASSWORD2.size() / 2);
    ASSERT_EQ(PASSWORD1, PASSWORD2);
}

