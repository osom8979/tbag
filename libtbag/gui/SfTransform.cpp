/**
 * @file   SfTransform.cpp
 * @brief  SfTransform class implementation.
 * @author zer0
 * @date   2019-02-17
 */

#include <libtbag/gui/SfTransform.hpp>
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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

SfTransform::SfTransform() : SfNative(SfType::ST_TRANSFORM)
{
    // EMPTY.
}

SfTransform::SfTransform(float a00, float a01, float a02,
                         float a10, float a11, float a12,
                         float a20, float a21, float a22)
        : SfNative(SfType::ST_TRANSFORM, no_init)
{
    assert(ptr == nullptr);
    ptr = new sf::Transform(a00 ,a01, a02, a10, a11, a12, a20, a21, a22);
    assert(ptr != nullptr);
}

SfTransform::SfTransform(float const * matrix) : SfTransform()
{
    assert(matrix != nullptr);
    memcpy(getMutableMatrix(), matrix, MATRIX_BYTE_SIZE);
    assert(ptr != nullptr);
}

SfTransform::SfTransform(SfTransform && obj) TBAG_NOEXCEPT
        : SfNative(SfType::ST_TRANSFORM, no_init)
{
    if (this != &obj) {
        std::swap(ptr, obj.ptr);
    }
}

SfTransform & SfTransform::operator =(SfTransform && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(ptr, obj.ptr);
    }
    return *this;
}

void SfTransform::swap(SfTransform & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(ptr, obj.ptr);
    }
}

SfTransform::~SfTransform()
{
    // EMPTY.
}

#ifndef _self_sf
#define _self_sf() Parent::cast<sf::Transform>()
#endif

using Pointf = SfTransform::Pointf;
using Rectf  = SfTransform::Rectf;

float * SfTransform::getMutableMatrix()
{
    return const_cast<float*>(getMatrix());
}

float const * SfTransform::getMatrix() const
{
    return _self_sf()->getMatrix();
}

SfTransform SfTransform::getInverse() const
{
    return SfTransform(_self_sf()->getInverse().getMatrix());
}

Pointf SfTransform::transformPoint(float x, float y) const
{
    auto const POINT = _self_sf()->transformPoint(x, y);
    return Pointf(POINT.x, POINT.y);
}

Pointf SfTransform::transformPoint(Pointf const & point) const
{
    auto const POINT = _self_sf()->transformPoint(point.x, point.y);
    return Pointf(POINT.x, POINT.y);
}

Rectf SfTransform::transformRect(Rectf const & r) const
{
    auto const RECT = _self_sf()->transformRect(sf::FloatRect(r.x, r.y, r.width, r.height));
    return Rectf(RECT.left, RECT.top, RECT.width, RECT.height);
}

SfTransform & SfTransform::combine(SfTransform const & transform)
{
    sf::Transform t;
    memcpy(const_cast<float*>(t.getMatrix()), transform.getMatrix(), MATRIX_BYTE_SIZE);
    _self_sf()->combine(t);
    return *this;
}

SfTransform & SfTransform::translate(float x, float y)
{
    _self_sf()->translate(x, y);
    return *this;
}

SfTransform & SfTransform::translate(Pointf const & offset)
{
    _self_sf()->translate(offset.x, offset.y);
    return *this;
}

SfTransform & SfTransform::rotate(float angle)
{
    _self_sf()->rotate(angle);
    return *this;
}

SfTransform & SfTransform::rotate(float angle, float center_x, float center_y)
{
    _self_sf()->rotate(angle, center_x, center_y);
    return *this;
}

SfTransform & SfTransform::rotate(float angle, Pointf const & center)
{
    _self_sf()->rotate(angle, center.x, center.y);
    return *this;
}

SfTransform & SfTransform::scale(float scale_x, float scale_y)
{
    _self_sf()->scale(scale_x, scale_y);
    return *this;
}

SfTransform & SfTransform::scale(float scale_x, float scale_y, float center_x, float center_y)
{
    _self_sf()->scale(scale_x, scale_y, center_x, center_y);
    return *this;
}

SfTransform & SfTransform::scale(Pointf const & factors)
{
    _self_sf()->scale(factors.x, factors.y);
    return *this;
}

SfTransform & SfTransform::scale(Pointf const & factors, Pointf const & center)
{
    _self_sf()->scale(factors.x, factors.y, center.x, center.y);
    return *this;
}

#undef _self_sf

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

