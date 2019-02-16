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

SfRenderTarget::SfRenderTarget(SfType type) : SfNative(type)
{
    // EMPTY.
}

SfRenderTarget::SfRenderTarget(SfType type, no_init_t) : SfNative(type, no_init)
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

