/**
 * @file   RenderStates.hpp
 * @brief  RenderStates class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_RENDERSTATES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_RENDERSTATES_HPP__

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
 * RenderStates class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API RenderStates : public SfNative
{
public:
    RenderStates();
    RenderStates(void * handle, no_init_no_ref_t);
    RenderStates(RenderStates && obj) TBAG_NOEXCEPT;
    ~RenderStates();

public:
    RenderStates & operator =(RenderStates && obj) TBAG_NOEXCEPT;

public:
    void swap(RenderStates & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(RenderStates & lh, RenderStates & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_RENDERSTATES_HPP__

