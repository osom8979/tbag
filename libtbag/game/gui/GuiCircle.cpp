/**
 * @file   GuiCircle.cpp
 * @brief  GuiCircle class implementation.
 * @author zer0
 * @date   2019-04-07
 */

#include <libtbag/game/gui/GuiCircle.hpp>
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
 * GuiCircle::Impl class implementation.
 *
 * @author zer0
 * @date   2019-04-07
 */
struct GuiCircle::Impl : public sf::CircleShape
{
private:
    GuiCircle * _parent;

public:
    Impl(GuiCircle * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// ------------------------
// GuiCircle implementation
// ------------------------

// clang-format off
using Size2f  = GuiCircle::Size2f;
using Point2f = GuiCircle::Point2f;
using Rgb32   = GuiCircle::Rgb32;
using Rect2f  = GuiCircle::Rect2f;
// clang-format on

GuiCircle::GuiCircle() : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

GuiCircle::GuiCircle(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

GuiCircle::GuiCircle(GuiCircle const & obj) TBAG_NOEXCEPT : GuiCircle(nullptr)
{
    (*this) = obj;
}

GuiCircle::GuiCircle(GuiCircle && obj) TBAG_NOEXCEPT : GuiCircle(nullptr)
{
    (*this) = std::move(obj);
}

GuiCircle::~GuiCircle()
{
    // EMPTY.
}

GuiCircle & GuiCircle::operator =(GuiCircle const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

GuiCircle & GuiCircle::operator =(GuiCircle && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void GuiCircle::copy(GuiCircle const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void GuiCircle::swap(GuiCircle & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void GuiCircle::reset()
{
    _impl.reset();
}

void GuiCircle::setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->setFillColor(sf::Color(r, g, b, a));
}

void GuiCircle::setOutlineColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(exists());
    _impl->setOutlineColor(sf::Color(r, g, b, a));
}

void GuiCircle::setOutlineThickness(float thickness)
{
    assert(exists());
    _impl->setOutlineThickness(thickness);
}

void GuiCircle::getFillColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const
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

void GuiCircle::getOutlineColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const
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

void GuiCircle::getOutlineThickness(float * thickness) const
{
    assert(exists());
    if (thickness) {
        *thickness = _impl->getOutlineThickness();
    }
}

void GuiCircle::getLocalBounds(float * x, float * y, float * w, float * h) const
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

void GuiCircle::getGlobalBounds(float * x, float * y, float * w, float * h) const
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

void GuiCircle::setPosition(float x, float y)
{
    assert(exists());
    _impl->setPosition(x, y);
}

void GuiCircle::setRotation(float angle)
{
    assert(exists());
    _impl->setRotation(angle);
}

void GuiCircle::setScale(float factor_x, float factor_y)
{
    assert(exists());
    _impl->setScale(factor_x, factor_y);
}

void GuiCircle::setOrigin(float x, float y)
{
    assert(exists());
    _impl->setOrigin(x, y);
}

void GuiCircle::getPosition(float * x, float * y) const
{
    assert(exists());
    auto const & POS = _impl->getPosition();
    // clang-format off
    if (x) { *x = POS.x; }
    if (y) { *y = POS.y; }
    // clang-format on
}

void GuiCircle::getRotation(float * angle) const
{
    assert(exists());
    if (angle) {
        *angle = _impl->getRotation();
    }
}

void GuiCircle::getScale(float * x, float * y) const
{
    assert(exists());
    auto const & SCALE = _impl->getScale();
    // clang-format off
    if (x) { *x = SCALE.x; }
    if (y) { *y = SCALE.y; }
    // clang-format on
}

void GuiCircle::getOrigin(float * x, float * y) const
{
    assert(exists());
    auto const & ORIGIN = _impl->getOrigin();
    // clang-format off
    if (x) { *x = ORIGIN.x; }
    if (y) { *y = ORIGIN.y; }
    // clang-format on
}

void GuiCircle::move(float offset_x, float offset_y)
{
    assert(exists());
    _impl->move(offset_x, offset_y);
}

void GuiCircle::rotate(float angle)
{
    assert(exists());
    _impl->rotate(angle);
}

void GuiCircle::scale(float factor_x, float factor_y)
{
    assert(exists());
    _impl->scale(factor_x, factor_y);
}

void GuiCircle::setRadius(float radius)
{
    assert(exists());
    _impl->setRadius(radius);
}

float GuiCircle::getRadius() const
{
    assert(exists());
    return _impl->getRadius();
}

void GuiCircle::setPointCount(std::size_t count)
{
    assert(exists());
    _impl->setPointCount(count);
}

std::size_t GuiCircle::getPointCount() const
{
    assert(exists());
    return _impl->getPointCount();
}

void GuiCircle::setFillColor(Rgb32 const & color)
{
    setFillColor(color.r, color.g, color.b, color.a);
}

void GuiCircle::setOutlineColor(Rgb32 const & color)
{
    setOutlineColor(color.r, color.g, color.b, color.a);
}

Rgb32 GuiCircle::getFillColor() const
{
    Rgb32 color;
    getFillColor(&color.r, &color.g, &color.b, &color.a);
    return color;
}

Rgb32 GuiCircle::getOutlineColor() const
{
    Rgb32 color;
    getOutlineColor(&color.r, &color.g, &color.b, &color.a);
    return color;
}

Rect2f GuiCircle::getLocalBounds() const
{
    Rect2f rect;
    getLocalBounds(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

Rect2f GuiCircle::getGlobalBounds() const
{
    Rect2f rect;
    getGlobalBounds(&rect.x, &rect.y, &rect.width, &rect.height);
    return rect;
}

void GuiCircle::setPosition(Point2f const & pos)
{
    setPosition(pos.x, pos.y);
}

void GuiCircle::setScale(Point2f const & factor)
{
    setScale(factor.x, factor.y);
}

void GuiCircle::setOrigin(Point2f const & origin)
{
    setScale(origin.x, origin.y);
}

Point2f GuiCircle::getPosition() const
{
    Point2f pos;
    getPosition(&pos.x, &pos.y);
    return pos;
}

Point2f GuiCircle::getScale() const
{
    Point2f scale;
    getScale(&scale.x, &scale.y);
    return scale;
}

Point2f GuiCircle::getOrigin() const
{
    Point2f origin;
    getOrigin(&origin.x, &origin.y);
    return origin;
}

void GuiCircle::move(Point2f const & offset)
{
    move(offset.x, offset.y);
}

void GuiCircle::scale(Point2f const & factor)
{
    scale(factor.x, factor.y);
}

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

