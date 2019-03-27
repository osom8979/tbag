/**
 * @file   WindowInterface.hpp
 * @brief  WindowInterface class prototype.
 * @author zer0
 * @date   2019-03-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_WINDOWINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_WINDOWINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/game/WindowState.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {

/**
 * WindowInterface class prototype.
 *
 * @author zer0
 * @date   2019-03-23
 */
struct WindowInterface
{
    enum class Key
    {
        Unknown = -1, ///< Unhandled key
        A = 0,        ///< The A key
        B,            ///< The B key
        C,            ///< The C key
        D,            ///< The D key
        E,            ///< The E key
        F,            ///< The F key
        G,            ///< The G key
        H,            ///< The H key
        I,            ///< The I key
        J,            ///< The J key
        K,            ///< The K key
        L,            ///< The L key
        M,            ///< The M key
        N,            ///< The N key
        O,            ///< The O key
        P,            ///< The P key
        Q,            ///< The Q key
        R,            ///< The R key
        S,            ///< The S key
        T,            ///< The T key
        U,            ///< The U key
        V,            ///< The V key
        W,            ///< The W key
        X,            ///< The X key
        Y,            ///< The Y key
        Z,            ///< The Z key
        Num0,         ///< The 0 key
        Num1,         ///< The 1 key
        Num2,         ///< The 2 key
        Num3,         ///< The 3 key
        Num4,         ///< The 4 key
        Num5,         ///< The 5 key
        Num6,         ///< The 6 key
        Num7,         ///< The 7 key
        Num8,         ///< The 8 key
        Num9,         ///< The 9 key
        Escape,       ///< The Escape key
        LControl,     ///< The left Control key
        LShift,       ///< The left Shift key
        LAlt,         ///< The left Alt key
        LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
        RControl,     ///< The right Control key
        RShift,       ///< The right Shift key
        RAlt,         ///< The right Alt key
        RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
        Menu,         ///< The Menu key
        LBracket,     ///< The [ key
        RBracket,     ///< The ] key
        Semicolon,    ///< The ; key
        Comma,        ///< The , key
        Period,       ///< The . key
        Quote,        ///< The ' key
        Slash,        ///< The / key
        Backslash,    ///< The \ key
        Tilde,        ///< The ~ key
        Equal,        ///< The = key
        Hyphen,       ///< The - key (hyphen)
        Space,        ///< The Space key
        Enter,        ///< The Enter/Return keys
        Backspace,    ///< The Backspace key
        Tab,          ///< The Tabulation key
        PageUp,       ///< The Page up key
        PageDown,     ///< The Page down key
        End,          ///< The End key
        Home,         ///< The Home key
        Insert,       ///< The Insert key
        Delete,       ///< The Delete key
        Add,          ///< The + key
        Subtract,     ///< The - key (minus, usually from numpad)
        Multiply,     ///< The * key
        Divide,       ///< The / key
        Left,         ///< Left arrow
        Right,        ///< Right arrow
        Up,           ///< Up arrow
        Down,         ///< Down arrow
        Numpad0,      ///< The numpad 0 key
        Numpad1,      ///< The numpad 1 key
        Numpad2,      ///< The numpad 2 key
        Numpad3,      ///< The numpad 3 key
        Numpad4,      ///< The numpad 4 key
        Numpad5,      ///< The numpad 5 key
        Numpad6,      ///< The numpad 6 key
        Numpad7,      ///< The numpad 7 key
        Numpad8,      ///< The numpad 8 key
        Numpad9,      ///< The numpad 9 key
        F1,           ///< The F1 key
        F2,           ///< The F2 key
        F3,           ///< The F3 key
        F4,           ///< The F4 key
        F5,           ///< The F5 key
        F6,           ///< The F6 key
        F7,           ///< The F7 key
        F8,           ///< The F8 key
        F9,           ///< The F9 key
        F10,          ///< The F10 key
        F11,          ///< The F11 key
        F12,          ///< The F12 key
        F13,          ///< The F13 key
        F14,          ///< The F14 key
        F15,          ///< The F15 key
        Pause,        ///< The Pause key
    };

