/**
 * @file   AesTest.cpp
 * @brief  Aes class tester.
 * @author zer0
 * @date   2020-03-05
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Aes.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(AesTest, Default)
{
    Aes object;
    ASSERT_TRUE(true);
}

