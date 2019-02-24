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
    explicit RenderTarget(SfType type, no_init_t, bool ref = true);
    explicit RenderTarget(SfType type, bool ref = true);
    virtual ~RenderTarget();

public:
    void clear(Channel r, Channel g, Channel b, Channel a = CHANNEL_MAX);
    void clear(Rgb24 const & color);
    void clear(Rgb32 const & color);
    void clear();

public:
    Pointf mapPixelToCoords(Pointi const & point) const;
    Pointi mapCoordsToPixel(Pointf const & point) const;

public:
    Sizeu getSize() const;
    bool  setActive(bool active = true);

public:
    void pushGLStates();
    void popGLStates();
    void resetGLStates();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_RENDERTARGET_HPP__

