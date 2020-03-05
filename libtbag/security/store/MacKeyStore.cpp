/**
 * @file   MacKeyStore.cpp
 * @brief  MacKeyStore class implementation.
 * @author zer0
 * @date   2017-12-07
 */

#include <libtbag/security/store/MacKeyStore.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace security {
namespace store    {

MacKeyStore::MacKeyStore(std::string const & params)
{
    // EMPTY.
}

MacKeyStore::~MacKeyStore()
{
    // EMPTY.
}

bool MacKeyStore::create(std::string const & key)
{
    return false;
}

bool MacKeyStore::remove(std::string const & key)
{
    return false;
}

bool MacKeyStore::exists(std::string const & key) const
{
    return false;
}

bool MacKeyStore::get(std::string const & key, std::string & result) const
{
    return false;
}

bool MacKeyStore::set(std::string const & key, std::string const & value, bool encrypt)
{
    return false;
}

bool MacKeyStore::cmp(std::string const & key, std::string const & value, bool encrypt) const
{
    return false;
}

std::set<std::string> MacKeyStore::list() const
{
    return {};
}

} // namespace store
} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

