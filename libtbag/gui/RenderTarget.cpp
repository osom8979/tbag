/**
 * @file   RenderTarget.cpp
 * @brief  RenderTarget class implementation.
 * @author zer0
 * @date   2019-02-16
 */

#include <libtbag/gui/RenderTarget.hpp>
#include <libtbag/gui/View.hpp>
#include <libtbag/gui/Cursor.hpp>
#include <libtbag/gui/Drawable.hpp>
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

using Pointi = RenderTarget::Pointi;
using Pointu = RenderTarget::Pointu;
using Pointf = RenderTarget::Pointf;
using Sizeu  = RenderTarget::Sizeu;
using Recti  = RenderTarget::Recti;

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

#define _TBAG_NO_XX(name, type)

void RenderTarget::clear(Channel r, Channel g, Channel b, Channel a)
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        Pointer::cast<sf::type>()->clear(sf::Color(r, g, b, a)); \
        break;
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default: break;
    }
    // @formatter:on
}

void RenderTarget::clear(Rgb24 const & color)
{
    clear(color.r, color.g, color.b);
}

void RenderTarget::clear(Rgb32 const & color)
{
    clear(color.r, color.g, color.b, color.a);
}

void RenderTarget::clear()
{
    clear(CHANNEL_MIN, CHANNEL_MIN, CHANNEL_MIN);
}

void RenderTarget::setView(View const & view)
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        Pointer::cast<sf::type>()->setView(*view.cast<sf::View>()); \
        break;
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default: break;
    }
    // @formatter:on
}

View RenderTarget::getView() const
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        return View((void*)&(Pointer::cast<sf::type>()->getView()), no_init_no_ref);
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default:
        return View();
    }
    // @formatter:on
}

View RenderTarget::getDefaultView() const
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        return View((void*)&(Pointer::cast<sf::type>()->getDefaultView()), no_init_no_ref);
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default:
        return View();
    }
    // @formatter:on
}

Recti RenderTarget::getViewport(View const & view) const
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const RECT = Pointer::cast<sf::type>()->getViewport(*view.cast<sf::View>()); \
            return Recti(RECT.left, RECT.top, RECT.width, RECT.height); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default:
        return Recti();
    }
    // @formatter:on
}

Pointf RenderTarget::mapPixelToCoords(Pointi const & point) const
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const COORDS = Pointer::cast<sf::type>()->mapPixelToCoords(sf::Vector2i(point.x, point.y)); \
            return Pointf(COORDS.x, COORDS.y); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default:
        return Pointf();
    }
    // @formatter:on
}

Pointf RenderTarget::mapPixelToCoords(Pointi const & point, View const & view) const
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const COORDS = Pointer::cast<sf::type>()->mapPixelToCoords( \
                    sf::Vector2i(point.x, point.y), \
                    *view.cast<sf::View>()); \
            return Pointf(COORDS.x, COORDS.y); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default:
        return Pointf();
    }
    // @formatter:on
}

Pointi RenderTarget::mapCoordsToPixel(Pointf const & point) const
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const PIXEL = Pointer::cast<sf::type>()->mapCoordsToPixel(sf::Vector2f(point.x, point.y)); \
            return Pointi(PIXEL.x, PIXEL.y); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default:
        return Pointi();
    }
    // @formatter:on
}

Pointi RenderTarget::mapCoordsToPixel(Pointf const & point, View const & view) const
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const PIXEL = Pointer::cast<sf::type>()->mapCoordsToPixel( \
                    sf::Vector2f(point.x, point.y), \
                    *view.cast<sf::View>()); \
            return Pointi(PIXEL.x, PIXEL.y); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default:
        return Pointi();
    }
    // @formatter:on
}

Sizeu RenderTarget::getSize() const
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const SIZE = Pointer::cast<sf::type>()->getSize(); \
            return Sizeu(SIZE.x, SIZE.y); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default:
        return Sizeu();
    }
    // @formatter:on
}

bool RenderTarget::setActive(bool active)
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        return Pointer::cast<sf::type>()->setActive(active);
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default:
        return false;
    }
    // @formatter:on
}

void RenderTarget::pushGLStates()
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        Pointer::cast<sf::type>()->pushGLStates(); \
        break;
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default: break;
    }
    // @formatter:on
}

void RenderTarget::popGLStates()
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        Pointer::cast<sf::type>()->popGLStates(); \
        break;
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default: break;
    }
    // @formatter:on
}

void RenderTarget::resetGLStates()
{
    assert(isRenderTarget());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        Pointer::cast<sf::type>()->resetGLStates(); \
        break;
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX)
#undef _TBAG_YES_XX
    default: break;
    }
    // @formatter:on
}

#undef _TBAG_NO_XX

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

