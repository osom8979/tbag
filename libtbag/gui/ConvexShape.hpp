/**
 * @file   ConvexShape.hpp
 * @brief  ConvexShape class prototype.
 * @author zer0
 * @date   2019-02-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_CONVEXSHAPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_CONVEXSHAPE_HPP__

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
 * ConvexShape class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API ConvexShape : public Shape
{
public:
    ConvexShape(std::size_t pointCount = 0);
    ConvexShape(void * handle, no_init_no_ref_t);
    ConvexShape(ConvexShape && obj) TBAG_NOEXCEPT;
    virtual ~ConvexShape();

public:
    ConvexShape & operator =(ConvexShape && obj) TBAG_NOEXCEPT;

public:
    void swap(ConvexShape & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(ConvexShape & lh, ConvexShape & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    void setPointCount(std::size_t count);
    void setPoint(std::size_t index, Pointf const & point);
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_CONVEXSHAPE_HPP__

