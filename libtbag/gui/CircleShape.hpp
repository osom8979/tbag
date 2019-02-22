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
#include <libtbag/Noncopyable.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

// Forward declaration.

/**
 * CircleShape class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API CircleShape : private Noncopyable
{
private:
    // Insert member variables.

public:
    CircleShape();
    virtual ~CircleShape();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_CIRCLESHAPE_HPP__

