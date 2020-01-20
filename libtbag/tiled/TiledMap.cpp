/**
 * @file   TiledMap.cpp
 * @brief  TiledMap class implementation.
 * @author zer0
 * @date   2020-01-07
 */

#include <libtbag/tiled/TiledMap.hpp>
#include <libtbag/filesystem/File.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled {

TiledMap::TiledMap(Callbacks * cb) : _cb(cb)
{
    // EMPTY.
}

TiledMap::~TiledMap()
{
    // EMPTY.
}

Err TiledMap::readFromFile(std::string const & path, bool auto_init)
{
    std::string content;
    auto const read_code = libtbag::filesystem::readFile(path, content);
    if (isFailure(read_code)) {
        return read_code;
    }
    return readFromXmlText(content, auto_init);
}

Err TiledMap::readFromXmlText(std::string const & xml, bool auto_init)
{
    auto const code = _map.read(xml);
    if (isSuccess(code) && auto_init) {
        init();
    }
    return code;
}

Err TiledMap::writeToFile(std::string const & path) const
{
    std::string content;
    auto const write_code = writeToXmlText(content);
    if (isFailure(write_code)) {
        return write_code;
    }
    return libtbag::filesystem::writeFile(path, content);
}

Err TiledMap::writeToXmlText(std::string & xml) const
{
    return _map.write(xml);
}

void TiledMap::init()
{
    if (!_cb) {
        return;
    }
    _cb->onInitMap(_map);
    // for (auto const & tileset : _map.tilesets) {
    //     _cb->onInitTileSet(tileset);
    // }
}

} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

