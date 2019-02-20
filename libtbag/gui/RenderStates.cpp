/**
 * @file   RenderStates.cpp
 * @brief  RenderStates class implementation.
 * @author zer0
 * @date   2019-02-19
 */

#include <libtbag/gui/RenderStates.hpp>
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

RenderStates::RenderStates() : SfNative(SfType::ST_RENDER_STATES)
{
    // EMPTY.
}

RenderStates::RenderStates(RenderStates && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_RENDER_STATES, no_init)
{
    *this = std::move(obj);
}

RenderStates::~RenderStates()
{
    // EMPTY.
}

RenderStates & RenderStates::operator =(RenderStates && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void RenderStates::swap(RenderStates & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

#ifndef _self_sf
#define _self_sf() Parent::cast<sf::RenderStates>()
#endif

#undef _self_sf

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

