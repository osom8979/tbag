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
    CircleShape();
    CircleShape(CircleShape && obj) TBAG_NOEXCEPT;
    virtual ~CircleShape();

public:
    CircleShape & operator =(CircleShape && obj) TBAG_NOEXCEPT;

public:
    void swap(CircleShape & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(CircleShape & lh, CircleShape & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_CIRCLESHAPE_HPP__

