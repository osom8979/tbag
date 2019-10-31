/**
 * @file   TmxEllipse.cpp
 * @brief  TmxEllipse class implementation.
 * @author zer0
 * @date   2019-06-23
 */

#include <libtbag/tiled/details/TmxEllipse.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxEllipse::TmxEllipse()
{
    // EMPTY.
}

TmxEllipse::TmxEllipse(int x_, int y_, int w_, int h_)
        : x(x_), y(y_), width(w_), height(h_)
{
    // EMPTY.
}

TmxEllipse::~TmxEllipse()
{
    // EMPTY.
}

bool TmxEllipse::empty() const
{
    return x == 0 && y == 0 && width == 0 && height == 0;
}

Err TmxEllipse::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    optAttr(elem, ATT_X, x);
    optAttr(elem, ATT_Y, y);
    optAttr(elem, ATT_WIDTH, width);
    optAttr(elem, ATT_HEIGHT, height);
    return E_SUCCESS;
}

Err TmxEllipse::read(std::string const & xml)
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

Err TmxEllipse::write(Element & elem) const
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

Err TmxEllipse::write(std::string & xml) const
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

