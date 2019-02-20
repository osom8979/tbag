/**
 * @file   BlendMode.hpp
 * @brief  BlendMode class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_BLENDMODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_BLENDMODE_HPP__

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
 * BlendMode class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API BlendMode : public SfNative
{
public:
    BlendMode();
    BlendMode(BlendMode && obj) TBAG_NOEXCEPT;
    ~BlendMode();

public:
    BlendMode & operator =(BlendMode && obj) TBAG_NOEXCEPT;

public:
    void swap(BlendMode & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(BlendMode & lh, BlendMode & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_BLENDMODE_HPP__

