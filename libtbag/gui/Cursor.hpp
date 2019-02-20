/**
 * @file   Cursor.hpp
 * @brief  Cursor class prototype.
 * @author zer0
 * @date   2019-02-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_CURSOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_CURSOR_HPP__

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
 * Cursor class prototype.
 *
 * @author zer0
 * @date   2019-02-17
 */
class TBAG_API Cursor : public SfNative
{
public:
    enum class CursorType
    {
        CT_ARROW,                    ///< Arrow cursor (default)
        CT_ARROWWAIT,                ///< Busy arrow cursor.
        CT_WAIT,                     ///< Busy cursor.
        CT_TEXT,                     ///< I-beam, cursor when hovering over a field allowing text entry.
        CT_HAND,                     ///< Pointing hand cursor.
        CT_SIZE_HORIZONTAL,          ///< Horizontal double arrow cursor.
        CT_SIZE_VERTICAL,            ///< Vertical double arrow cursor.
        CT_SIZE_TOPLEFT_BOTTOMRIGHT, ///< Double arrow cursor going from top-left to bottom-right.
        CT_SIZE_BOTTOMLEFT_TOPRIGHT, ///< Double arrow cursor going from bottom-left to top-right.
        CT_SIZE_ALL,                 ///< Combination of SizeHorizontal and SizeVertical.
        CT_CROSS,                    ///< Crosshair cursor.
        CT_HELP,                     ///< Help cursor.
        CT_NOT_ALLOWED               ///< Action not allowed cursor.
    };

public:
    Cursor();
    Cursor(Cursor && obj) TBAG_NOEXCEPT;
    ~Cursor();

public:
    Cursor & operator =(Cursor && obj) TBAG_NOEXCEPT;

public:
    void swap(Cursor & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Cursor & lh, Cursor & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    bool loadFromPixels(std::uint8_t const * pixels, Sizeu const & size, Pointu const & hotspot);
    bool loadFromSystem(CursorType type);
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_CURSOR_HPP__

