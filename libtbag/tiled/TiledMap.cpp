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

    for (auto const & tileset : _map.tilesets) {
        for (auto const & terrain_type : tileset.terrain_types) {
        }
        for (auto const & wang_set : tileset.wang_sets) {
            for (auto const & cc : wang_set.wang_corner_colors) {
            }
            for (auto const & ec : wang_set.wang_edge_colors) {
            }
            for (auto const & t : wang_set.wang_tiles) {
            }
        }
        for (auto const & offset : tileset.tile_offsets) {
        }
        for (auto const & grid : tileset.grids) {
        }
        for (auto const & image : tileset.images) {
            // image.data
        }
        for (auto const & tile : tileset.tiles) {
            for (auto const & group : tile.object_group) {
            }
            for (auto const & frame : tile.animation) {
            }
            // tile.image.data
        }
    }

    for (auto const & layer : _map.layers) {
        // data;
    }

    for (auto const & object_group : _map.object_groups) {
        for (auto const & object : object_group) {
            // object.ellipse
            // object.polygon
            // object.polyline
            // object.text
            // object.image
        }
    }

    for (auto const & image_layer : _map.image_layers) {
        // image_layer.image
        // image_layer.image.data
    }

    for (auto const & group : _map.groups) {
        for (auto const & layer : group.layers) {
            // data;
        }
        for (auto const & object_group : group.object_groups) {
            for (auto const & object : object_group) {
            }
        }
        for (auto const & image_layer : group.image_layers) {
            // image_layer.image
            // image_layer.image.data
        }

        //while (true) {
        //    for (auto const & sub_group : group.groups) {
        //    }
        //}
    }
}

} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

