/**
 * @file   Csr.hpp
 * @brief  Csr class prototype.
 * @author zer0
 * @date   2019-01-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_CSR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_CSR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

/**
 * The two-letter ISO abbreviation for your country.
 */
TBAG_CONSTEXPR char const * const COUNTRY = "C";

/**
 * The name of the state or province where your organization is located.
 * You cannot abbreviate this name.
 */
TBAG_CONSTEXPR char const * const STATE = "ST";

/**
 * The name of the city where your organization is located.
 */
TBAG_CONSTEXPR char const * const LOCALITY = "L";

/**
 * The full legal name of your organization.
 * Do not abbreviate your organization name.
 */
TBAG_CONSTEXPR char const * const ORGANIZATION = "O";

/** [OPTIONAL] for additional organization information. */
TBAG_CONSTEXPR char const * const ORGANIZATIONAL_UNIT = "OU";

/**
 * The fully qualified domain name for your web site.
 * This must match the domain name that users see when they visit your site,
 * otherwise certificate errors will be shown.
 */
TBAG_CONSTEXPR char const * const COMMON_NAME = "CN";

/**
 * The site administrator's email address.
 */
TBAG_CONSTEXPR char const * const EMAIL_ADDRESS = "emailAddress";

struct X509NameEntry
{
    std::string key;
    std::string val;
};

using X509NameEntries = std::vector<X509NameEntry>;

TBAG_API X509NameEntries getDefaultX509NameEntry(
        std::string const & country,
        std::string const & state,
        std::string const & locality,
        std::string const & organization,
        std::string const & organizational_unit,
        std::string const & common_name,
        std::string const & email_address);
TBAG_API X509NameEntries getDefaultX509NameEntry();

/**
 * Generate X.509 CSR(Certificate Signing Request) File.
 */
TBAG_API std::string generateCsrVersion1(X509NameEntries const & entries, std::string const & pem_private_key);
TBAG_API std::string generateCsrVersion1(std::string const & pem_private_key);

/**
 * @param[in] name_options
 *      Various certificate name options.
 * @param[in] request_options
 *      Various request text options.
 */
TBAG_API std::string decoderCsr(std::string const & pem_csr,
                                std::string const & name_options,
                                std::string const & request_options);
TBAG_API std::string decoderCsr(std::string const & pem_csr);

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_CSR_HPP__

