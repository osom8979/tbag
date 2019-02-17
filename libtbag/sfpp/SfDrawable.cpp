/**
 * @file   SfDrawable.cpp
 * @brief  SfDrawable class implementation.
 * @author zer0
 * @date   2019-02-13
 */

#include <libtbag/sfpp/SfDrawable.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace sfpp {

SfDrawable::SfDrawable(SfType type, no_init_t, bool ref)
        : SfNative(type, no_init, ref)
{
    // EMPTY.
}

SfDrawable::SfDrawable(SfType type, bool ref)
        : SfNative(type, ref)
{
    // EMPTY.
}

SfDrawable::~SfDrawable()
{
    // EMPTY.
}

} // namespace sfpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

