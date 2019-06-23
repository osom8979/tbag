/**
 * @file   TmxPoint.cpp
 * @brief  TmxPoint class implementation.
 * @author zer0
 * @date   2019-06-23
 */

#include <libtbag/tiled/details/TmxPoint.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxPoint::TmxPoint() : x(), y()
{
    // EMPTY.
}

TmxPoint::TmxPoint(int x_, int y_) : x(x_), y(y_)
{
    // EMPTY.
}

TmxPoint::~TmxPoint()
{
    // EMPTY.
}

Err TmxPoint::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    optAttr(elem, ATT_X, x);
    optAttr(elem, ATT_Y, y);
    return E_SUCCESS;
}

Err TmxPoint::read(std::string const & xml)
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

Err TmxPoint::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_X, x);
    setAttr(elem, ATT_Y, y);
    return E_SUCCESS;
}

Err TmxPoint::write(std::string & xml) const
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

