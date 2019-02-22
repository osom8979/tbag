/**
 * @file   RenderTarget.cpp
 * @brief  RenderTarget class implementation.
 * @author zer0
 * @date   2019-02-16
 */

#include <libtbag/gui/RenderTarget.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

RenderTarget::RenderTarget(SfType type, no_init_no_ref_t)
        : SfNative(type, no_init_no_ref)
{
    assert(isRenderTarget());
}

RenderTarget::RenderTarget(SfType type, no_init_t, bool ref)
        : SfNative(type, no_init, ref)
{
    assert(isRenderTarget());
}

RenderTarget::RenderTarget(SfType type, bool ref)
        : SfNative(type, ref)
{
    assert(isRenderTarget());
}

RenderTarget::~RenderTarget()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

