/**
 * @file   SfRenderTarget.cpp
 * @brief  SfRenderTarget class implementation.
 * @author zer0
 * @date   2019-02-16
 */

#include <libtbag/gui/SfRenderTarget.hpp>
#include <libtbag/log/Log.hpp>

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

#include <cassert>

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

void SfRenderTarget::clear(Channel r, Channel g, Channel b, Channel a)
{
    assert(ptr != nullptr);
    cast<sf::RenderTarget>()->clear(sf::Color(r, g, b, a));
}

void SfRenderTarget::clear(Rgb24 const & color)
{
    clear(color.r, color.g, color.b);
}

void SfRenderTarget::clear(Rgb32 const & color)
{
    clear(color.r, color.g, color.b, color.a);
}

void SfRenderTarget::clear()
{
    clear(CHANNEL_MIN, CHANNEL_MIN, CHANNEL_MIN);
}

SfRenderTarget::Pointu SfRenderTarget::getSize() const
{
    assert(ptr != nullptr);
    auto const SIZE = cast<sf::RenderTarget>()->getSize();
    return Pointu(SIZE.x, SIZE.y);
}

bool SfRenderTarget::setActive(bool active)
{
    assert(ptr != nullptr);
    return cast<sf::RenderTarget>()->setActive(active);
}

void SfRenderTarget::pushGLStates()
{
    assert(ptr != nullptr);
    cast<sf::RenderTarget>()->pushGLStates();
}

void SfRenderTarget::popGLStates()
{
    assert(ptr != nullptr);
    cast<sf::RenderTarget>()->popGLStates();
}

void SfRenderTarget::resetGLStates()
{
    assert(ptr != nullptr);
    cast<sf::RenderTarget>()->resetGLStates();
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

