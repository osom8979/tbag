/**
 * @file   CircleShape.cpp
 * @brief  CircleShape class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/CircleShape.hpp>
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
#define _self_sf() Pointer::cast<sf::CircleShape>()
#endif

using Pointf = CircleShape::Pointf;

CircleShape::CircleShape(float radius, std::size_t point_count)
        : Shape(SfType::ST_CIRCLE_SHAPE, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::CircleShape(radius, point_count);
    assert(ptr != nullptr);
}

CircleShape::CircleShape(void * handle, no_init_no_ref_t)
        : Shape(SfType::ST_CIRCLE_SHAPE, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

CircleShape::CircleShape(CircleShape && obj) TBAG_NOEXCEPT
        : Shape(SfType::ST_CIRCLE_SHAPE, no_init_no_ref)
{
    *this = std::move(obj);
}

CircleShape::~CircleShape()
{
    // EMPTY.
}

CircleShape & CircleShape::operator =(CircleShape && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void CircleShape::swap(CircleShape & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

void CircleShape::setRadius(float radius)
{
    _self_sf()->setRadius(radius);
}

float CircleShape::getRadius() const
{
    return _self_sf()->getRadius();
}

void CircleShape::setPointCount(std::size_t count)
{
    _self_sf()->setPointCount(count);
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

