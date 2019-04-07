/**
 * @file   GuiConvex.cpp
 * @brief  GuiConvex class implementation.
 * @author zer0
 * @date   2019-04-07
 */

#include <libtbag/game/gui/GuiConvex.hpp>
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
 * GuiConvex::Impl class implementation.
 *
 * @author zer0
 * @date   2019-04-07
 */
struct GuiConvex::Impl : public sf::ConvexShape
{
private:
    GuiConvex * _parent;

public:
    Impl(GuiConvex * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ------------------------
// GuiConvex implementation
// ------------------------

// clang-format off
using Size2f  = GuiConvex::Size2f;
using Point2f = GuiConvex::Point2f;
using Rgb32   = GuiConvex::Rgb32;
using Rect2f  = GuiConvex::Rect2f;
// clang-format on

GuiConvex::GuiConvex() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

GuiConvex::GuiConvex(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

GuiConvex::GuiConvex(GuiConvex const & obj) TBAG_NOEXCEPT : GuiConvex(nullptr)
{
    (*this) = obj;
}

GuiConvex::GuiConvex(GuiConvex && obj) TBAG_NOEXCEPT : GuiConvex(nullptr)
{
    (*this) = std::move(obj);
}

GuiConvex::~GuiConvex()
{
    // EMPTY.
}

GuiConvex & GuiConvex::operator =(GuiConvex const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

GuiConvex & GuiConvex::operator =(GuiConvex && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void GuiConvex::copy(GuiConvex const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void GuiConvex::swap(GuiConvex & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void GuiConvex::reset()
{
    _impl.reset();
}

void GuiConvex::setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->setFillColor(sf::Color(r, g, b, a));
}

void GuiConvex::setOutlineColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->setOutlineColor(sf::Color(r, g, b, a));
}

void GuiConvex::setOutlineThickness(float thickness)
{
    assert(exists());
    _impl->setOutlineThickness(thickness);
}

void GuiConvex::getFillColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const
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

void GuiConvex::getOutlineColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const
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

void GuiConvex::getOutlineThickness(float * thickness) const
{
    assert(exists());
    if (thickness) {
        *thickness = _impl->getOutlineThickness();
    }
}

void GuiConvex::getLocalBounds(float * x, float * y, float * w, float * h) const
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

void GuiConvex::getGlobalBounds(float * x, float * y, float * w, float * h) const
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

void GuiConvex::setPosition(float x, float y)
{
    assert(exists());
    _impl->setPosition(x, y);
}

void GuiConvex::setRotation(float angle)
{
    assert(exists());
    _impl->setRotation(angle);
}

void GuiConvex::setScale(float factor_x, float factor_y)
{
    assert(exists());
    _impl->setScale(factor_x, factor_y);
}

void GuiConvex::setOrigin(float x, float y)
{
    assert(exists());
    _impl->setOrigin(x, y);
}

void GuiConvex::getPosition(float * x, float * y) const
{
    assert(exists());
    auto const & POS = _impl->getPosition();
    // clang-format off
    if (x) { *x = POS.x; }
    if (y) { *y = POS.y; }
    // clang-format on
}

void GuiConvex::getRotation(float * angle) const
{
    assert(exists());
    if (angle) {
        *angle = _impl->getRotation();
    }
}

void GuiConvex::getScale(float * x, float * y) const
{
    assert(exists());
    auto const & SCALE = _impl->getScale();
    // clang-format off
    if (x) { *x = SCALE.x; }
    if (y) { *y = SCALE.y; }
    // clang-format on
}

void GuiConvex::getOrigin(float * x, float * y) const
{
    assert(exists());
    auto const & ORIGIN = _impl->getOrigin();
    // clang-format off
    if (x) { *x = ORIGIN.x; }
    if (y) { *y = ORIGIN.y; }
    // clang-format on
}

void GuiConvex::move(float offset_x, float offset_y)
{
    assert(exists());
    _impl->move(offset_x, offset_y);
}

void GuiConvex::rotate(float angle)
{
    assert(exists());
    _impl->rotate(angle);
}

void GuiConvex::scale(float factor_x, float factor_y)
{
    assert(exists());
    _impl->scale(factor_x, factor_y);
}

void GuiConvex::setPointCount(std::size_t count)
{
    assert(exists());
    _impl->setPointCount(count);
}

std::size_t GuiConvex::getPointCount() const
{
    assert(exists());
    return _impl->getPointCount();
}

void GuiConvex::setPoint(std::size_t index, float x, float y)
{
    assert(exists());
    _impl->setPoint(index, sf::Vector2f(x, y));
}

void GuiConvex::getPoint(std::size_t index, float * x, float * y) const
{
    assert(exists());
    auto const POS = _impl->getPoint(index);
    // clang-format off
    if (x) { *x = POS.x; }
    if (y) { *y = POS.y; }
    // clang-format on
}

void GuiConvex::setFillColor(Rgb32 const & color)
{
    setFillColor(color.r, color.g, color.b, color.a);
}

void GuiConvex::setOutlineColor(Rgb32 const & color)
{
    setOutlineColor(color.r, color.g, color.b, color.a);
}

Rgb32 GuiConvex::getFillColor() const
{
    Rgb32 color;
    getFillColor(&color.r, &color.g, &color.b, &color.a);
    return color;
}

Rgb32 GuiConvex::getOutlineColor() const
{
    Rgb32 color;
    getOutlineColor(&color.r, &color.g, &color.b, &color.a);
    return color;
}

Rect2f GuiConvex::getLocalBounds() const
{
    Rect2f rect;
    getLocalBounds(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

Rect2f GuiConvex::getGlobalBounds() const
{
    Rect2f rect;
    getGlobalBounds(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

void GuiConvex::setPosition(Point2f const & pos)
{
    setPosition(pos.x, pos.y);
}

void GuiConvex::setScale(Point2f const & factor)
{
    setScale(factor.x, factor.y);
}

void GuiConvex::setOrigin(Point2f const & origin)
{
    setScale(origin.x, origin.y);
}

Point2f GuiConvex::getPosition() const
{
    Point2f pos;
    getPosition(&pos.x, &pos.y);
    return pos;
}

Point2f GuiConvex::getScale() const
{
    Point2f scale;
    getScale(&scale.x, &scale.y);
    return scale;
}

Point2f GuiConvex::getOrigin() const
{
    Point2f origin;
    getOrigin(&origin.x, &origin.y);
    return origin;
}

void GuiConvex::move(Point2f const & offset)
{
    move(offset.x, offset.y);
}

void GuiConvex::scale(Point2f const & factor)
{
    scale(factor.x, factor.y);
}

void GuiConvex::setPoint(std::size_t index, Point2f const & pos)
{
    setPoint(index, pos.x, pos.y);
}

Point2f GuiConvex::getPoint(std::size_t index) const
{
    Point2f pos;
    getPoint(index, &pos.x, &pos.y);
    return pos;
}

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

