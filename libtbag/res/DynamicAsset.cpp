/**
 * @file   DynamicAsset.cpp
 * @brief  DynamicAsset class implementation.
 * @author zer0
 * @date   2016-10-13
 */

#include <libtbag/res/DynamicAsset.hpp>
#include <libtbag/filesystem/Common.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

bool DynamicAsset::init()
{
    for (auto & path : _paths) {
        if (path.second.createDir() == false) {
            return false;
        }
    }
    return true;
}

bool DynamicAsset::addPath(String const & key, Path const & path)
{
    return _paths.insert(PathMap::value_type(key, path)).second;
}

DynamicAsset::Path DynamicAsset::getPath(String const & key)
{
    return _paths.at(key);
}

bool DynamicAsset::exists(String const & key)
{
    return filesystem::common::isDirectory(_paths.at(key));
}

bool DynamicAsset::create(String const & key)
{
    return _paths.at(key).createDir();
}

bool DynamicAsset::remove(String const & key)
{
    return _paths.at(key).removeDir();
}

DynamicAsset::PathVector DynamicAsset::scan(String const & key)
{
    return _paths.at(key).scanDir();
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

