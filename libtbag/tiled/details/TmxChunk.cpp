/**
 * @file   TmxChunk.cpp
 * @brief  TmxChunk class implementation.
 * @author zer0
 * @date   2019-07-10
 */

#include <libtbag/tiled/details/TmxChunk.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

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

Err TmxChunk::read(Element const & elem, Encoding e, Compression c)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_X, x);
    optAttr(elem, ATT_Y, y);
    optAttr(elem, ATT_WIDTH, width);
    optAttr(elem, ATT_HEIGHT, height);

    if (elem.NoChildren() && elem.GetText() != nullptr) {
    } else {
    }

    return E_SUCCESS;
}

Err TmxChunk::read(std::string const & xml, Encoding e, Compression c)
{
    Document doc;
    auto const CODE = readFromXmlText(doc, xml);
    if (isFailure(CODE)) {
        return CODE;
    }
    auto const * elem = doc.FirstChildElement(TAG_NAME);
    assert(elem != nullptr);
    return read(*elem, e, c);
}

Err TmxChunk::write(Element & elem, Encoding e, Compression c) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_X, x);
    setAttr(elem, ATT_Y, y);
    setAttr(elem, ATT_WIDTH, width);
    setAttr(elem, ATT_HEIGHT, height);
    return E_SUCCESS;
}

Err TmxChunk::write(std::string & xml, Encoding e, Compression c) const
{
    Document doc;
    auto * new_elem = newElement(doc, TAG_NAME);
    assert(new_elem != nullptr);
    auto const CODE = write(*new_elem, e, c);
    if (isFailure(CODE)) {
        return CODE;
    }
    insertElement(doc, new_elem);
    return writeToXmlText(doc, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

