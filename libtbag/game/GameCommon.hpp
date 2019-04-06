/**
 * @file   GameCommon.hpp
 * @brief  GameCommon class prototype.
 * @author zer0
 * @date   2019-04-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_GAMECOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_GAMECOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/res/Storage.hpp>

#include <cstdint>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {

/**
 * GameState class prototype.
 *
 * @author zer0
 * @date   2019-03-24
 * @date   2019-04-06 (Rename: GameState -> GameState)
 */
struct GameState
{
    std::int64_t delta = 0;

    GameState()
    { /* EMPTY. */ }

    ~GameState()
    { /* EMPTY. */ }
};

/**
 * GameVideoMode parameters.
 *
 * @author zer0
 * @date   2019-03-23
 * @date   2019-04-06 (Rename: VideoMode -> GameVideoMode)
 */
struct GameVideoMode
{
    TBAG_CONSTEXPR static unsigned int DEFAULT_WINDOW_WIDTH  = 600;
    TBAG_CONSTEXPR static unsigned int DEFAULT_WINDOW_HEIGHT = 480;
    TBAG_CONSTEXPR static unsigned int DEFAULT_WINDOW_BPP    = 32;

    /** Window width. */
    unsigned int width = DEFAULT_WINDOW_WIDTH;

    /** Window height. */
    unsigned int height = DEFAULT_WINDOW_HEIGHT;

    /** Bits Per Pixel. */
    unsigned int bpp = DEFAULT_WINDOW_BPP;

    GameVideoMode()
    { /* EMPTY. */ }

    GameVideoMode(unsigned int w, unsigned int h, unsigned int b)
            : width(w), height(h), bpp(b)
    { /* EMPTY. */ }

    ~GameVideoMode()
    { /* EMPTY. */ }
};

/**
 * Window parameters.
 *
 * @author zer0
 * @date   2019-03-23
 * @date   2019-04-06 (Rename: WindowParams -> GameParams)
 */
struct GameParams : public GameVideoMode
{
    /** Non-debug, compatibility context (this and the core attribute are mutually exclusive) */
    TBAG_CONSTEXPR static unsigned int const ATTRIBUTE_DEFAULT_FLAG = 0;

    /** Core attribute. */
    TBAG_CONSTEXPR static unsigned int const ATTRIBUTE_CORE_FLAG = (1<<0);

    /** Debug attribute. */
    TBAG_CONSTEXPR static unsigned int const ATTRIBUTE_DEBUG_FLAG = (1<<2);

    /** No border / title bar (this flag and all others are mutually exclusive) */
    TBAG_CONSTEXPR static std::uint32_t STYLE_NONE = 0;

    /** Title bar + fixed border */
    TBAG_CONSTEXPR static std::uint32_t STYLE_TITLEBAR = 1 << 0;

    /** Title bar + resizable border + maximize button */
    TBAG_CONSTEXPR static std::uint32_t STYLE_RESIZE = 1 << 1;

    /** Title bar + close button */
    TBAG_CONSTEXPR static std::uint32_t STYLE_CLOSE = 1 << 2;

    /** Fullscreen mode (this flag and all others are mutually exclusive) */
    TBAG_CONSTEXPR static std::uint32_t STYLE_FULL_SCREEN = 1 << 3;

    /** Default window style */
    TBAG_CONSTEXPR static std::uint32_t STYLE_DEFAULT = STYLE_TITLEBAR | STYLE_RESIZE | STYLE_CLOSE;

    /** Window title. */
    std::string title = LIBTBAG_MAIN_TITLE;

    /** Window style. */
    std::uint32_t style = STYLE_DEFAULT;

    /** Bits of the depth buffer. */
    unsigned int depth_bits = 0;

    /** Bits of the stencil buffer. */
    unsigned int stencil_bits = 0;

    /** Level of antialiasing. */
    unsigned int antialiasing_level = 0;

    /** Major number of the context version to create. */
    unsigned int major_version = 1;

    /** Minor number of the context version to create. */
    unsigned int minor_version = 1;

