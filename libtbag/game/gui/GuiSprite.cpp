/**
 * @file   GuiSprite.cpp
 * @brief  GuiSprite class implementation.
 * @author zer0
 * @date   2019-04-07
 */

#include <libtbag/game/gui/GuiSprite.hpp>
#include <libtbag/game/gui/GuiTexture.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

#include <SFML/Graphics.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {
namespace gui  {

/**
 * GuiSprite::Impl class implementation.
 *
 * @author zer0
 * @date   2019-04-07
 */
struct GuiSprite::Impl : public sf::Sprite
{
private:
    GuiSprite * _parent;
    GuiTexture _texture;

public:
    Impl(GuiSprite * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    void setTexture(GuiTexture const & texture, bool reset_rect)
    {
        _texture = texture;
        sf::Sprite::setTexture((sf::Texture const &)_texture.ref(), reset_rect);
    }

    GuiTexture const & getTexture() const
    {
        return _texture;
    }
};

// ------------------------
// GuiSprite implementation
// ------------------------

// clang-format off
using Size2f  = GuiSprite::Size2f;
using Point2f = GuiSprite::Point2f;
using Rgb32   = GuiSprite::Rgb32;
using Rect2f  = GuiSprite::Rect2f;
using Rect2i  = GuiSprite::Rect2i;
// clang-format on

GuiSprite::GuiSprite() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

GuiSprite::GuiSprite(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

GuiSprite::GuiSprite(GuiSprite const & obj) TBAG_NOEXCEPT : GuiSprite(nullptr)
{
    (*this) = obj;
}

GuiSprite::GuiSprite(GuiSprite && obj) TBAG_NOEXCEPT : GuiSprite(nullptr)
{
    (*this) = std::move(obj);
}

GuiSprite::~GuiSprite()
{
    // EMPTY.
}

GuiSprite & GuiSprite::operator =(GuiSprite const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

GuiSprite & GuiSprite::operator =(GuiSprite && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void GuiSprite::copy(GuiSprite const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void GuiSprite::swap(GuiSprite & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void GuiSprite::reset()
{
    _impl.reset();
}

void GuiSprite::getLocalBounds(float * x, float * y, float * w, float * h) const
{
    assert(exists());
    auto const BOUNDS = _impl->getLocalBounds();
    // clang-format off
    if (x) { *x = BOUNDS.left; }
    if (y) { *y = BOUNDS.top; }
    if (w) { *w = BOUNDS.width; }
    if (h) { *h = BOUNDS.height; }
    // clang-format on
}

void GuiSprite::getGlobalBounds(float * x, float * y, float * w, float * h) const
{
    assert(exists());
    auto const BOUNDS = _impl->getGlobalBounds();
    // clang-format off
    if (x) { *x = BOUNDS.left; }
    if (y) { *y = BOUNDS.top; }
    if (w) { *w = BOUNDS.width; }
    if (h) { *h = BOUNDS.height; }
    // clang-format on
}

void GuiSprite::setPosition(float x, float y)
{
    assert(exists());
    _impl->setPosition(x, y);
}

void GuiSprite::setRotation(float angle)
{
    assert(exists());
    _impl->setRotation(angle);
}

void GuiSprite::setScale(float factor_x, float factor_y)
{
    assert(exists());
    _impl->setScale(factor_x, factor_y);
}

void GuiSprite::setOrigin(float x, float y)
{
    assert(exists());
    _impl->setOrigin(x, y);
}

void GuiSprite::getPosition(float * x, float * y) const
{
    assert(exists());
    auto const & POS = _impl->getPosition();
    // clang-format off
    if (x) { *x = POS.x; }
    if (y) { *y = POS.y; }
    // clang-format on
}

void GuiSprite::getRotation(float * angle) const
{
    assert(exists());
    if (angle) {
        *angle = _impl->getRotation();
    }
}

void GuiSprite::getScale(float * x, float * y) const
{
    assert(exists());
    auto const & SCALE = _impl->getScale();
    // clang-format off
    if (x) { *x = SCALE.x; }
    if (y) { *y = SCALE.y; }
    // clang-format on
}

void GuiSprite::getOrigin(float * x, float * y) const
{
    assert(exists());
    auto const & ORIGIN = _impl->getOrigin();
    // clang-format off
    if (x) { *x = ORIGIN.x; }
    if (y) { *y = ORIGIN.y; }
    // clang-format on
}

void GuiSprite::move(float offset_x, float offset_y)
{
    assert(exists());
    _impl->move(offset_x, offset_y);
}

void GuiSprite::rotate(float angle)
{
    assert(exists());
    _impl->rotate(angle);
}

void GuiSprite::scale(float factor_x, float factor_y)
{
    assert(exists());
    _impl->scale(factor_x, factor_y);
}

void GuiSprite::setTexture(GuiTexture const & texture, bool reset_rect)
{
    assert(exists());
    _impl->setTexture(texture, reset_rect);
}

void GuiSprite::setTextureRect(int x, int y, int w, int h)
{
    assert(exists());
    _impl->setTextureRect(sf::IntRect(x, y, w, h));
}

void GuiSprite::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->setColor(sf::Color(r, g, b, a));
}

GuiTexture const & GuiSprite::getTexture() const
{
    assert(exists());
    return _impl->getTexture();
}

void GuiSprite::getTextureRect(int * x, int * y, int * w, int * h) const
{
    assert(exists());
    auto const RECT = _impl->getTextureRect();
    // clang-format off
    if (x) { *x = RECT.left; }
    if (y) { *y = RECT.top; }
    if (w) { *w = RECT.width; }
    if (h) { *h = RECT.height; }
    // clang-format on
}

void GuiSprite::getColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const
{
    assert(exists());
    auto const COLOR = _impl->getColor();
    // clang-format off
    if (r) { *r = COLOR.r; }
    if (g) { *g = COLOR.g; }
    if (b) { *b = COLOR.b; }
    if (a) { *a = COLOR.a; }
    // clang-format on
}

Rect2f GuiSprite::getLocalBounds() const
{
    Rect2f rect;
    getLocalBounds(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

Rect2f GuiSprite::getGlobalBounds() const
{
    Rect2f rect;
    getGlobalBounds(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

void GuiSprite::setPosition(Point2f const & pos)
{
    setPosition(pos.x, pos.y);
}

void GuiSprite::setScale(Point2f const & factor)
{
    setScale(factor.x, factor.y);
}

void GuiSprite::setOrigin(Point2f const & origin)
{
    setScale(origin.x, origin.y);
}

Point2f GuiSprite::getPosition() const
{
    Point2f pos;
    getPosition(&pos.x, &pos.y);
    return pos;
}

Point2f GuiSprite::getScale() const
{
    Point2f scale;
    getScale(&scale.x, &scale.y);
    return scale;
}

Point2f GuiSprite::getOrigin() const
{
    Point2f origin;
    getOrigin(&origin.x, &origin.y);
    return origin;
}

void GuiSprite::move(Point2f const & offset)
{
    move(offset.x, offset.y);
}

void GuiSprite::scale(Point2f const & factor)
{
    scale(factor.x, factor.y);
}

void GuiSprite::setTextureRect(Rect2i const & rect)
{
    setTextureRect(rect.x, rect.y, rect.width, rect.height);
}

void GuiSprite::setColor(Rgb32 const & color)
{
    setColor(color.r, color.g, color.b, color.a);
}

Rect2i GuiSprite::getTextureRect() const
{
    Rect2i rect;
    getTextureRect(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

Rgb32 GuiSprite::getColor() const
{
    Rgb32 color;
    getColor(&color.r, &color.g, &color.b, &color.a);
    return color;
}

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

