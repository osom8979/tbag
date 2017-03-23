/**
 * @file   DynamicAsset.cpp
 * @brief  DynamicAsset class implementation.
 * @author zer0
 * @date   2016-10-13
 */

#include <libtbag/res/DynamicAsset.hpp>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

DynamicAsset::DynamicAsset()
{
    // EMPTY.
}

DynamicAsset::DynamicAsset(DynamicAsset const & obj)
{
    (*this) = obj;
}

DynamicAsset::DynamicAsset(DynamicAsset && obj)
{
    (*this) = std::move(obj);
}

DynamicAsset::~DynamicAsset()
{
    // EMPTY.
}

DynamicAsset & DynamicAsset::operator =(DynamicAsset const & obj)
{
    if (this != &obj) {
        _paths.clear();
        _paths.insert(obj._paths.begin(), obj._paths.end());
    }
    return *this;
}

DynamicAsset & DynamicAsset::operator =(DynamicAsset && obj)
{
    if (this != &obj) {
        _paths.swap(obj._paths);
    }
    return *this;
}

bool DynamicAsset::init()
{
    bool all_success = true;
    for (auto & path : _paths) {
        all_success &= path.second.createDir();
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

DynamicAsset::PathVector DynamicAsset::getPaths() const
{
    PathVector result;
    for (auto & path : _paths) {
        result.push_back(path.second);
    }
    return result;
}

bool DynamicAsset::exists(String const & key) const
{
    return _paths.at(key).isDirectory();
}

bool DynamicAsset::create(String const & key) const
{
    return _paths.at(key).createDir();
}

bool DynamicAsset::remove(String const & key) const
{
    return _paths.at(key).remove();
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

