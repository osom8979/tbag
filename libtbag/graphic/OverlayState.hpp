/**
 * @file   OverlayState.hpp
 * @brief  OverlayState class prototype.
 * @author zer0
 * @date   2018-06-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_OVERLAYSTATE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_OVERLAYSTATE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Rect2.hpp>

#include <cassert>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

enum class OverlayState
{
    OS_NORMAL,
    OS_HOVER,
    OS_ACTIVE,
};

enum class MouseEvent
{
    ME_NONE,
    ME_DOWN,
    ME_UP,
};

using IsOnClick = bool;

template <typename T>
std::pair<OverlayState, IsOnClick> calcOverlayState(T x, T y, T w, T h, T mouse_x, T mouse_y, MouseEvent mouse_event,
                                                    bool & save_inside_clicked) TBAG_NOEXCEPT
{
    auto const IS_CONTAINS = libtbag::geometry::BaseRect2<T>(x, y, w, h).contains(mouse_x, mouse_y);
    auto result = std::make_pair(OverlayState::OS_NORMAL, false);

    // Check the OnClick event.
    if (mouse_event == MouseEvent::ME_DOWN && IS_CONTAINS) {
        save_inside_clicked = true;
    } else if (mouse_event == MouseEvent::ME_UP) {
        if (IS_CONTAINS && save_inside_clicked) {
            result.second = true;
        }
        save_inside_clicked = false;
    }

    // Check the overlay state.
    if (IS_CONTAINS) {
        if (save_inside_clicked) {
            result.first = OverlayState::OS_ACTIVE;
        } else {
            result.first = OverlayState::OS_HOVER;
        }
    } else {
        result.first = OverlayState::OS_NORMAL;
    }
    return result;
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_OVERLAYSTATE_HPP__

