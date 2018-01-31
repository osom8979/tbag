/**
 * @file   X509Test.cpp
 * @brief  X509 class tester.
 * @author zer0
 * @date   2018-01-31
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/X509.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(X509Test, Default)
{
    X509 object;
    ASSERT_TRUE(true);
}

