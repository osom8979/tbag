/**
 * @file   Transform.hpp
 * @brief  Transform class prototype.
 * @author zer0
 * @date   2019-02-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_TRANSFORM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_TRANSFORM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gui/SfNative.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * Transform class prototype.
 *
 * @author zer0
 * @date   2019-02-17
 */
class TBAG_API Transform : public SfNative
{
public:
    TBAG_CONSTEXPR static std::size_t const TRANSFORM_MATRIX_SIZE = 16;
    TBAG_CONSTEXPR static std::size_t const MATRIX_BYTE_SIZE = sizeof(float) * TRANSFORM_MATRIX_SIZE;

public:
    Transform();
    Transform(float a00, float a01, float a02,
                float a10, float a11, float a12,
                float a20, float a21, float a22);
    Transform(float const * matrix);
    Transform(Transform && obj) TBAG_NOEXCEPT;
    ~Transform();

public:
    Transform & operator =(Transform && obj) TBAG_NOEXCEPT;

public:
    void swap(Transform & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Transform & lh, Transform & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

private:
    float * getMutableMatrix();

public:
    float const * getMatrix() const;
    Transform getInverse() const;

public:
    Pointf transformPoint(float x, float y) const;
    Pointf transformPoint(Pointf const & point) const;
    Rectf  transformRect(Rectf const & rectangle) const;

public:
    Transform & combine(Transform const & transform);

    Transform & translate(float x, float y);
    Transform & translate(Pointf const & offset);

    Transform & rotate(float angle);
    Transform & rotate(float angle, float center_x, float center_y);
    Transform & rotate(float angle, Pointf const & center);

    Transform & scale(float scale_x, float scale_y);
    Transform & scale(float scale_x, float scale_y, float center_x, float center_y);
    Transform & scale(Pointf const & factors);
    Transform & scale(Pointf const & factors, Pointf const & center);
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_TRANSFORM_HPP__

