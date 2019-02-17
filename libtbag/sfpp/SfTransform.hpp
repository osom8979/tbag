/**
 * @file   SfTransform.hpp
 * @brief  SfTransform class prototype.
 * @author zer0
 * @date   2019-02-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFTRANSFORM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFTRANSFORM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/sfpp/SfNative.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace sfpp {

/**
 * SfTransform class prototype.
 *
 * @author zer0
 * @date   2019-02-17
 */
class TBAG_API SfTransform : public SfNative
{
public:
    TBAG_CONSTEXPR static std::size_t const TRANSFORM_MATRIX_SIZE = 16;
    TBAG_CONSTEXPR static std::size_t const MATRIX_BYTE_SIZE = sizeof(float) * TRANSFORM_MATRIX_SIZE;

public:
    SfTransform();
    SfTransform(float a00, float a01, float a02,
                float a10, float a11, float a12,
                float a20, float a21, float a22);
    SfTransform(float const * matrix);
    SfTransform(SfTransform && obj) TBAG_NOEXCEPT;
    virtual ~SfTransform();

public:
    SfTransform & operator =(SfTransform && obj) TBAG_NOEXCEPT;

public:
    void swap(SfTransform & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(SfTransform & lh, SfTransform & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

private:
    float * getMutableMatrix();

public:
    float const * getMatrix() const;
    SfTransform getInverse() const;

public:
    Pointf transformPoint(float x, float y) const;
    Pointf transformPoint(Pointf const & point) const;
    Rectf  transformRect(Rectf const & rectangle) const;

public:
    SfTransform & combine(SfTransform const & transform);

    SfTransform & translate(float x, float y);
    SfTransform & translate(Pointf const & offset);

    SfTransform & rotate(float angle);
    SfTransform & rotate(float angle, float center_x, float center_y);
    SfTransform & rotate(float angle, Pointf const & center);

    SfTransform & scale(float scale_x, float scale_y);
    SfTransform & scale(float scale_x, float scale_y, float center_x, float center_y);
    SfTransform & scale(Pointf const & factors);
    SfTransform & scale(Pointf const & factors, Pointf const & center);
};

} // namespace sfpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SFPP_SFTRANSFORM_HPP__

