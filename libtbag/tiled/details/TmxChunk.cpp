/**
 * @file   TmxChunk.cpp
 * @brief  TmxChunk class implementation.
 * @author zer0
 * @date   2019-07-10
 */

#include <libtbag/tiled/details/TmxChunk.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxChunk::TmxChunk()
{
    // EMPTY.
}

TmxChunk::TmxChunk(int x_, int y_, int w_, int h_)
        : x(x_), y(y_), width(w_), height(h_)
{
    // EMPTY.
}

TmxChunk::~TmxChunk()
{
    // EMPTY.
}

bool TmxChunk::empty() const
{
    return gids.empty();
}

std::size_t TmxChunk::size() const
{
    return gids.size();
}

Err TmxChunk::read(Element const & elem, TileLayerFormat f)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    optAttr(elem, ATT_X, x);
    optAttr(elem, ATT_Y, y);
    optAttr(elem, ATT_WIDTH, width);
    optAttr(elem, ATT_HEIGHT, height);
    readGids(elem, gids, f);
    return E_SUCCESS;
}

Err TmxChunk::read(Element const & elem, Encoding e, Compression c)
{
    return read(elem, getTileLayerFormat(e, c));
}

Err TmxChunk::read(std::string const & xml, TileLayerFormat f)
{
    Document doc;
    auto const CODE = readFromXmlText(doc, xml);
    if (isFailure(CODE)) {
        return CODE;
    }
    auto const * elem = doc.FirstChildElement(TAG_NAME);
    assert(elem != nullptr);
    return read(*elem, f);
}

Err TmxChunk::read(std::string const & xml, Encoding e, Compression c)
{
    return read(xml, getTileLayerFormat(e, c));
}

Err TmxChunk::write(Element & elem, TileLayerFormat f) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_X, x);
    setAttr(elem, ATT_Y, y);
    setAttr(elem, ATT_WIDTH, width);
    setAttr(elem, ATT_HEIGHT, height);
    writeGids(elem, gids, f);
    return E_SUCCESS;
}

Err TmxChunk::write(Element & elem, Encoding e, Compression c) const
{
    return write(elem, getTileLayerFormat(e, c));
}

Err TmxChunk::write(std::string & xml, TileLayerFormat f) const
{
    Document doc;
    auto * new_elem = newElement(doc, TAG_NAME);
    assert(new_elem != nullptr);
    auto const CODE = write(*new_elem, f);
    if (isFailure(CODE)) {
        return CODE;
    }
    insertElement(doc, new_elem);
    return writeDocumentToXmlText(doc, xml);
}

Err TmxChunk::write(std::string & xml, Encoding e, Compression c) const
{
    return write(xml, getTileLayerFormat(e, c));
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

