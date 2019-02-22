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
#include <libtbag/Noncopyable.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

// Forward declaration.

/**
 * ConvexShape class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API ConvexShape : private Noncopyable
{
private:
    // Insert member variables.

public:
    ConvexShape();
    virtual ~ConvexShape();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_CONVEXSHAPE_HPP__

