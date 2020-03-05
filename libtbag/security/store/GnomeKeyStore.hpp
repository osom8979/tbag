/**
 * @file   GnomeKeyStore.hpp
 * @brief  GnomeKeyStore class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_GNOMEKEYSTORE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_GNOMEKEYSTORE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/security/store/KeyStoreInterface.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace security {
namespace store    {

/**
 * GnomeKeyStore class prototype.
 *
 * @author zer0
 * @date   2017-12-07
 */
class TBAG_API GnomeKeyStore : private Noncopyable, public KeyStoreInterface
{
public:
    GnomeKeyStore(std::string const & params);
    virtual ~GnomeKeyStore();

public:
    bool create(std::string const & key) override;
    bool remove(std::string const & key) override;

    bool exists(std::string const & key) const override;
    bool get(std::string const & key, std::string & result) const override;
    bool set(std::string const & key, std::string const & value, bool encrypt = false) override;
    bool cmp(std::string const & key, std::string const & value, bool encrypt = false) const override;

    std::set<std::string> list() const override;
};

} // namespace store
} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_GNOMEKEYSTORE_HPP__

