/**
 * @file   Sha1Test.cpp
 * @brief  Sha1 class tester.
 * @author zer0
 * @date   2017-04-11
 */

#include <gtest/gtest.h>
#include <libtbag/encrypt/Sha1.hpp>

using namespace libtbag;
using namespace libtbag::encrypt;

TEST(Sha1Test, Default)
{
    std::string const ORIGINAL = "abcdefghijklmnopqrstuvwxyz";
    std::string const ENCRYPT  = "32d10c7b8cf96570ca04ce37f2a19d84240d3a89";

    std::string encrypt;
    ASSERT_TRUE(encryptSha1(ORIGINAL, encrypt));
    ASSERT_EQ(ENCRYPT, encrypt);
}

