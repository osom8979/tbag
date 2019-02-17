/**
 * @file   SfView.cpp
 * @brief  SfView class implementation.
 * @author zer0
 * @date   2019-02-17
 */

#include <libtbag/sfpp/SfView.hpp>
#include <libtbag/sfpp/SfTransform.hpp>
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

namespace sfpp {

using Pointf = SfView::Pointf;
using Sizef  = SfView::Sizef;
using Rectf  = SfView::Rectf;

SfView::SfView() : SfNative(SfType::ST_VIEW)
{
    // EMPTY.
}

SfView::SfView(void * handle, no_init_t, bool ref)
        : SfNative(SfType::ST_VIEW, no_init)
{
    ptr = handle;
    assert(ptr != nullptr);
}

SfView::SfView(Rectf const & r)
        : SfNative(SfType::ST_VIEW, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::View(sf::FloatRect(r.x, r.y, r.width, r.height));
    assert(ptr != nullptr);
}

SfView::SfView(Pointf const & center, Sizef const & size)
        : SfNative(SfType::ST_VIEW, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::View(sf::Vector2f(center.x, center.y), sf::Vector2f(size.width, size.height));
    assert(ptr != nullptr);
}

SfView::SfView(SfView && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_VIEW, no_init)
{
    *this = std::move(obj);
}

SfView::~SfView()
{
    // EMPTY.
}

SfView & SfView::operator =(SfView && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void SfView::swap(SfView & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

#ifndef _self_sf
#define _self_sf() Parent::cast<sf::View>()
#endif

void SfView::setCenter(float x, float y)
{
    _self_sf()->setCenter(x, y);
}

void SfView::setCenter(Pointf const & center)
{
    _self_sf()->setCenter(center.x, center.y);
}

void SfView::setSize(float width, float height)
{
    _self_sf()->setSize(width, height);
}

void SfView::setSize(Sizef const & size)
{
    _self_sf()->setSize(size.width, size.height);
}

void SfView::setRotation(float angle)
{
    _self_sf()->setRotation(angle);
}

void SfView::setViewport(Rectf const & viewport)
{
    _self_sf()->setViewport(sf::FloatRect(viewport.x, viewport.y, viewport.width, viewport.height));
}

void SfView::reset(Rectf const & rectangle)
{
    _self_sf()->reset(sf::FloatRect(rectangle.x, rectangle.y, rectangle.width, rectangle.height));
}

Pointf SfView::getCenter() const
{
    auto const CENTER = _self_sf()->getCenter();
    return Pointf(CENTER.x, CENTER.y);
}

Sizef SfView::getSize() const
{
    auto const SIZE = _self_sf()->getSize();
    return Sizef(SIZE.x, SIZE.y);
}

float SfView::getRotation() const
{
    return _self_sf()->getRotation();
}

Rectf SfView::getViewport() const
{
    auto const VIEWPORT = _self_sf()->getViewport();
    return Rectf(VIEWPORT.left, VIEWPORT.top, VIEWPORT.width, VIEWPORT.height);
}

void SfView::move(float offset_x, float offset_y)
{
    _self_sf()->move(offset_x, offset_y);
}

void SfView::move(Pointf const & offset)
{
    _self_sf()->move(offset.x, offset.y);
}

void SfView::rotate(float angle)
{
    _self_sf()->rotate(angle);
}

void SfView::zoom(float factor)
{
    _self_sf()->zoom(factor);
}

SfTransform SfView::getTransform() const
{
    return SfTransform(_self_sf()->getTransform().getMatrix());
}

SfTransform SfView::getInverseTransform() const
{
    return SfTransform(_self_sf()->getInverseTransform().getMatrix());
}

#undef _self_sf

} // namespace sfpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

