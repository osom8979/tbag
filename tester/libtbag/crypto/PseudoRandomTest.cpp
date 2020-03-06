/**
 * @file   PseudoRandomTest.cpp
 * @brief  PseudoRandom class tester.
 * @author zer0
 * @date   2017-12-07
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/PseudoRandom.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(PseudoRandomTest, GenerateRandomString)
{
    std::size_t const BYTE_LENGTH = 10;
    std::string const RANDOM1 = generateRandomString(BYTE_LENGTH);
    std::string const RANDOM2 = generateRandomString(BYTE_LENGTH);
    ASSERT_FALSE(RANDOM1.empty());
    ASSERT_FALSE(RANDOM2.empty());
    ASSERT_EQ(BYTE_LENGTH, RANDOM1.size());
    ASSERT_EQ(BYTE_LENGTH, RANDOM2.size());
    ASSERT_NE(RANDOM1, RANDOM2);
}

TEST(PseudoRandomTest, GeneratePseudoRandomString)
{
    std::size_t const BYTE_LENGTH = 10;
    std::string const RANDOM1 = generatePseudoRandomString(BYTE_LENGTH);
    std::string const RANDOM2 = generatePseudoRandomString(BYTE_LENGTH);
    ASSERT_FALSE(RANDOM1.empty());
    ASSERT_FALSE(RANDOM2.empty());
    ASSERT_EQ(BYTE_LENGTH, RANDOM1.size());
    ASSERT_EQ(BYTE_LENGTH, RANDOM2.size());
    ASSERT_NE(RANDOM1, RANDOM2);
}

