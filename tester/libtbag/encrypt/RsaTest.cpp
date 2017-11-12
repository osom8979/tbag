/**
 * @file   RsaTest.cpp
 * @brief  Rsa class tester.
 * @author zer0
 * @date   2017-11-11
 */

#include <gtest/gtest.h>
#include <libtbag/encrypt/Rsa.hpp>

using namespace libtbag;
using namespace libtbag::encrypt;

TEST(RsaTest, Default)
{
    std::vector<char> public_key;
    std::vector<char> private_key;
    ASSERT_TRUE(generateRsa(public_key, private_key));

    std::cout << std::string(public_key.begin(), public_key.end()) << std::endl;
    std::cout << std::string(private_key.begin(), private_key.end()) << std::endl;
}

