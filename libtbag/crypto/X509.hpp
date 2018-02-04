/**
 * @file   X509.hpp
 * @brief  X509 class prototype.
 * @author zer0
 * @date   2018-01-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_X509_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_X509_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <string>
#include <map>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

/**
 * X509 class prototype.
 *
 * @author zer0
 * @date   2018-01-31
 *
 * @remarks
 */
class TBAG_API X509 : private Noncopyable
{
public:
    /** The two-letter ISO abbreviation for your country. */
    TBAG_CONSTEXPR static char const * const COUNTRY = "C";
    /** The name of the state or province where your organization is located. You cannot abbreviate this name. */
    TBAG_CONSTEXPR static char const * const STATE = "ST";
    /** The name of the city where your organization is located. */
    TBAG_CONSTEXPR static char const * const LOCALITY = "L";
    /** The full legal name of your organization. Do not abbreviate your organization name. */
    TBAG_CONSTEXPR static char const * const ORGANIZATION = "O";
    /** [OPTIONAL] for additional organization information. */
    TBAG_CONSTEXPR static char const * const ORGANIZATIONAL_UNIT = "OU";
    /** The fully qualified domain name for your web site. This must match the domain name that users see when they visit your site, otherwise certificate errors will be shown. */
    TBAG_CONSTEXPR static char const * const COMMON_NAME = "CN";
    /** The site administrator's email address. */
    TBAG_CONSTEXPR static char const * const EMAIL_ADDRESS = "emailAddress";

public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl   = std::unique_ptr<Impl>;
    using Informations = std::map<std::string, std::string>;

private:
    UniqueImpl   _impl;
    Informations _infos;

public:
    X509();
    ~X509();

public:
    inline bool       empty() const TBAG_NOEXCEPT_SP_OP(_infos.empty()) { return _infos.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_infos. size()) { return _infos. size(); }

public:
    void clear();
    std::string get(std::string const & key) const;

public:
    bool insert(std::string const & key, std::string const & value);
};

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CRYPTO_X509_HPP__

