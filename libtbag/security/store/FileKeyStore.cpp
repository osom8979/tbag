/**
 * @file   FileKeyStore.cpp
 * @brief  FileKeyStore class implementation.
 * @author zer0
 * @date   2017-12-07
 */

#include <libtbag/security/store/FileKeyStore.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace security {
namespace store    {

FileKeyStore::FileKeyStore(std::string const & path)
{
    // EMPTY.
}

FileKeyStore::~FileKeyStore()
{
    // EMPTY.
}

bool FileKeyStore::create(std::string const & key)
{
    return false;
}

bool FileKeyStore::remove(std::string const & key)
{
    return false;
}

std::string FileKeyStore::get(std::string const & key)
{
    return std::string();
}

bool FileKeyStore::set(std::string const & key, std::string const & value)
{
    return false;
}

std::vector<std::string> FileKeyStore::list()
{
    return std::vector<std::string>();
}

} // namespace store
} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

