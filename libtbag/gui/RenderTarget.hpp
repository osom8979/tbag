/**
 * @file   RenderTarget.hpp
 * @brief  RenderTarget class prototype.
 * @author zer0
 * @date   2019-02-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_RENDERTARGET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_RENDERTARGET_HPP__

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
 * RenderTarget class prototype.
 *
 * @author zer0
 * @date   2019-02-16
 */
class TBAG_API RenderTarget : public SfNative
{
public:
    explicit RenderTarget(SfType type, no_init_no_ref_t);
    explicit RenderTarget(SfType type, no_init_t, bool ref = true);
    explicit RenderTarget(SfType type, bool ref = true);
    virtual ~RenderTarget();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_RENDERTARGET_HPP__

