/**
 * @file   Window.cpp
 * @brief  Window class implementation.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> RenderWindow)
 * @date   2019-03-23 (Rename: RenderWindow -> Window)
 */

#include <libtbag/gui/Window.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/gui/WindowInterface.hpp>
#include <libtbag/gui/WindowParams.hpp>

#include <libtbag/graphic/Color.hpp>
#include <libtbag/geometry/GeometryTypes.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/res/Storage.hpp>

#include <libtbag/3rd/imgui/imgui.h>
#include <libtbag/3rd/imgui/imgui-SFML.h>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <cassert>

#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

enum class WindowExitCode
{
    WEC_RESTART = 0,
    WEC_EXIT_SUCCESS,
    WEC_EXIT_FAILURE,
};

/**
 * Window backend implementation.
 *
 * @author zer0
 * @date   2019-03-23
 */
struct Window : public WindowInterface, public libtbag::geometry::GeometryTypes
{
    STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::Unknown, Key::Unknown);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::A,       Key::A);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::Pause,   Key::Pause);

    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Left,     Button::Left);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Right,    Button::Right);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Middle,   Button::Middle);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::XButton1, Button::XButton1);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::XButton2, Button::XButton2);

    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Wheel::VerticalWheel,   Wheel::VerticalWheel);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Wheel::HorizontalWheel, Wheel::HorizontalWheel);

    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::X,    JoystickAxis::X);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::Y,    JoystickAxis::Y);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::Z,    JoystickAxis::Z);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::R,    JoystickAxis::R);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::U,    JoystickAxis::U);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::V,    JoystickAxis::V);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::PovX, JoystickAxis::PovX);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::PovY, JoystickAxis::PovY);

    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Accelerometer,    SensorType::Accelerometer);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Gyroscope,        SensorType::Gyroscope);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Magnetometer,     SensorType::Magnetometer);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Gravity,          SensorType::Gravity);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::UserAcceleration, SensorType::UserAcceleration);
    STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Orientation,      SensorType::Orientation);

    using Channel = libtbag::graphic::Channel;
    using Rgb24   = libtbag::graphic::Rgb24;
    using Rgb32   = libtbag::graphic::Rgb32;

    TBAG_CONSTEXPR static Channel const CHANNEL_MAX  = libtbag::graphic::channel_max();
    TBAG_CONSTEXPR static Channel const CHANNEL_MIN  = libtbag::graphic::channel_min();
    TBAG_CONSTEXPR static Channel const CHANNEL_HALF = libtbag::graphic::channel_half();

    using Storage = libtbag::res::Storage;

    Storage          _storage;
    sf::RenderWindow _window;
    sf::Color        _clear;

    Window(Storage const & storage,
           sf::VideoMode const mode,
           std::string const & title,
           sf::Uint32 style,
           sf::ContextSettings const context,
           sf::Color const & clear)
            : _storage(storage), _window(mode, title, style, context), _clear(clear)
    {
        // EMPTY.
    }

    ~Window()
    {
        // EMPTY.
    }

    void runEvent(sf::Event & e)
    {
        // @formatter:off
        switch (e.type) {
        case sf::Event::Closed:
            onClosed();
            break;
        case sf::Event::Resized:
            onResized(e.size.width, e.size.height);
            break;
        case sf::Event::LostFocus:
            onLostFocus();
            break;
        case sf::Event::GainedFocus:
            onGainedFocus();
            break;
        case sf::Event::TextEntered:
            onTextEntered(e.text.unicode);
            break;
        case sf::Event::KeyPressed:
            onKeyPressed((Key)e.key.code, e.key.alt, e.key.control, e.key.shift, e.key.system);
            break;
        case sf::Event::KeyReleased:
            onKeyReleased((Key)e.key.code, e.key.alt, e.key.control, e.key.shift, e.key.system);
            break;
        case sf::Event::MouseWheelMoved:
            // DEPRECATED!
            break;
        case sf::Event::MouseWheelScrolled:
            onMouseWheelScrolled((Wheel)e.mouseWheelScroll.wheel, e.mouseWheelScroll.delta, e.mouseWheelScroll.x, e.mouseWheelScroll.y);
            break;
        case sf::Event::MouseButtonPressed:
            onMouseButtonPressed((Button)e.mouseButton.button, e.mouseButton.x, e.mouseButton.y);
            break;
        case sf::Event::MouseButtonReleased:
            onMouseButtonReleased((Button)e.mouseButton.button, e.mouseButton.x, e.mouseButton.y);
            break;
        case sf::Event::MouseMoved:
            onMouseMoved(e.mouseMove.x, e.mouseMove.y);
            break;
        case sf::Event::MouseEntered:
            onMouseEntered();
            break;
        case sf::Event::MouseLeft:
            onMouseLeft();
            break;
        case sf::Event::JoystickButtonPressed:
            onJoystickButtonPressed(e.joystickButton.joystickId, e.joystickButton.button);
            break;
        case sf::Event::JoystickButtonReleased:
            onJoystickButtonReleased(e.joystickButton.joystickId, e.joystickButton.button);
            break;
        case sf::Event::JoystickMoved:
            onJoystickMoved(e.joystickMove.joystickId, (JoystickAxis)e.joystickMove.axis, e.joystickMove.position);
            break;
        case sf::Event::JoystickConnected:
            onJoystickConnected(e.joystickConnect.joystickId);
            break;
        case sf::Event::JoystickDisconnected:
            onJoystickDisconnected(e.joystickConnect.joystickId);
            break;
        case sf::Event::TouchBegan:
            onTouchBegan(e.touch.finger, e.touch.x, e.touch.y);
            break;
        case sf::Event::TouchMoved:
            onTouchMoved(e.touch.finger, e.touch.x, e.touch.y);
            break;
        case sf::Event::TouchEnded:
            onTouchEnded(e.touch.finger, e.touch.x, e.touch.y);
            break;
        case sf::Event::SensorChanged:
            onSensorChanged((SensorType)e.sensor.type, e.sensor.x, e.sensor.y, e.sensor.z);
            break;
        default:
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
            break;
        }
        // @formatter:on
    }

    virtual void onClosed() override
    {
        _window.close();
    }

    WindowExitCode run()
    {
        if (!onCreate()) {
            return WindowExitCode::WEC_EXIT_FAILURE;
        }

        WindowState state;
        sf::Clock   clock;
        sf::Event   event;
        sf::Time    delta;

        ImGui::SFML::Init(_window);

        while (_window.isOpen()) {
            delta = clock.restart();
            state.delta = delta.asMicroseconds();

            onCheck(state);

            while (_window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);
                onPreEvent(state);
                runEvent(event);
                onPostEvent(state);
            }

            ImGui::SFML::Update(_window, delta);
            onUpdate(state);

            _window.clear(_clear);

            onPreDraw(state);
            ImGui::SFML::Render(_window);
            onPostDraw(state);

            _window.display();
        }

        onDestroy();

        return WindowExitCode::WEC_EXIT_SUCCESS;
    }
};

