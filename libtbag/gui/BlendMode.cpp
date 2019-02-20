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

BlendMode::BlendMode() : SfNative(SfType::ST_BLEND_MODE)
{
    // EMPTY.
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

#ifndef _self_sf
#define _self_sf() Parent::cast<sf::BlendMode>()
#endif

#undef _self_sf

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

