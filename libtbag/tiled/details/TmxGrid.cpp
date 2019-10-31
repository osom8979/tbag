/**
 * @file   TmxGrid.cpp
 * @brief  TmxGrid class implementation.
 * @author zer0
 * @date   2019-06-19
 */

#include <libtbag/tiled/details/TmxGrid.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxGrid::TmxGrid() : width(), height(), orientation(Orientation::NONE)
{
    // EMPTY.
}

TmxGrid::TmxGrid(int w, int h, Orientation o) : width(w), height(h), orientation(o)
{
    // EMPTY.
}

TmxGrid::~TmxGrid()
{
    // EMPTY.
}

TmxGrid::Orientation TmxGrid::getOrientation(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_ORTHOGONAL) {
        return Orientation::ORTHOGONAL;
    } else if (text == VAL_ISOMETRIC) {
        return Orientation::ISOMETRIC;
    } else {
        return Orientation::NONE;
    }
}

char const * const TmxGrid::getOrientationName(Orientation o) TBAG_NOEXCEPT
{
    // clang-format off
    switch (o) {
    case Orientation::ORTHOGONAL:  return VAL_ORTHOGONAL;
    case Orientation::ISOMETRIC:   return VAL_ISOMETRIC;
    case Orientation::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

Err TmxGrid::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_WIDTH, width);
    optAttr(elem, ATT_HEIGHT, height);

    std::string orientation_text;
    optAttr(elem, ATT_ORIENTATION, orientation_text);
    orientation = getOrientation(orientation_text);
    if (orientation == Orientation::NONE) {
        // Unknown Type value.
    }

    return E_SUCCESS;
}

Err TmxGrid::read(std::string const & xml)
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

Err TmxGrid::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_WIDTH, width);
    setAttr(elem, ATT_HEIGHT, height);
    setAttr(elem, ATT_ORIENTATION, getOrientationName(orientation));
    return E_SUCCESS;
}

Err TmxGrid::write(std::string & xml) const
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

