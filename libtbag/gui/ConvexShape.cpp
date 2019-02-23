/**
 * @file   ConvexShape.cpp
 * @brief  ConvexShape class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/ConvexShape.hpp>
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
#define _self_sf() Pointer::cast<sf::ConvexShape>()
#endif

using Pointf = ConvexShape::Pointf;

ConvexShape::ConvexShape(std::size_t point_count)
        : Shape(SfType::ST_CONVEX_SHAPE, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::ConvexShape(point_count);
    assert(ptr != nullptr);
}

ConvexShape::ConvexShape(void * handle, no_init_no_ref_t)
        : Shape(SfType::ST_CONVEX_SHAPE, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

ConvexShape::ConvexShape(ConvexShape && obj) TBAG_NOEXCEPT
        : Shape(SfType::ST_CONVEX_SHAPE, no_init_no_ref)
{
    *this = std::move(obj);
}

ConvexShape::~ConvexShape()
{
    // EMPTY.
}

ConvexShape & ConvexShape::operator =(ConvexShape && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void ConvexShape::swap(ConvexShape & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

void ConvexShape::setPointCount(std::size_t count)
{
    _self_sf()->setPointCount(count);
}

void ConvexShape::setPoint(std::size_t index, Pointf const & point)
{
    _self_sf()->setPoint(index, sf::Vector2f(point.x, point.y));
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

