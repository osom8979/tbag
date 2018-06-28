/**
 * @file   Overlay.hpp
 * @brief  Overlay class prototype.
 * @author zer0
 * @date   2018-06-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_OVERLAY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_OVERLAY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/geometry/Rect.hpp>
#include <libtbag/graphic/OverlayState.hpp>

#include <cassert>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

/**
 * Overlay class prototype.
 *
 * @author zer0
 * @date   2018-06-28
 */
template <typename T>
struct Overlay
{
    using Value = T;
    using Rect  = libtbag::geometry::BaseRect<Value>;

    using State = OverlayState;
    using Event = MouseEvent;

    State state = OverlayState::OS_NORMAL;  ///< Current overlay state.
    Event event = Event::ME_NONE;           ///< Current mouse state.

    bool pressed  = false;  ///< Pressed the mouse button?
    bool clicked  = false;  ///< Was it clicked in the area?
    bool on_event = false;  ///< Is the OnClick() event enabled?

    Rect area;  ///< Overlay area.

    Overlay() TBAG_NOEXCEPT_SPECIFIER(std::is_nothrow_constructible<Rect>::value)
    { /* EMPTY. */ }
    Overlay(Value x, Value y, Value w, Value h) : area(x, y, w, h)
    { /* EMPTY. */ }
    Overlay(Rect const & a) : area(a)
    { /* EMPTY. */ }
    ~Overlay()
    { /* EMPTY. */ }

    /**
     * Update state.
     *
     * @param[in] mouse_x
     *      X-axis of the mouse pointer.
     * @param[in] mouse_y
     *      Y-axis of the mouse pointer.
     * @param[in] button_pressed
     *      Pressed the mouse button?
     *
     * @return
     *  Changed state?
     */
    bool update(Value mouse_x, Value mouse_y, bool button_pressed)
    {
        if (button_pressed) {
            pressed = true;
            event = Event::ME_DOWN;
        } else if (pressed) {
            pressed = false;
            event = Event::ME_UP;
        } else {
            assert(button_pressed == false);
            assert(pressed == false);
            event = Event::ME_NONE;
        }

        auto const POS = area.point();
        auto const SIZE = area.size();
        auto const RESULT = calcOverlayState<Value>(
                POS.x, POS.y,
                SIZE.width, SIZE.height,
                mouse_x, mouse_y,
                event, clicked);

        on_event = RESULT.second;
        if (state != RESULT.first) {
            state = RESULT.first;
            return true;
        }
        return false;
    }
};

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_OVERLAY_HPP__

