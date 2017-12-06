/**
 * @file   KeyStoreTest.cpp
 * @brief  KeyStore class tester.
 * @author zer0
 * @date   2017-12-06
 */

#include <gtest/gtest.h>
#include <libtbag/security/KeyStore.hpp>

using namespace libtbag;
using namespace libtbag::security;

TEST(KeyStoreTest, Default)
{
    KeyStore object;
    ASSERT_TRUE(true);
}

