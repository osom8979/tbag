/**
 * @file   WindowsKeyStore.cpp
 * @brief  WindowsKeyStore class implementation.
 * @author zer0
 * @date   2017-12-07
 */

#include <libtbag/security/store/WindowsKeyStore.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace security {
namespace store    {

WindowsKeyStore::WindowsKeyStore()
{
    // EMPTY.
}

WindowsKeyStore::~WindowsKeyStore()
{
    // EMPTY.
}

bool WindowsKeyStore::create(std::string const & key)
{
    return false;
}

bool WindowsKeyStore::remove(std::string const & key)
{
    return false;
}

std::string WindowsKeyStore::get(std::string const & key)
{
    return std::string();
}

bool WindowsKeyStore::set(std::string const & key, std::string const & value)
{
    return false;
}

std::vector<std::string> WindowsKeyStore::list()
{
    return std::vector<std::string>();
}

} // namespace store
} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

