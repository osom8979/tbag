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

TiledMap::TiledMap()
{
    // EMPTY.
}

TiledMap::~TiledMap()
{
    // EMPTY.
}

Err TiledMap::readFromFile(std::string const & path)
{
    std::string content;
    auto const read_code = libtbag::filesystem::readFile(path, content);
    if (isFailure(read_code)) {
        return read_code;
    }
    return readFromXmlText(content);
}

Err TiledMap::readFromXmlText(std::string const & xml)
{
    return _map.read(xml);
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

} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

