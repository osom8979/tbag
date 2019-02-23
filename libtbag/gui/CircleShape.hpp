/**
 * @file   CircleShape.hpp
 * @brief  CircleShape class prototype.
 * @author zer0
 * @date   2019-02-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_CIRCLESHAPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_CIRCLESHAPE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gui/Shape.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * CircleShape class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API CircleShape : public Shape
{
public:
    CircleShape(float radius = 0, std::size_t point_count = 30);
    CircleShape(void * handle, no_init_no_ref_t);
    CircleShape(CircleShape && obj) TBAG_NOEXCEPT;
    virtual ~CircleShape();

public:
    CircleShape & operator =(CircleShape && obj) TBAG_NOEXCEPT;

public:
    void swap(CircleShape & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(CircleShape & lh, CircleShape & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    void setRadius(float radius);
    float getRadius() const;
    void setPointCount(std::size_t count);

public:
    virtual std::size_t getPointCount() const override;
    virtual Pointf getPoint(std::size_t index) const override;
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_CIRCLESHAPE_HPP__