    /** The attribute flags to create the context with. */
    unsigned int attribute_flags = ATTRIBUTE_DEFAULT_FLAG;

    /** Whether the context framebuffer is sRGB capable. */
    bool srgb_capable = false;

    std::uint8_t clear_red   = 0;
    std::uint8_t clear_green = 0;
    std::uint8_t clear_blue  = 0;
    std::uint8_t clear_alpha = 0;

    GameParams()
    { /* EMPTY. */ }

    ~GameParams()
    { /* EMPTY. */ }
};

enum class GameKey
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

enum class GameButton
{
    Left,       ///< The left mouse button
    Right,      ///< The right mouse button
    Middle,     ///< The middle (wheel) mouse button
    XButton1,   ///< The first extra mouse button
    XButton2,   ///< The second extra mouse button
};

enum class GameWheel
{
    VerticalWheel,  ///< The vertical mouse wheel
    HorizontalWheel ///< The horizontal mouse wheel
};

enum class GameJoystickAxis
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

enum class GameSensorType
{
    Accelerometer,    ///< Measures the raw acceleration (m/s^2)
    Gyroscope,        ///< Measures the raw rotation rates (degrees/s)
    Magnetometer,     ///< Measures the ambient magnetic field (micro-teslas)
    Gravity,          ///< Measures the direction and intensity of gravity, independent of device acceleration (m/s^2)
    UserAcceleration, ///< Measures the direction and intensity of device acceleration, independent of the gravity (m/s^2)
    Orientation,      ///< Measures the absolute 3D orientation (degrees)
};

/**
 * WindowInterface class prototype.
 *
 * @author zer0
 * @date   2019-03-23
 * @date   2019-04-06 (Rename: WindowInterface -> GameInterface)
 */
struct GameInterface
{
    GameInterface()
    { /* EMPTY. */ }

    virtual ~GameInterface()
    { /* EMPTY. */ }

    virtual void closeGame()
    { /* EMPTY. */ }

    virtual bool onCreate()
    { return true; }

    virtual void onDestroy()
    { /* EMPTY. */ }

    virtual void onCheck(GameState & state)
    { /* EMPTY. */ }

    virtual void onPreEvent(GameState & state)
    { /* EMPTY. */ }

    virtual void onPostEvent(GameState & state)
    { /* EMPTY. */ }

    virtual void onUpdate(GameState & state)
    { /* EMPTY. */ }

    virtual void onPostUpdate1(GameState & state)
    { /* EMPTY. */ }

    virtual void onPostUpdate2(GameState & state)
    { /* EMPTY. */ }

    virtual void onDraw(GameState & state)
    { /* EMPTY. */ }

    virtual void onPostDraw1(GameState & state)
    { /* EMPTY. */ }

    virtual void onPostDraw2(GameState & state)
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
    virtual void onKeyPressed(GameKey code, bool alt, bool control, bool shift, bool system)
    { /* EMPTY. */ }

    /**
     * A key was released (data in event.key)
     */
    virtual void onKeyReleased(GameKey code, bool alt, bool control, bool shift, bool system)
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
    virtual void onMouseButtonPressed(GameButton button, int x, int y)
    { /* EMPTY. */ }

    /**
     * A mouse button was released (data in event.mouseButton)
     */
    virtual void onMouseButtonReleased(GameButton button, int x, int y)
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
    virtual void onMouseWheelScrolled(GameWheel wheel, float delta, int x, int y)
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
    virtual void onJoystickMoved(unsigned int joystick_id, GameJoystickAxis axis, float position)
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
    virtual void onSensorChanged(GameSensorType type, float x, float y, float z)
    { /* EMPTY. */ }
};

// ---------
// Utilities
// ---------

TBAG_API GameVideoMode getDesktopMode();
TBAG_API std::vector<GameVideoMode> getFullscreenModes();

TBAG_API bool readWindowParams(libtbag::res::Storage & storage, GameParams & params);
TBAG_API bool saveWindowParams(libtbag::res::Storage & storage, GameParams const & params);

TBAG_API void callEvnet(void * sf_event, GameInterface * interface);

} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_GAMECOMMON_HPP__

