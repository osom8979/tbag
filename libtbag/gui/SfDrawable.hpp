/**
 * @file   SfDrawable.hpp
 * @brief  SfDrawable class prototype.
 * @author zer0
 * @date   2019-02-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFDRAWABLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFDRAWABLE_HPP__

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
 * SfDrawable class prototype.
 *
 * @author zer0
 * @date   2019-02-13
 */
class TBAG_API SfDrawable : public SfNative
{
public:
    SfDrawable(SfType type);
    SfDrawable(SfType type, no_init_t);
    virtual ~SfDrawable();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFDRAWABLE_HPP__

