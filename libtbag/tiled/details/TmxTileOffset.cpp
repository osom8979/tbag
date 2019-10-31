/**
 * @file   TmxTileOffset.cpp
 * @brief  TmxTileOffset class implementation.
 * @author zer0
 * @date   2019-06-19
 */

#include <libtbag/tiled/details/TmxTileOffset.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxTileOffset::TmxTileOffset() : x(), y()
{
    // EMPTY.
}

TmxTileOffset::TmxTileOffset(int x_, int y_) : x(x_), y(y_)
{
    // EMPTY.
}

TmxTileOffset::~TmxTileOffset()
{
    // EMPTY.
}

Err TmxTileOffset::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    optAttr(elem, ATT_X, x);
    optAttr(elem, ATT_Y, y);
    return E_SUCCESS;
}

Err TmxTileOffset::read(std::string const & xml)
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

Err TmxTileOffset::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_X, x);
    setAttr(elem, ATT_Y, y);
    return E_SUCCESS;
}

Err TmxTileOffset::write(std::string & xml) const
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