    enum class Button
    {
        Left,       ///< The left mouse button
        Right,      ///< The right mouse button
        Middle,     ///< The middle (wheel) mouse button
        XButton1,   ///< The first extra mouse button
        XButton2,   ///< The second extra mouse button
    };

    enum class Wheel
    {
        VerticalWheel,  ///< The vertical mouse wheel
        HorizontalWheel ///< The horizontal mouse wheel
    };

    enum class JoystickAxis
    {
        X,    ///< The X axis
        Y,    ///< The Y axis
        Z,    ///< The Z axis
        R,    ///< The R axis
        U,    ///< The U axis
        V,    ///< The V axis
        PovX, ///< The X axis of the point-of-view hat
        PovY  ///< The Y axis of the point-of-view hat
    };

    enum class SensorType
    {
        Accelerometer,    ///< Measures the raw acceleration (m/s^2)
        Gyroscope,        ///< Measures the raw rotation rates (degrees/s)
        Magnetometer,     ///< Measures the ambient magnetic field (micro-teslas)
        Gravity,          ///< Measures the direction and intensity of gravity, independent of device acceleration (m/s^2)
        UserAcceleration, ///< Measures the direction and intensity of device acceleration, independent of the gravity (m/s^2)
        Orientation,      ///< Measures the absolute 3D orientation (degrees)
    };

    WindowInterface()
    { /* EMPTY. */ }

    virtual ~WindowInterface()
    { /* EMPTY. */ }

    virtual bool onCreate()
    { return true; }

    virtual void onDestroy()
    { /* EMPTY. */ }

    virtual void onCheck(WindowState & state)
    { /* EMPTY. */ }

    virtual void onPreEvent(WindowState & state)
    { /* EMPTY. */ }

    virtual void onPostEvent(WindowState & state)
    { /* EMPTY. */ }

    virtual void onUpdate(WindowState & state)
    { /* EMPTY. */ }

    virtual void onPreDraw(WindowState & state)
    { /* EMPTY. */ }

    virtual void onPostDraw(WindowState & state)
    { /* EMPTY. */ }

    /**
     * The window requested to be closed (no data)
     */
    virtual void onClosed()
    { /* EMPTY. */ }

    /**
     * The window was resized (data in event.size)
     *
     * @param[in] width
     *      New width, in pixels.
     * @param[in] height
     *      New height, in pixels.
     */
    virtual void onResized(unsigned int width, unsigned int height)
    { /* EMPTY. */ }

    /**
     * The window lost the focus (no data)
     */
    virtual void onLostFocus()
    { /* EMPTY. */ }

    /**
     * The window gained the focus (no data)
     */
    virtual void onGainedFocus()
    { /* EMPTY. */ }

    /**
     * A key was pressed (data in event.key)
     *
     * @param[in] code
     *      Key code.
     * @param[in] alt
     *      Is the Alt key pressed?
     * @param[in] control
     *      Is the Control key pressed?
     * @param[in] shift
     *      Is the Shift key pressed?
     * @param[in] system
     *      Is the System key pressed?
     */
    virtual void onKeyPressed(Key code, bool alt, bool control, bool shift, bool system)
    { /* EMPTY. */ }

    /**
     * A key was released (data in event.key)
     */
    virtual void onKeyReleased(Key code, bool alt, bool control, bool shift, bool system)
    { /* EMPTY. */ }

    /**
     * A character was entered (data in event.text)
     *
     * @param[in] unicode
     *      UTF-32 Unicode value of the character.
     */
    virtual void onTextEntered(unsigned int unicode)
    { /* EMPTY. */ }

    /**
     * The mouse cursor moved (data in event.mouseMove)
     *
     * @param[in] x
     *      X position of the mouse pointer, relative to the left of the owner window.
     * @param[in] y
     *      Y position of the mouse pointer, relative to the top of the owner window.
     */
    virtual void onMouseMoved(int x, int y)
    { /* EMPTY. */ }

    /**
     * The mouse cursor entered the area of the window (no data)
     */
    virtual void onMouseEntered()
    { /* EMPTY. */ }

