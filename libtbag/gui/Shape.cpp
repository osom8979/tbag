/**
 * @file   Shape.cpp
 * @brief  Shape class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/Shape.hpp>
#include <libtbag/gui/Texture.hpp>
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

using Recti  = Shape::Recti;
using Rectf  = Shape::Rectf;
using Pointf = Shape::Pointf;
using Rgb32  = Shape::Rgb32;

Shape::Shape(SfType type, no_init_no_ref_t)
        : Drawable(type, no_init_no_ref)
{
    assert(isDrawableShape());
}

Shape::Shape(SfType type, no_init_t, bool ref)
        : Drawable(type, no_init, ref)
{
    assert(isDrawableShape());
}

Shape::Shape(SfType type, bool ref)
        : Drawable(type, ref)
{
    assert(isDrawableShape());
}

Shape::~Shape()
{
    // EMPTY.
}

void Shape::setTexture(Texture const * texture, bool reset_rect)
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    auto const * sf_texture = texture->cast<sf::Texture>();
    assert(sf_texture != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        Pointer::cast<sf::type>()->setTexture(sf_texture, reset_rect); \
        break;
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default: break;
    }
    // @formatter:on
}

void Shape::setTextureRect(Recti const & rect)
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        Pointer::cast<sf::type>()->setTextureRect(sf::IntRect(rect.x, rect.y, rect.width, rect.height)); \
        break;
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default: break;
    }
    // @formatter:on
}

void Shape::setFillColor(Rgb32 const & color)
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        Pointer::cast<sf::type>()->setFillColor(sf::Color(color.r, color.g, color.b, color.a)); \
        break;
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default: break;
    }
    // @formatter:on
}

void Shape::setOutlineColor(Rgb32 const & color)
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        Pointer::cast<sf::type>()->setOutlineColor(sf::Color(color.r, color.g, color.b, color.a)); \
        break;
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default: break;
    }
    // @formatter:on
}

void Shape::setOutlineThickness(float thickness)
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        Pointer::cast<sf::type>()->setOutlineThickness(thickness); \
        break;
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default: break;
    }
    // @formatter:on
}

void * Shape::getTexturePtr() const
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        return Pointer::cast<sf::type>()->getTexture(); \
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default:
        return nullptr;
    }
    // @formatter:on
}

Texture Shape::getTexture() const
{
    return Texture((void*)Shape::getTexturePtr(), Texture::no_init_no_ref);
}

Recti Shape::getTextureRect() const
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const & RECT = Pointer::cast<sf::type>()->getTextureRect(); \
            return Recti(RECT.left, RECT.top, RECT.width, RECT.height); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default:
        return Recti();
    }
    // @formatter:on
}

Rgb32 Shape::getFillColor() const
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const & COLOR = Pointer::cast<sf::type>()->getFillColor(); \
            return Rgb32(COLOR.r, COLOR.g, COLOR.b, COLOR.a); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default:
        return Rgb32();
    }
    // @formatter:on
}

Rgb32 Shape::getOutlineColor() const
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const & COLOR = Pointer::cast<sf::type>()->getOutlineColor(); \
            return Rgb32(COLOR.r, COLOR.g, COLOR.b, COLOR.a); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default:
        return Rgb32();
    }
    // @formatter:on
}

float Shape::getOutlineThickness() const
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        return Pointer::cast<sf::type>()->getOutlineThickness();
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default:
        return 0.f;
    }
    // @formatter:on
}

std::size_t Shape::getPointCount() const
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: \
        return Pointer::cast<sf::type>()->getPointCount();
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default:
        return 0;
    }
    // @formatter:on
}

Pointf Shape::getPoint(std::size_t index) const
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const POINT = Pointer::cast<sf::type>()->getPoint(index); \
            return Pointf(POINT.x, POINT.y); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default:
        return Pointf();
    }
    // @formatter:on
}

Rectf Shape::getLocalBounds() const
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const RECT = Pointer::cast<sf::type>()->getLocalBounds(); \
            return Rectf(RECT.left, RECT.top, RECT.width, RECT.height); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default:
        return Rectf();
    }
    // @formatter:on
}

Rectf Shape::getGlobalBounds() const
{
    assert(isDrawableShape());
    assert(ptr != nullptr);

    // @formatter:off
    switch (getType()) {
#define _TBAG_NO_XX(name, type)
#define _TBAG_YES_XX(name, type) \
    case SfType::ST_##name: { \
            auto const RECT = Pointer::cast<sf::type>()->getGlobalBounds(); \
            return Rectf(RECT.left, RECT.top, RECT.width, RECT.height); \
        }
    TBAG_SF_HANDLE_MAP(_TBAG_NO_XX, _TBAG_NO_XX, _TBAG_YES_XX, _TBAG_NO_XX)
#undef _TBAG_NO_XX
#undef _TBAG_YES_XX
    default:
        return Rectf();
    }
    // @formatter:on
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

