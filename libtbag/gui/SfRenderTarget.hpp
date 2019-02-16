/**
 * @file   SfRenderTarget.hpp
 * @brief  SfRenderTarget class prototype.
 * @author zer0
 * @date   2019-02-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFRENDERTARGET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFRENDERTARGET_HPP__

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
 * SfRenderTarget class prototype.
 *
 * @author zer0
 * @date   2019-02-16
 */
class TBAG_API SfRenderTarget : public SfNative
{
public:
    SfRenderTarget(SfType type);
    SfRenderTarget(SfType type, no_init_t);
    virtual ~SfRenderTarget();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFRENDERTARGET_HPP__

