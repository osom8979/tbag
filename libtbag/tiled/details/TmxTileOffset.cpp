/**
 * @file   TmxTileOffset.cpp
 * @brief  TmxTileOffset class implementation.
 * @author zer0
 * @date   2019-06-19
 */

#include <libtbag/tiled/details/TmxTileOffset.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

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
    Err code = optAttr(elem, ATT_X, x);
    if (isFailure(code)) {
        return code;
    }
    code = optAttr(elem, ATT_Y, y);
    if (isFailure(code)) {
        return code;
    }
    return E_SUCCESS;
}

Err TmxTileOffset::dump(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_X, x);
    setAttr(elem, ATT_Y, y);
    return E_SUCCESS;
}

Err TmxTileOffset::dumpToParent(Element & elem) const
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

