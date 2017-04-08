/**
 * @file   MultiAsset.cpp
 * @brief  MultiAsset class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/res/MultiAsset.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

TBAG_CONSTEXPR static char const * const DEFAULT_ASSET_LAYOUT[] = {
        ".",
        "assets",
        "db"    ,
        "dom"   ,
        "config",
        "image" ,
        "log"   ,
        "map"   ,
        "plugin",
        "save"  ,
        "script",
        "sprite",
        "string",
        "temp"  ,
};

TBAG_CONSTEXPR static std::size_t const DEFAULT_ASSET_LAYOUT_SIZE =
        sizeof(DEFAULT_ASSET_LAYOUT) / sizeof(DEFAULT_ASSET_LAYOUT[0]);

MultiAsset::MultiAsset()
{
    // EMPTY.
}

MultiAsset::MultiAsset(MultiAsset const & obj)
{
    (*this) = obj;
}

MultiAsset::MultiAsset(MultiAsset && obj)
{
    (*this) = std::move(obj);
}

MultiAsset::~MultiAsset()
{
    // EMPTY.
}

MultiAsset & MultiAsset::operator =(MultiAsset const & obj)
{
    if (this != &obj) {
        _assets.clear();
        _assets.insert(obj._assets.begin(), obj._assets.end());
    }
    return *this;
}

MultiAsset & MultiAsset::operator =(MultiAsset && obj)
{
    if (this != &obj) {
        _assets = std::move(obj._assets);
    }
    return *this;
}

bool MultiAsset::add(String const & name, DynamicAsset const & asset)
{
    return _assets.insert(AssetMapPair(name, asset)).second;
}

MultiAsset::DynamicAsset & MultiAsset::at(String const & name)
{
    return _assets.at(name);
}

MultiAsset::DynamicAsset const & MultiAsset::at(String const & name) const
{
    return _assets.at(name);
}

MultiAsset::StringVector MultiAsset::getKeys() const
{
    StringVector result;
    for (auto & asset : _assets) {
        result.push_back(asset.first);
    }
    return result;
}

bool MultiAsset::initAll()
{
    bool all_success = true;
    for (auto & asset : _assets) {
        all_success &= asset.second.init();
    }
    return all_success;
}

MultiAsset MultiAsset::create(PathVector const & paths, StringVector const & layouts)
{
    MultiAsset result;
    for (auto & path : paths) {
        DynamicAsset asset;
        for (auto & layout : layouts) {
            asset.add(layout, (path / layout));
        }
        result.add(path.getName(), asset);
    }
    return result;
}

MultiAsset::StringVector MultiAsset::getDefaultLayout()
{
    StringVector result;
    for (int i = 0; i < DEFAULT_ASSET_LAYOUT_SIZE; ++i) {
        result.push_back(std::string(DEFAULT_ASSET_LAYOUT[i]));
    }
    return result;
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

