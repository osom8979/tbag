/**
 * @file   BlendMode.cpp
 * @brief  BlendMode class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/gui/BlendMode.hpp>
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

#ifndef _self_sf
#define _self_sf() Pointer::cast<sf::BlendMode>()
#endif

BlendMode::BlendMode() : SfNative(SfType::ST_BLEND_MODE)
{
    assert(ptr != nullptr);
}

BlendMode::BlendMode(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_BLEND_MODE, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

BlendMode::BlendMode(BlendMode && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_BLEND_MODE, no_init)
{
    *this = std::move(obj);
}

BlendMode::~BlendMode()
{
    // EMPTY.
}

BlendMode & BlendMode::operator =(BlendMode && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void BlendMode::swap(BlendMode & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

