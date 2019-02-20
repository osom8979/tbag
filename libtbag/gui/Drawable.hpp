/**
 * @file   Drawable.hpp
 * @brief  Drawable class prototype.
 * @author zer0
 * @date   2019-02-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_DRAWABLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_DRAWABLE_HPP__

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
 * Drawable class prototype.
 *
 * @author zer0
 * @date   2019-02-13
 */
class TBAG_API Drawable : public SfNative
{
public:
    explicit Drawable(SfType type, no_init_t, bool ref = true);
    explicit Drawable(SfType type, bool ref = true);
    ~Drawable();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_DRAWABLE_HPP__

