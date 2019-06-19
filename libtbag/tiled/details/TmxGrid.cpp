/**
 * @file   TmxGrid.cpp
 * @brief  TmxGrid class implementation.
 * @author zer0
 * @date   2019-06-19
 */

#include <libtbag/tiled/details/TmxGrid.hpp>
#include <libtbag/log/Log.hpp>

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
    if (elem.Name() != TAG_NAME) {
        return E_ILLARGS;
    }

    Err code = optAttr(elem, ATT_WIDTH, width);
    if (isFailure(code)) {
        return code;
    }
    code = optAttr(elem, ATT_HEIGHT, height);
    if (isFailure(code)) {
        return code;
    }

    std::string orientation_text = getOrientationName(orientation);
    code = optAttr(elem, ATT_ORIENTATION, orientation_text);
    if (isFailure(code)) {
        return code;
    }
    return E_SUCCESS;
}

Err TmxGrid::dump(Element & elem) const
{
    if (elem.Name() != TAG_NAME) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_WIDTH, width);
    setAttr(elem, ATT_HEIGHT, height);
    setAttr(elem, ATT_ORIENTATION, getOrientationName(orientation));
    return E_SUCCESS;
}

Err TmxGrid::dumpToParent(Element & elem) const
{
    auto * new_elem = newElement(elem, TAG_NAME);
    assert(new_elem != nullptr);
    auto const CODE = dump(*new_elem);
    if (isFailure(CODE)) {
        return CODE;
    }
    insertElement(elem, new_elem);
    return E_SUCCESS;
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

