/**
 * @file   Canvas.hpp
 * @brief  Canvas class prototype.
 * @author zer0
 * @date   2020-01-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_CANVAS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_CANVAS_HPP__

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

namespace graphic {

// Forward declaration.

/**
 * Canvas class prototype.
 *
 * @author zer0
 * @date   2020-01-11
 */
class TBAG_API Canvas : private Noncopyable
{
private:
    // Insert member variables.

public:
    Canvas();
    virtual ~Canvas();
};

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_CANVAS_HPP__

