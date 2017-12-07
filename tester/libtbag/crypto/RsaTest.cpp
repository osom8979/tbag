/**
 * @file   RsaTest.cpp
 * @brief  Rsa class tester.
 * @author zer0
 * @date   2017-12-07
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Rsa.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(RsaTest, Default)
{
    std::string public_key;
    std::string private_key;

    ASSERT_TRUE(Rsa::generate(public_key, private_key));
    ASSERT_FALSE(public_key.empty());
    ASSERT_FALSE(private_key.empty());

    std::cout << public_key << std::endl;
    std::cout << private_key << std::endl;
}

