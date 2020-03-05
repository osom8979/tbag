/**
 * @file   KeyStore.hpp
 * @brief  KeyStore class prototype.
 * @author zer0
 * @date   2017-12-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_KEYSTORE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_KEYSTORE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/security/store/KeyStoreInterface.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace security {

/**
 * KeyStore class prototype.
 *
 * @author zer0
 * @date   2017-12-06
 */
class TBAG_API KeyStore : private Noncopyable
{
public:
    using KeyStoreInterface = libtbag::security::store::KeyStoreInterface;
    using SharedKeyStore = std::shared_ptr<KeyStoreInterface>;

public:
    enum class Type
    {
        T_FILE,
        T_GNOME,
        T_MAC,
        T_WINDOWS,
    };

public:
    SharedKeyStore _store;

public:
    KeyStore();
    virtual ~KeyStore();

public:
    bool open(Type type, std::string const & params);
    void close();
    bool isOpen() const;

public:
    bool create(std::string const & key);
    bool remove(std::string const & key);

    bool get(std::string const & key, std::string & result) const;
    bool set(std::string const & key, std::string const & value, bool encrypt = false);
    bool cmp(std::string const & key, std::string const & value, bool encrypt = false) const;

    std::vector<std::string> list() const;
};

} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_KEYSTORE_HPP__

