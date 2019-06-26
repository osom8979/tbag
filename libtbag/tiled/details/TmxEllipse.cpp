/**
 * @file   TmxEllipse.cpp
 * @brief  TmxEllipse class implementation.
 * @author zer0
 * @date   2019-06-23
 */

#include <libtbag/tiled/details/TmxEllipse.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxEllipse::TmxEllipse() : x(), y(), width(), height()
{
    // EMPTY.
}

TmxEllipse::TmxEllipse(int x_, int y_, int w_, int h_) : x(x_), y(y_), width(w_), height(h_)
{
    // EMPTY.
}

TmxEllipse::~TmxEllipse()
{
    // EMPTY.
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
    auto const CODE = readFromXml(doc, xml);
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
    return writeToXml(doc, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------
