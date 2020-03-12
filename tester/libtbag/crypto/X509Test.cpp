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
    auto const PRIVATE_KEY = Rsa::generatePemPrivateKey();
    ASSERT_FALSE(PRIVATE_KEY.empty());

    auto const CSR = generateCsrVersion1(PRIVATE_KEY);
    ASSERT_FALSE(CSR.empty());

    std::cout << getPrintbleInformationFromPemCsr(CSR) << std::endl;
}

TEST(X509Test, Sign)
{
    auto const PRIVATE_KEY = Rsa::generatePemPrivateKey();
    ASSERT_FALSE(PRIVATE_KEY.empty());

    auto const CSR = generateCsrVersion1(PRIVATE_KEY);
    ASSERT_FALSE(CSR.empty());

    auto const CERTIFICATE = generateSelfSignedCertificate(PRIVATE_KEY, CSR);
    ASSERT_FALSE(CERTIFICATE.empty());

    std::cout << getPrintbleInformationFromPemX509(CERTIFICATE) << std::endl;
}

TEST(X509Test, GenX509PemToSingleText)
{
    std::cout << genX509PemToSingleText() << std::endl;
}

