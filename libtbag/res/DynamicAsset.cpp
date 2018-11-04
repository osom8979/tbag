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

DynamicAsset::DynamicAsset(DynamicAsset && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

DynamicAsset::~DynamicAsset()
{
    // EMPTY.
}

DynamicAsset & DynamicAsset::operator =(DynamicAsset const & obj)
{
    copy(obj);
    return *this;
}

DynamicAsset & DynamicAsset::operator =(DynamicAsset && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void DynamicAsset::copy(DynamicAsset const & obj)
{
    if (this != &obj) {
        _paths.clear();
        _paths.insert(obj._paths.begin(), obj._paths.end());
    }
}

void DynamicAsset::swap(DynamicAsset & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _paths.swap(obj._paths);
    }
}

std::size_t DynamicAsset::createLayouts()
{
    std::size_t count = 0;
    for (auto & path : _paths) {
        if (!path.second.exists()) {
            if (path.second.createDir()) {
                ++count;
            }
        }
        //else if (!path.second.isDirectory()) {
        //    // Error!
        //}
    }
    return count;
}

bool DynamicAsset::add(std::string const & key, Path const & path)
{
    return _paths.insert(PathMap::value_type(key, path)).second;
}

void DynamicAsset::set(std::string const & key, Path const & path)
{
    auto itr = _paths.find(key);
    if (itr == _paths.end()) {
        add(key, path);
    } else {
        itr->second = path;
    }
}

DynamicAsset::Path DynamicAsset::get(std::string const & key) const
{
    auto itr = _paths.find(key);
    if (itr == _paths.end()) {
        return Path();
    }
    return itr->second;
}

DynamicAsset::Strings DynamicAsset::getKeys() const
{
    Strings result;
    for (auto & path : _paths) {
        result.push_back(path.first);
    }
    return result;
}

DynamicAsset::Paths DynamicAsset::getPaths() const
{
    Paths result;
    for (auto & path : _paths) {
        result.push_back(path.second);
    }
    return result;
}

bool DynamicAsset::exists(std::string const & key) const
{
    return _paths.find(key) != _paths.end();
}

bool DynamicAsset::createDir(std::string const & key) const
{
    auto itr = _paths.find(key);
    if (itr == _paths.end()) {
        return false;
    }
    return itr->second.createDir();
}

bool DynamicAsset::removeAll(std::string const & key) const
{
    auto itr = _paths.find(key);
    if (itr == _paths.end()) {
        return false;
    }
    return itr->second.removeAll();
}

DynamicAsset::Paths DynamicAsset::scan(std::string const & key) const
{
    auto itr = _paths.find(key);
    if (itr == _paths.end()) {
        return Paths();
    }
    return itr->second.scanDir();
}

// ------------------------
// Miscellaneous utilities.
// ------------------------

std::vector<std::string> getDefaultLayout()
{
    std::vector<std::string> result;
    for (std::size_t i = 0; i < DEFAULT_ASSET_LAYOUT_SIZE; ++i) {
        result.emplace_back(DEFAULT_ASSET_LAYOUT[i]);
    }
    return result;
}

DynamicAsset getDynamicAsset(filesystem::Path const & path, std::vector<std::string> const & layouts)
{
    DynamicAsset asset;
    for (auto & layout : layouts) {
        asset.add(layout, (path / layout));
    }
    return asset;
}

DynamicAsset getDynamicAsset(std::string const & path, std::vector<std::string> const & layouts)
{
    return getDynamicAsset(filesystem::Path(path), layouts);
}

DynamicAsset getDynamicAsset(filesystem::Path const & path)
{
    return getDynamicAsset(path, getDefaultLayout());
}

DynamicAsset getDynamicAsset(std::string const & path)
{
    return getDynamicAsset(filesystem::Path(path));
}

DynamicAsset getDynamicAsset()
{
    return getDynamicAsset(filesystem::Path::getExeDir() / filesystem::Path::getExePath().getName());
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