static WindowExitCode runGameMain(libtbag::res::Storage & storage, WindowParams const & params)
{
    auto const MODE = sf::VideoMode(params.width, params.height, params.bpp);
    auto const STYLE = static_cast<sf::Uint32>(params.style);
    auto const CONTEXT = sf::ContextSettings(
            params.depth_bits,
            params.stencil_bits,
            params.antialiasing_level,
            params.major_version,
            params.minor_version,
            params.attribute_flags,
            params.srgb_capable);
    auto const CLEAR = sf::Color(params.clear_red, params.clear_green, params.clear_blue, params.clear_alpha);
    return Window(storage, MODE, LIBTBAG_MAIN_TITLE, STYLE, CONTEXT, CLEAR).run();
}

int runGame(libtbag::res::Storage & storage)
{
    bool exit_game = false;
    int  exit_code = EXIT_FAILURE;

    while (!exit_game) {
        WindowParams params;
        readWindowParams(storage, params);
        switch (runGameMain(storage, params)) {
        case WindowExitCode::WEC_RESTART:
            break;
        case WindowExitCode::WEC_EXIT_SUCCESS:
            exit_game = true;
            exit_code = EXIT_SUCCESS;
            break;
        case WindowExitCode::WEC_EXIT_FAILURE:
            exit_game = true;
            exit_code = EXIT_FAILURE;
            break;
        }
    }

    return exit_code;
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

