/**
 * @file   TmxWangSet.cpp
 * @brief  TmxWangSet class implementation.
 * @author zer0
 * @date   2019-07-14
 */

#include <libtbag/tiled/details/TmxWangSet.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxWangSet::TmxWangSet()
{
    // EMPTY.
}

TmxWangSet::TmxWangSet(std::string const & n, int t) : name(n), tile(t)
{
    // EMPTY.
}

TmxWangSet::~TmxWangSet()
{
    // EMPTY.
}

Err TmxWangSet::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    optAttr(elem, ATT_NAME, name);
    optAttr(elem, ATT_TILE, tile);

    foreachElement(elem, TmxWangCornerColor::TAG_NAME, [&](Element const & e){
        TmxWangCornerColor wang_corner_color;
        if (isSuccess(wang_corner_color.read(e))) {
            wang_corner_colors.push_back(std::move(wang_corner_color));
        }
    });

    foreachElement(elem, TmxWangEdgeColor::TAG_NAME, [&](Element const & e){
        TmxWangEdgeColor wang_edge_color;
        if (isSuccess(wang_edge_color.read(e))) {
            wang_edge_colors.push_back(std::move(wang_edge_color));
        }
    });

    foreachElement(elem, TmxWangTile::TAG_NAME, [&](Element const & e){
        TmxWangTile wang_tile;
        if (isSuccess(wang_tile.read(e))) {
            wang_tiles.push_back(std::move(wang_tile));
        }
    });

    return E_SUCCESS;
}

Err TmxWangSet::read(std::string const & xml)
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

Err TmxWangSet::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_NAME, name);
    setAttr(elem, ATT_TILE, tile);

    for (auto & wang_corner_color : wang_corner_colors) {
        newElement(elem, TmxWangCornerColor::TAG_NAME, [&](Element & p){
            wang_corner_color.write(p);
        });
    }

    for (auto & wang_edge_color : wang_edge_colors) {
        newElement(elem, TmxWangEdgeColor::TAG_NAME, [&](Element & p){
            wang_edge_color.write(p);
        });
    }

    for (auto & wang_tile : wang_tiles) {
        newElement(elem, TmxWangTile::TAG_NAME, [&](Element & p){
            wang_tile.write(p);
        });
    }

    return E_SUCCESS;
}

Err TmxWangSet::write(std::string & xml) const
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

