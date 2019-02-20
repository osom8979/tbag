/**
 * @file   Image.hpp
 * @brief  Image class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_IMAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_IMAGE_HPP__

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
 * Image class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API Image : public SfNative
{
public:
    Image();
    Image(Image && obj) TBAG_NOEXCEPT;
    ~Image();

public:
    Image & operator =(Image && obj) TBAG_NOEXCEPT;

public:
    void swap(Image & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Image & lh, Image & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_IMAGE_HPP__

