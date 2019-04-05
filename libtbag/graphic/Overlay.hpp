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

#include <libtbag/geometry/Rect2.hpp>
#include <libtbag/graphic/OverlayState.hpp>

#include <cassert>
#include <string>
#include <sstream>
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
    using Rect  = libtbag::geometry::BaseRect2<Value>;

    using State = OverlayState;
    using Event = MouseEvent;

    State state = OverlayState::OS_NORMAL;  ///< Current overlay state.
    Event event = Event::ME_NONE;           ///< Current mouse state.

    bool momentarily_pressed = false; ///< Did you pressed for a while?
    bool save_inside_clicked = false; ///< Was it clicked in the area?
    bool on_click_event      = false; ///< Is the OnClick() event enabled?

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
        if (button_pressed && momentarily_pressed == false) {
            momentarily_pressed = true;
            event = Event::ME_DOWN;
        } else if (button_pressed == false && momentarily_pressed) {
            momentarily_pressed = false;
            event = Event::ME_UP;
        } else {
            event = Event::ME_NONE;
        }

        auto const POS = area.point();
        auto const SIZE = area.size();
        auto const RESULT = calcOverlayState<Value>(
                POS.x, POS.y,
                SIZE.width, SIZE.height,
                mouse_x, mouse_y,
                event, save_inside_clicked);

        on_click_event = RESULT.second;
        if (state != RESULT.first) {
            state = RESULT.first;
            return true;
        }
        return false;
    }

    std::string toString() const
    {
        std::stringstream ss;
        // clang-format off
        switch (state) {
        case OverlayState::OS_NORMAL: ss << "NORMAL"; break;
        case OverlayState::OS_HOVER:  ss << "HOVER";  break;
        case OverlayState::OS_ACTIVE: ss << "ACTIVE"; break;
        }
        switch (event) {
        case MouseEvent::ME_NONE: ss << ",NONE"; break;
        case MouseEvent::ME_DOWN: ss << ",DOWN"; break;
        case MouseEvent::ME_UP:   ss << ",UP";   break;
        }
        if (momentarily_pressed) { ss << ",PRESSED"; }
        if (save_inside_clicked) { ss << ",CLICKED"; }
        if (on_click_event) { ss << ",EVENT"; }
        // clang-format on
        return ss.str();
    }
};

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_OVERLAY_HPP__

