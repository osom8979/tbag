/**
 * @file   Transform.cpp
 * @brief  Transform class implementation.
 * @author zer0
 * @date   2019-02-17
 */

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
#include <cstring>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

#ifndef _self_sf
#define _self_sf() Pointer::cast<sf::Transform>()
#endif

using Pointf = Transform::Pointf;
using Rectf  = Transform::Rectf;

Transform::Transform() : SfNative(SfType::ST_TRANSFORM)
{
    assert(ptr != nullptr);
}

Transform::Transform(void * handle, no_init_no_ref_t)
        : SfNative(SfType::ST_TRANSFORM, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

Transform::Transform(float a00, float a01, float a02,
                     float a10, float a11, float a12,
                     float a20, float a21, float a22)
        : SfNative(SfType::ST_TRANSFORM, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::Transform(a00 ,a01, a02, a10, a11, a12, a20, a21, a22);
    assert(ptr != nullptr);
}

Transform::Transform(float const * matrix)
        : SfNative(SfType::ST_TRANSFORM)
{
    assert(matrix != nullptr);
    memcpy(getMutableMatrix(), matrix, MATRIX_BYTE_SIZE);
    assert(ptr != nullptr);
}

Transform::Transform(Transform && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_TRANSFORM, no_init)
{
    *this = std::move(obj);
}

Transform & Transform::operator =(Transform && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Transform::swap(Transform & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

Transform::~Transform()
{
    // EMPTY.
}

float * Transform::getMutableMatrix()
{
    return const_cast<float*>(getMatrix());
}

float const * Transform::getMatrix() const
{
    return _self_sf()->getMatrix();
}

Transform Transform::getInverse() const
{
    return Transform(_self_sf()->getInverse().getMatrix());
}

Pointf Transform::transformPoint(float x, float y) const
{
    auto const POINT = _self_sf()->transformPoint(x, y);
    return Pointf(POINT.x, POINT.y);
}

Pointf Transform::transformPoint(Pointf const & point) const
{
    auto const POINT = _self_sf()->transformPoint(point.x, point.y);
    return Pointf(POINT.x, POINT.y);
}

Rectf Transform::transformRect(Rectf const & r) const
{
    auto const RECT = _self_sf()->transformRect(sf::FloatRect(r.x, r.y, r.width, r.height));
    return Rectf(RECT.left, RECT.top, RECT.width, RECT.height);
}

Transform & Transform::combine(Transform const & transform)
{
    sf::Transform t;
    memcpy(const_cast<float*>(t.getMatrix()), transform.getMatrix(), MATRIX_BYTE_SIZE);
    _self_sf()->combine(t);
    return *this;
}

Transform & Transform::translate(float x, float y)
{
    _self_sf()->translate(x, y);
    return *this;
}

Transform & Transform::translate(Pointf const & offset)
{
    _self_sf()->translate(offset.x, offset.y);
    return *this;
}

Transform & Transform::rotate(float angle)
{
    _self_sf()->rotate(angle);
    return *this;
}

Transform & Transform::rotate(float angle, float center_x, float center_y)
{
    _self_sf()->rotate(angle, center_x, center_y);
    return *this;
}

Transform & Transform::rotate(float angle, Pointf const & center)
{
    _self_sf()->rotate(angle, center.x, center.y);
    return *this;
}

Transform & Transform::scale(float scale_x, float scale_y)
{
    _self_sf()->scale(scale_x, scale_y);
    return *this;
}

Transform & Transform::scale(float scale_x, float scale_y, float center_x, float center_y)
{
    _self_sf()->scale(scale_x, scale_y, center_x, center_y);
    return *this;
}

Transform & Transform::scale(Pointf const & factors)
{
    _self_sf()->scale(factors.x, factors.y);
    return *this;
}

Transform & Transform::scale(Pointf const & factors, Pointf const & center)
{
    _self_sf()->scale(factors.x, factors.y, center.x, center.y);
    return *this;
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

