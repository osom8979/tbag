/**
 * @file   GuiRect.cpp
 * @brief  GuiRect class implementation.
 * @author zer0
 * @date   2019-04-07
 */

#include <libtbag/game/gui/GuiRect.hpp>
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
 * GuiRect::Impl class implementation.
 *
 * @author zer0
 * @date   2019-04-07
 */
struct GuiRect::Impl : public sf::RectangleShape
{
private:
    GuiRect * _parent;

public:
    Impl(GuiRect * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ----------------------
// GuiRect implementation
// ----------------------

// clang-format off
using Size2f  = GuiRect::Size2f;
using Point2f = GuiRect::Point2f;
using Rgb32   = GuiRect::Rgb32;
using Rect2f  = GuiRect::Rect2f;
// clang-format on

GuiRect::GuiRect() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

GuiRect::GuiRect(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

GuiRect::GuiRect(GuiRect const & obj) TBAG_NOEXCEPT : GuiRect(nullptr)
{
    (*this) = obj;
}

GuiRect::GuiRect(GuiRect && obj) TBAG_NOEXCEPT : GuiRect(nullptr)
{
    (*this) = std::move(obj);
}

GuiRect::~GuiRect()
{
    // EMPTY.
}

GuiRect & GuiRect::operator =(GuiRect const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

GuiRect & GuiRect::operator =(GuiRect && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void GuiRect::copy(GuiRect const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void GuiRect::swap(GuiRect & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void GuiRect::reset()
{
    _impl.reset();
}

void GuiRect::setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->setFillColor(sf::Color(r, g, b, a));
}

void GuiRect::setOutlineColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->setOutlineColor(sf::Color(r, g, b, a));
}

void GuiRect::setOutlineThickness(float thickness)
{
    assert(exists());
    _impl->setOutlineThickness(thickness);
}

void GuiRect::getFillColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const
{
    assert(exists());
    auto const COLOR = _impl->getFillColor();
    // clang-format off
    if (r) { *r = COLOR.r; }
    if (g) { *g = COLOR.g; }
    if (b) { *b = COLOR.b; }
    if (a) { *a = COLOR.a; }
    // clang-format on
}

void GuiRect::getOutlineColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const
{
    assert(exists());
    auto const COLOR = _impl->getOutlineColor();
    // clang-format off
    if (r) { *r = COLOR.r; }
    if (g) { *g = COLOR.g; }
    if (b) { *b = COLOR.b; }
    if (a) { *a = COLOR.a; }
    // clang-format on
}

void GuiRect::getOutlineThickness(float * thickness) const
{
    assert(exists());
    if (thickness) {
        *thickness = _impl->getOutlineThickness();
    }
}

void GuiRect::getLocalBounds(float * x, float * y, float * w, float * h) const
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

void GuiRect::getGlobalBounds(float * x, float * y, float * w, float * h) const
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

void GuiRect::setPosition(float x, float y)
{
    assert(exists());
    _impl->setPosition(x, y);
}

void GuiRect::setRotation(float angle)
{
    assert(exists());
    _impl->setRotation(angle);
}

void GuiRect::setScale(float factor_x, float factor_y)
{
    assert(exists());
    _impl->setScale(factor_x, factor_y);
}

void GuiRect::setOrigin(float x, float y)
{
    assert(exists());
    _impl->setOrigin(x, y);
}

void GuiRect::getPosition(float * x, float * y) const
{
    assert(exists());
    auto const & POS = _impl->getPosition();
    // clang-format off
    if (x) { *x = POS.x; }
    if (y) { *y = POS.y; }
    // clang-format on
}

void GuiRect::getRotation(float * angle) const
{
    assert(exists());
    if (angle) {
        *angle = _impl->getRotation();
    }
}

void GuiRect::getScale(float * x, float * y) const
{
    assert(exists());
    auto const & SCALE = _impl->getScale();
    // clang-format off
    if (x) { *x = SCALE.x; }
    if (y) { *y = SCALE.y; }
    // clang-format on
}

void GuiRect::getOrigin(float * x, float * y) const
{
    assert(exists());
    auto const & ORIGIN = _impl->getOrigin();
    // clang-format off
    if (x) { *x = ORIGIN.x; }
    if (y) { *y = ORIGIN.y; }
    // clang-format on
}

void GuiRect::move(float offset_x, float offset_y)
{
    assert(exists());
    _impl->move(offset_x, offset_y);
}

void GuiRect::rotate(float angle)
{
    assert(exists());
    _impl->rotate(angle);
}

void GuiRect::scale(float factor_x, float factor_y)
{
    assert(exists());
    _impl->scale(factor_x, factor_y);
}

void GuiRect::setSize(float w, float h)
{
    assert(exists());
    _impl->setSize(sf::Vector2f(w, h));
}

void GuiRect::getSize(float * w, float * h) const
{
    assert(exists());
    auto const & SIZE = _impl->getSize();
    // clang-format off
    if (w) { *w = SIZE.x; }
    if (h) { *h = SIZE.y; }
    // clang-format on
}

void GuiRect::setFillColor(Rgb32 const & color)
{
    setFillColor(color.r, color.g, color.b, color.a);
}

void GuiRect::setOutlineColor(Rgb32 const & color)
{
    setOutlineColor(color.r, color.g, color.b, color.a);
}

Rgb32 GuiRect::getFillColor() const
{
    Rgb32 color;
    getFillColor(&color.r, &color.g, &color.b, &color.a);
    return color;
}

Rgb32 GuiRect::getOutlineColor() const
{
    Rgb32 color;
    getOutlineColor(&color.r, &color.g, &color.b, &color.a);
    return color;
}

Rect2f GuiRect::getLocalBounds() const
{
    Rect2f rect;
    getLocalBounds(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

Rect2f GuiRect::getGlobalBounds() const
{
    Rect2f rect;
    getGlobalBounds(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

void GuiRect::setPosition(Point2f const & pos)
{
    setPosition(pos.x, pos.y);
}

void GuiRect::setScale(Point2f const & factor)
{
    setScale(factor.x, factor.y);
}

void GuiRect::setOrigin(Point2f const & origin)
{
    setScale(origin.x, origin.y);
}

Point2f GuiRect::getPosition() const
{
    Point2f pos;
    getPosition(&pos.x, &pos.y);
    return pos;
}

Point2f GuiRect::getScale() const
{
    Point2f scale;
    getScale(&scale.x, &scale.y);
    return scale;
}

Point2f GuiRect::getOrigin() const
{
    Point2f origin;
    getOrigin(&origin.x, &origin.y);
    return origin;
}

void GuiRect::move(Point2f const & offset)
{
    move(offset.x, offset.y);
}

void GuiRect::scale(Point2f const & factor)
{
    scale(factor.x, factor.y);
}

void GuiRect::setSize(Size2f const & size)
{
    setSize(size.width, size.height);
}

Size2f GuiRect::getSize() const
{
    Size2f size;
    getSize(&size.width, &size.height);
    return size;
}

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

