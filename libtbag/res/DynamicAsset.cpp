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
    bool all_success = true;
    for (auto & path : _paths) {
        all_success &= path.second.createDirWithRecursive();
    }
    return all_success;
}

bool DynamicAsset::addPath(String const & key, Path const & path)
{
    return _paths.insert(PathMap::value_type(key, path)).second;
}

DynamicAsset::Path DynamicAsset::getPath(String const & key) const
{
    return _paths.at(key);
}

DynamicAsset::StringVector DynamicAsset::getKeys() const
{
    StringVector result;
    for (auto & path : _paths) {
        result.push_back(path.first);
    }
    return result;
}

bool DynamicAsset::exists(String const & key) const
{
    return filesystem::common::isDirectory(_paths.at(key));
}

bool DynamicAsset::create(String const & key) const
{
    return _paths.at(key).createDir();
}

bool DynamicAsset::remove(String const & key) const
{
    return _paths.at(key).removeDir();
}

DynamicAsset::PathVector DynamicAsset::scan(String const & key) const
{
    return _paths.at(key).scanDir();
}

DynamicAsset::Path DynamicAsset::findWriteableDir() const
{
    for (auto & path : _paths) {
        if (path.second.isDirectory() && path.second.isWritable()) {
            return path.second;
        }
    }
    return Path();
}

DynamicAsset::Path DynamicAsset::findFile(String const & filename) const
{
    for (auto & path : _paths) {
        auto file_path = path.second / filename;
        if (file_path.exists()) {
            return file_path;
        }
    }
    return Path();
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

