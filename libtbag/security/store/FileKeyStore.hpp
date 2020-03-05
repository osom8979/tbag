/**
 * @file   FileKeyStore.hpp
 * @brief  FileKeyStore class prototype.
 * @author zer0
 * @date   2017-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_FILEKEYSTORE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_FILEKEYSTORE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/security/store/KeyStoreInterface.hpp>
#include <libtbag/database/Sqlite.hpp>

#include <string>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace security {
namespace store    {

/**
 * FileKeyStore class prototype.
 *
 * @author zer0
 * @date   2017-12-07
 */
class TBAG_API FileKeyStore : private Noncopyable, public KeyStoreInterface
{
public:
    using Sqlite = database::Sqlite;
    using Mutex  = std::mutex;
    using Guard  = std::lock_guard<Mutex>;

public:
    TBAG_CONSTEXPR static char const * const TABLE_NAME = "t";
    TBAG_CONSTEXPR static char const * const   KEY_NAME = "k";
    TBAG_CONSTEXPR static char const * const  SALT_NAME = "s";
    TBAG_CONSTEXPR static char const * const VALUE_NAME = "v";

    TBAG_CONSTEXPR static char const * const getTableName() TBAG_NOEXCEPT { return TABLE_NAME; }
    TBAG_CONSTEXPR static char const * const getKeyName  () TBAG_NOEXCEPT { return   KEY_NAME; }
    TBAG_CONSTEXPR static char const * const getSaltName () TBAG_NOEXCEPT { return  SALT_NAME; }
    TBAG_CONSTEXPR static char const * const getValueName() TBAG_NOEXCEPT { return VALUE_NAME; }

public:
    TBAG_CONSTEXPR static std::size_t const HASH_SIZE = 512;
    TBAG_CONSTEXPR static std::size_t const SALT_SIZE = 512;

    TBAG_CONSTEXPR static std::size_t const getHashSize() TBAG_NOEXCEPT { return HASH_SIZE; }
    TBAG_CONSTEXPR static std::size_t const getSaltSize() TBAG_NOEXCEPT { return SALT_SIZE; }

private:
    Mutex mutable _mutex;
    Sqlite mutable _db;

public:
    FileKeyStore(std::string const & path);
    virtual ~FileKeyStore();

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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SECURITY_STORE_FILEKEYSTORE_HPP__

