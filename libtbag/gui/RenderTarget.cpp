/**
 * @file   RenderTarget.cpp
 * @brief  RenderTarget class implementation.
 * @author zer0
 * @date   2019-02-16
 */

#include <libtbag/gui/RenderTarget.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

RenderTarget::RenderTarget(SfType type, no_init_t, bool ref)
        : SfNative(type, no_init, ref)
{
    // EMPTY.
}

RenderTarget::RenderTarget(SfType type, bool ref)
        : SfNative(type, ref)
{
    // EMPTY.
}

RenderTarget::~RenderTarget()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

