/**
 * @file   Vertex.cpp
 * @brief  Vertex class implementation.
 * @author zer0
 * @date   2019-02-23
 */

#include <libtbag/gui/Vertex.hpp>
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
#define _self_sf() Pointer::cast<sf::Vertex>()
#endif

using Rgb32  = Vertex::Rgb32;
using Pointf = Vertex::Pointf;

Vertex::Vertex() : SfNative(SfType::ST_VERTEX)
{
    assert(ptr != nullptr);
}

Vertex::Vertex(Pointf const & position)
        : SfNative(SfType::ST_VERTEX, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::Vertex(sf::Vector2f(position.x, position.y));
    assert(ptr != nullptr);
}

Vertex::Vertex(Pointf const & position, Rgb32 const & color)
        : SfNative(SfType::ST_VERTEX, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::Vertex(sf::Vector2f(position.x, position.y), sf::Color(color.r, color.g, color.b, color.a));
    assert(ptr != nullptr);
}

Vertex::Vertex(Pointf const & position, Pointf const & tex_coords)
        : SfNative(SfType::ST_VERTEX, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::Vertex(sf::Vector2f(position.x, position.y), sf::Vector2f(tex_coords.x, tex_coords.y));
    assert(ptr != nullptr);
}

Vertex::Vertex(Pointf const & position, Rgb32 const & color, Pointf const & tex_coords)
        : SfNative(SfType::ST_VERTEX, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::Vertex(sf::Vector2f(position.x, position.y),
                         sf::Color(color.r, color.g, color.b, color.a),
                         sf::Vector2f(tex_coords.x, tex_coords.y));
    assert(ptr != nullptr);
}

Vertex::Vertex(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_VERTEX, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

Vertex::Vertex(Vertex const & obj)
        : SfNative(SfType::ST_VERTEX)
{
    assert(ptr != nullptr);
    *this = obj;
}

Vertex::Vertex(Vertex && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_VERTEX, no_init)
{
    *this = std::move(obj);
}

Vertex::~Vertex()
{
    // EMPTY.
}

Vertex & Vertex::operator =(Vertex const & obj)
{
    if (this != &obj) {
        setPosition(obj.getPosition());
        setColor(obj.getColor());
        setTexCoords(obj.getTexCoords());
    }
    return *this;
}

Vertex & Vertex::operator =(Vertex && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Vertex::swap(Vertex & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

Pointf Vertex::getPosition() const
{
    auto const POINT = _self_sf()->position;
    return Pointf(POINT.x, POINT.y);
}

Rgb32 Vertex::getColor() const
{
    auto const COLOR = _self_sf()->color;
    return Rgb32(COLOR.r, COLOR.g, COLOR.b, COLOR.a);
}

Pointf Vertex::getTexCoords() const
{
    auto const POINT = _self_sf()->texCoords;
    return Pointf(POINT.x, POINT.y);
}

void Vertex::setPosition(Pointf const & position) const
{
    _self_sf()->position = sf::Vector2f(position.x, position.y);
}

void Vertex::setColor(Rgb32 const & color) const
{
    _self_sf()->color = sf::Color(color.r, color.g, color.b, color.a);
}

void Vertex::setTexCoords(Pointf const & tex_coords) const
{
    _self_sf()->texCoords = sf::Vector2f(tex_coords.x, tex_coords.y);
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