    /**
     * The mouse cursor left the area of the window (no data)
     */
    virtual void onMouseLeft()
    { /* EMPTY. */ }

    /**
     * A mouse button was pressed (data in event.mouseButton)
     *
     * @param[in] button
     *      Code of the button that has been pressed.
     * @param[in] x
     *      X position of the mouse pointer, relative to the left of the owner window.
     * @param[in] y
     *      Y position of the mouse pointer, relative to the top of the owner window.
     */
    virtual void onMouseButtonPressed(Button button, int x, int y)
    { /* EMPTY. */ }

    /**
     * A mouse button was released (data in event.mouseButton)
     */
    virtual void onMouseButtonReleased(Button button, int x, int y)
    { /* EMPTY. */ }

    /**
     * The mouse wheel was scrolled (data in event.mouseWheelScroll)
     *
     * @param[in] wheel
     *      Which wheel (for mice with multiple ones)
     * @param[in] delta
     *      Wheel offset (positive is up/left, negative is down/right). @n
     *      High-precision mice may use non-integral offsets.
     * @param[in] x
     *      X position of the mouse pointer, relative to the left of the owner window.
     * @param[in] y
     *      Y position of the mouse pointer, relative to the top of the owner window.
     */
    virtual void onMouseWheelScrolled(Wheel wheel, float delta, int x, int y)
    { /* EMPTY. */ }

    /**
     * A joystick was connected (data in event.joystickConnect)
     *
     * @param[in] joystick_id
     *      Index of the joystick (in range [0 .. Joystick::Count - 1])
     */
    virtual void onJoystickConnected(unsigned int joystick_id)
    { /* EMPTY. */ }

    /**
     * A joystick was disconnected (data in event.joystickConnect)
     */
    virtual void onJoystickDisconnected(unsigned int joystick_id)
    { /* EMPTY. */ }

    /**
     * The joystick moved along an axis (data in event.joystickMove)
     *
     * @param[in] joystick_id
     *      Index of the joystick (in range [0 .. Joystick::Count - 1])
     * @param[in] axis
     *      Axis on which the joystick moved
     * @param[in] position
     *      New position on the axis (in range [-100 .. 100])
     */
    virtual void onJoystickMoved(unsigned int joystick_id, JoystickAxis axis, float position)
    { /* EMPTY. */ }

    /**
     * A joystick button was pressed (data in event.joystickButton)
     *
     * @param[in] joystick_id
     *      Index of the joystick (in range [0 .. Joystick::Count - 1])
     * @param[in] button
     *      Index of the button that has been pressed (in range [0 .. Joystick::ButtonCount - 1])
     */
    virtual void onJoystickButtonPressed(unsigned int joystick_id, unsigned int button)
    { /* EMPTY. */ }

    /**
     * A joystick button was released (data in event.joystickButton)
     */
    virtual void onJoystickButtonReleased(unsigned int joystick_id, unsigned int button)
    { /* EMPTY. */ }

    /**
     * A touch event began (data in event.touch)
     *
     * @param[in] finger
     *      Index of the finger in case of multi-touch events
     * @param[in] x
     *      X position of the touch, relative to the left of the owner window
     * @param[in] y
     *      Y position of the touch, relative to the top of the owner window
     */
    virtual void onTouchBegan(unsigned int finger, int x, int y)
    { /* EMPTY. */ }

    /**
     * A touch moved (data in event.touch)
     */
    virtual void onTouchMoved(unsigned int finger, int x, int y)
    { /* EMPTY. */ }

    /**
     * A touch event ended (data in event.touch)
     */
    virtual void onTouchEnded(unsigned int finger, int x, int y)
    { /* EMPTY. */ }

    /**
     * A sensor value changed (data in event.sensor)
     *
     * @param[in] type
     *      Type of the sensor.
     * @param[in] x
     *      Current value of the sensor on X axis.
     * @param[in] y
     *      Current value of the sensor on Y axis.
     * @param[in] z
     *      Current value of the sensor on Z axis.
     */
    virtual void onSensorChanged(SensorType type, float x, float y, float z)
    { /* EMPTY. */ }
};

} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_WINDOWINTERFACE_HPP__

