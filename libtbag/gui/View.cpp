/**
 * @file   View.cpp
 * @brief  View class implementation.
 * @author zer0
 * @date   2019-02-17
 */

#include <libtbag/gui/View.hpp>
#include <libtbag/gui/Transform.hpp>
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
#define _self_sf() Pointer::cast<sf::View>()
#endif

using Pointf = View::Pointf;
using Sizef  = View::Sizef;
using Rectf  = View::Rectf;

View::View() : SfNative(SfType::ST_VIEW)
{
    assert(ptr != nullptr);
}

View::View(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_VIEW, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

View::View(Rectf const & r)
        : SfNative(SfType::ST_VIEW, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::View(sf::FloatRect(r.x, r.y, r.width, r.height));
    assert(ptr != nullptr);
}

View::View(Pointf const & center, Sizef const & size)
        : SfNative(SfType::ST_VIEW, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::View(sf::Vector2f(center.x, center.y), sf::Vector2f(size.width, size.height));
    assert(ptr != nullptr);
}

View::View(View && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_VIEW, no_init)
{
    *this = std::move(obj);
}

View::~View()
{
    // EMPTY.
}

View & View::operator =(View && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void View::swap(View & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

void View::setCenter(float x, float y)
{
    _self_sf()->setCenter(x, y);
}

void View::setCenter(Pointf const & center)
{
    _self_sf()->setCenter(center.x, center.y);
}

void View::setSize(float width, float height)
{
    _self_sf()->setSize(width, height);
}

void View::setSize(Sizef const & size)
{
    _self_sf()->setSize(size.width, size.height);
}

void View::setRotation(float angle)
{
    _self_sf()->setRotation(angle);
}

void View::setViewport(Rectf const & viewport)
{
    _self_sf()->setViewport(sf::FloatRect(viewport.x, viewport.y, viewport.width, viewport.height));
}

void View::reset(Rectf const & rectangle)
{
    _self_sf()->reset(sf::FloatRect(rectangle.x, rectangle.y, rectangle.width, rectangle.height));
}

Pointf View::getCenter() const
{
    auto const CENTER = _self_sf()->getCenter();
    return Pointf(CENTER.x, CENTER.y);
}

Sizef View::getSize() const
{
    auto const SIZE = _self_sf()->getSize();
    return Sizef(SIZE.x, SIZE.y);
}

float View::getRotation() const
{
    return _self_sf()->getRotation();
}

Rectf View::getViewport() const
{
    auto const VIEWPORT = _self_sf()->getViewport();
    return Rectf(VIEWPORT.left, VIEWPORT.top, VIEWPORT.width, VIEWPORT.height);
}

void View::move(float offset_x, float offset_y)
{
    _self_sf()->move(offset_x, offset_y);
}

void View::move(Pointf const & offset)
{
    _self_sf()->move(offset.x, offset.y);
}

void View::rotate(float angle)
{
    _self_sf()->rotate(angle);
}

void View::zoom(float factor)
{
    _self_sf()->zoom(factor);
}

Transform View::getTransform() const
{
    return Transform(_self_sf()->getTransform().getMatrix());
}

Transform View::getInverseTransform() const
{
    return Transform(_self_sf()->getInverseTransform().getMatrix());
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

