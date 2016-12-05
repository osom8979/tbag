/**
 * @file   MultiAsset.cpp
 * @brief  MultiAsset class implementation.
 * @author zer0
 * @date   2016-10-14
 */

#include <libtbag/res/MultiAsset.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

MultiAsset::Value const * const MultiAsset::LAYOUT_NAMES[] = {
          CHAR_OR_WIDECHAR(MultiAsset::Value, "data"  )
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "db"    )
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "dom"   )
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "config")
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "image" )
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "log"   )
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "map"   )
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "plugin")
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "save"  )
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "script")
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "sprite")
        , CHAR_OR_WIDECHAR(MultiAsset::Value, "temp"  )};

std::size_t const MultiAsset::LAYOUT_NAMES_SIZE =
        sizeof(MultiAsset::LAYOUT_NAMES) / sizeof(MultiAsset::LAYOUT_NAMES[0]);

bool MultiAsset::add(String const & name, DynamicAsset const & asset)
{
    return _assets.insert(AssetMapPair(name, asset)).second;
}

bool MultiAsset::add(String && name, DynamicAsset && asset)
{
    return _assets.insert(std::make_pair(std::move(name), std::move(asset))).second;
}

bool MultiAsset::add(String const & name, PathVector const & paths)
{
    std::size_t const SIZE = paths.size();
    std::basic_stringstream<Value> ss;

    DynamicAsset asset;

    for (std::size_t index = 0; index < SIZE; ++index) {
        ss.clear();
        ss << index;
        asset.addPath(ss.str(), paths[index]);
    }

    return add(String(name), std::move(asset));
}

MultiAsset::DynamicAsset MultiAsset::getAsset(String const & name) const
{
    return _assets.at(name);
}

MultiAsset::StringVector MultiAsset::getNames() const
{
    StringVector result;
    for (auto & asset : _assets) {
        result.push_back(asset.first);
    }
    return result;
}

MultiAsset::Path MultiAsset::findWriteableDir(String const & name) const
{
    auto find_itr = _assets.find(name);
    if (find_itr != _assets.end()) {
        return find_itr->second.findWriteableDir();
    }
    return Path();
}

MultiAsset::Path MultiAsset::findFile(String const & name, String const & filename) const
{
    auto find_itr = _assets.find(name);
    if (find_itr != _assets.end()) {
        return find_itr->second.findFile(filename);
    }
    return Path();
}

MultiAsset MultiAsset::create(PathVector const & paths, StringVector const & layouts)
{
    MultiAsset asset;
    for (auto & layout : layouts) {
        PathVector sub;
        for (auto & path : paths) {
            sub.push_back(path / layout);
        }
        asset.add(layout, sub);
    }
    return asset;
}

MultiAsset MultiAsset::createDefault()
{
    StringVector layouts;
    for (int i = 0; i < LAYOUT_NAMES_SIZE; ++i) {
        layouts.push_back(LAYOUT_NAMES[i]);
    }
    return create({Path::getExeDir(), Path::getHomeDir()}, layouts);
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

