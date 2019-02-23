/**
 * @file   Text.hpp
 * @brief  Text class prototype.
 * @author zer0
 * @date   2019-02-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_TEXT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_TEXT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gui/Drawable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * Text class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API Text : public Drawable
{
public:
    Text();
    Text(Text && obj) TBAG_NOEXCEPT;
    virtual ~Text();

public:
    Text & operator =(Text && obj) TBAG_NOEXCEPT;

public:
    void swap(Text & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Text & lh, Text & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_TEXT_HPP__

