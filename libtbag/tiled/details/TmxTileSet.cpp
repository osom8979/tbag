/**
 * @file   TmxTileSet.cpp
 * @brief  TmxTileSet class implementation.
 * @author zer0
 * @date   2019-08-15
 */

#include <libtbag/tiled/details/TmxTileSet.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxTileSet::TmxTileSet()
{
    // EMPTY.
}

TmxTileSet::~TmxTileSet()
{
    // EMPTY.
}

Err TmxTileSet::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_FIRSTGID, firstgid);
    optAttr(elem, ATT_SOURCE, source);
    optAttr(elem, ATT_NAME, name);
    optAttr(elem, ATT_TILEWIDTH, tilewidth);
    optAttr(elem, ATT_TILEHEIGHT, tileheight);
    optAttr(elem, ATT_SPACING, spacing);
    optAttr(elem, ATT_MARGIN, margin);
    optAttr(elem, ATT_TILECOUNT, tilecount);
    optAttr(elem, ATT_COLUMNS, columns);

    auto const * properties_elem = elem.FirstChildElement(TmxProperties::TAG_NAME);
    if (properties_elem != nullptr) {
        properties.read(*properties_elem);
    }

    auto const * terrain_types_elem = elem.FirstChildElement(TmxTerrainTypes::TAG_NAME);
    if (terrain_types_elem != nullptr) {
        terrain_types.read(*terrain_types_elem);
    }

    auto const * wang_sets_elem = elem.FirstChildElement(TmxWangSets::TAG_NAME);
    if (wang_sets_elem != nullptr) {
        wang_sets.read(*wang_sets_elem);
    }

    foreachElement(elem, TmxTileOffset::TAG_NAME, [&](Element const & e){
        TmxTileOffset tile_offset;
        if (isSuccess(tile_offset.read(e))) {
            tile_offsets.push_back(std::move(tile_offset));
        }
    });

    foreachElement(elem, TmxGrid::TAG_NAME, [&](Element const & e){
        TmxGrid grid;
        if (isSuccess(grid.read(e))) {
            grids.push_back(std::move(grid));
        }
    });

    foreachElement(elem, TmxImage::TAG_NAME, [&](Element const & e){
        TmxImage image;
        if (isSuccess(image.read(e))) {
            images.push_back(std::move(image));
        }
    });

    foreachElement(elem, TmxTile::TAG_NAME, [&](Element const & e){
        TmxTile tile;
        if (isSuccess(tile.read(e))) {
            tiles.push_back(std::move(tile));
        }
    });

    return E_SUCCESS;
}

Err TmxTileSet::read(std::string const & xml)
{
    Document doc;
    auto const CODE = readFromXmlText(doc, xml);
    if (isFailure(CODE)) {
        return CODE;
    }
    auto const * elem = doc.FirstChildElement(TAG_NAME);
    assert(elem != nullptr);
    return read(*elem);
}

Err TmxTileSet::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    setAttr(elem, ATT_FIRSTGID, firstgid);
    setAttr(elem, ATT_SOURCE, source);
    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_TILEWIDTH, tilewidth);
    setAttr(elem, ATT_TILEHEIGHT, tileheight);
    setAttr(elem, ATT_SPACING, spacing);
    setAttr(elem, ATT_MARGIN, margin);
    setAttr(elem, ATT_TILECOUNT, tilecount);
    setAttr(elem, ATT_COLUMNS, columns);

    if (!properties.empty()) {
        newElement(elem, TmxProperties::TAG_NAME, [&](Element & d){
            properties.write(d);
        });
    }

    if (!terrain_types.empty()) {
        newElement(elem, TmxTerrainTypes::TAG_NAME, [&](Element & d){
            terrain_types.write(d);
        });
    }

    if (!wang_sets.empty()) {
        newElement(elem, TmxWangSets::TAG_NAME, [&](Element & d){
            wang_sets.write(d);
        });
    }

    for (auto & tile_offset : tile_offsets) {
        newElement(elem, TmxTileOffset::TAG_NAME, [&](Element & p){
            tile_offset.write(p);
        });
    }

    for (auto & grid : grids) {
        newElement(elem, TmxGrid::TAG_NAME, [&](Element & p){
            grid.write(p);
        });
    }

    for (auto & image : images) {
        newElement(elem, TmxImage::TAG_NAME, [&](Element & p){
            image.write(p);
        });
    }

    for (auto & tile : tiles) {
        newElement(elem, TmxTile::TAG_NAME, [&](Element & p){
            tile.write(p);
        });
    }

    return E_SUCCESS;
}

Err TmxTileSet::write(std::string & xml) const
{
    Document doc;
    auto * new_elem = newElement(doc, TAG_NAME);
    assert(new_elem != nullptr);
    auto const CODE = write(*new_elem);
    if (isFailure(CODE)) {
        return CODE;
    }
    insertElement(doc, new_elem);
    return writeDocumentToXmlText(doc, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

