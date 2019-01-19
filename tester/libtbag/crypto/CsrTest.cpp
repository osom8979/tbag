/**
 * @file   CsrTest.cpp
 * @brief  Csr class tester.
 * @author zer0
 * @date   2019-01-19
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Csr.hpp>
#include <libtbag/crypto/Rsa.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

TEST(CsrTest, Default)
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

