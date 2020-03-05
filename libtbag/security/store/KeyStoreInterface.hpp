/**
 * @file   KeyStoreInterface.hpp
 * @brief  KeyStoreInterface class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_KEYSTOREINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_KEYSTOREINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <set>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace security {
namespace store    {

/**
 * KeyStore interface prototype.
 *
 * @author zer0
 * @date   2017-12-07
 */
struct KeyStoreInterface
{
    virtual bool create(std::string const & key) = 0;
    virtual bool remove(std::string const & key) = 0;

    virtual bool exists(std::string const & key) const = 0;
    virtual bool get(std::string const & key, std::string & result) const = 0;
    virtual bool set(std::string const & key, std::string const & value, bool encrypt = false) = 0;
    virtual bool cmp(std::string const & key, std::string const & value, bool encrypt = false) const = 0;

    virtual std::set<std::string> list() const = 0;
};

} // namespace store
} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_KEYSTOREINTERFACE_HPP__

