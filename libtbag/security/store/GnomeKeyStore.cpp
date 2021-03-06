/**
 * @file   GnomeKeyStore.cpp
 * @brief  GnomeKeyStore class implementation.
 * @author zer0
 * @date   2017-12-07
 */

#include <libtbag/security/store/GnomeKeyStore.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace security {
namespace store    {

GnomeKeyStore::GnomeKeyStore(std::string const & params)
{
    // EMPTY.
}

GnomeKeyStore::~GnomeKeyStore()
{
    // EMPTY.
}

bool GnomeKeyStore::create(std::string const & key)
{
    return false;
}

bool GnomeKeyStore::remove(std::string const & key)
{
    return false;
}

bool GnomeKeyStore::exists(std::string const & key) const
{
    return false;
}

bool GnomeKeyStore::get(std::string const & key, std::string & result) const
{
    return false;
}

bool GnomeKeyStore::set(std::string const & key, std::string const & value, bool encrypt)
{
    return false;
}

bool GnomeKeyStore::cmp(std::string const & key, std::string const & value, bool encrypt) const
{
    return false;
}

std::set<std::string> GnomeKeyStore::list() const
{
    return {};
}

} // namespace store
} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

