/**
 * @file   RectangleShape.cpp
 * @brief  RectangleShape class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/RectangleShape.hpp>
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
#define _self_sf() Pointer::cast<sf::RectangleShape>()
#endif

using Sizef  = RectangleShape::Sizef;
using Pointf = RectangleShape::Pointf;

RectangleShape::RectangleShape(Sizef const & size)
        : Shape(SfType::ST_RECTANGLE_SHAPE, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::RectangleShape(sf::Vector2f(size.width, size.height));
    assert(ptr != nullptr);
}

RectangleShape::RectangleShape(void * handle, no_init_no_ref_t)
        : Shape(SfType::ST_RECTANGLE_SHAPE, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

RectangleShape::RectangleShape(RectangleShape && obj) TBAG_NOEXCEPT
        : Shape(SfType::ST_RECTANGLE_SHAPE, no_init_no_ref)
{
    *this = std::move(obj);
}

RectangleShape::~RectangleShape()
{
    // EMPTY.
}

RectangleShape & RectangleShape::operator =(RectangleShape && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void RectangleShape::swap(RectangleShape & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

void RectangleShape::setSize(Sizef const & size)
{
    _self_sf()->setSize(sf::Vector2f(size.width, size.height));
}

Sizef RectangleShape::getSize() const
{
    auto const SIZE = _self_sf()->getSize();
    return Sizef(SIZE.x, SIZE.y);
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

