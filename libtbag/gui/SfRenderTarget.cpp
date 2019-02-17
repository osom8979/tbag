/**
 * @file   SfRenderTarget.cpp
 * @brief  SfRenderTarget class implementation.
 * @author zer0
 * @date   2019-02-16
 */

#include <libtbag/gui/SfRenderTarget.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

SfRenderTarget::SfRenderTarget(SfType type, no_init_t, bool ref)
        : SfNative(type, no_init, ref)
{
    // EMPTY.
}

SfRenderTarget::SfRenderTarget(SfType type, bool ref)
        : SfNative(type, ref)
{
    // EMPTY.
}

SfRenderTarget::~SfRenderTarget()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

