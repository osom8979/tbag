/**
 * @file   X509Test.cpp
 * @brief  X509 class tester.
 * @author zer0
 * @date   2018-01-31
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/X509.hpp>
#include <libtbag/crypto/Rsa.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(X509Test, Csr)
{
    std::string public_key;
    std::string private_key;

    ASSERT_TRUE(Rsa::generatePem(public_key, private_key));
    ASSERT_FALSE(public_key.empty());
    ASSERT_FALSE(private_key.empty());

    std::string const CSR = generateCsrVersion1(private_key);
    ASSERT_FALSE(CSR.empty());

    std::cout << decoderCsr(CSR) << std::endl;
}

TEST(X509Test, Sign)
{
}

