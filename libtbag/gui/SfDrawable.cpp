/**
 * @file   SfDrawable.cpp
 * @brief  SfDrawable class implementation.
 * @author zer0
 * @date   2019-02-13
 */

#include <libtbag/gui/SfDrawable.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

SfDrawable::SfDrawable(SfType type) : SfNative(type)
{
    // EMPTY.
}

SfDrawable::SfDrawable(int type) : SfNative(type)
{
    // EMPTY.
}

SfDrawable::~SfDrawable()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

