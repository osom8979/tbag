/**
 * @file   Window.cpp
 * @brief  Window class implementation.
 * @author zer0
 * @date   2019-01-31
 */

#include <libtbag/gui/Window.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#ifndef _STATIC_ASSERT_EQUAL
#define _STATIC_ASSERT_EQUAL(x, y) STATIC_ASSERT_CHECK_IS_EQUALS((int)(x), (int)(y))
#endif

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
_STATIC_ASSERT_EQUAL(sf::Keyboard::Key::Unknown,         libtbag::gui::Window::Key::Unknown);
_STATIC_ASSERT_EQUAL(sf::Keyboard::Key::A,               libtbag::gui::Window::Key::A);
_STATIC_ASSERT_EQUAL(sf::Keyboard::Key::Pause,           libtbag::gui::Window::Key::Pause);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::Left,            libtbag::gui::Window::Button::Left);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::Right,           libtbag::gui::Window::Button::Right);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::Middle,          libtbag::gui::Window::Button::Middle);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::XButton1,        libtbag::gui::Window::Button::XButton1);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::XButton2,        libtbag::gui::Window::Button::XButton2);
_STATIC_ASSERT_EQUAL(sf::Mouse::Wheel::VerticalWheel,    libtbag::gui::Window::Wheel::VerticalWheel);
_STATIC_ASSERT_EQUAL(sf::Mouse::Wheel::HorizontalWheel,  libtbag::gui::Window::Wheel::HorizontalWheel);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::X,              libtbag::gui::Window::JoystickAxis::X);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::Y,              libtbag::gui::Window::JoystickAxis::Y);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::Z,              libtbag::gui::Window::JoystickAxis::Z);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::R,              libtbag::gui::Window::JoystickAxis::R);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::U,              libtbag::gui::Window::JoystickAxis::U);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::V,              libtbag::gui::Window::JoystickAxis::V);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::PovX,           libtbag::gui::Window::JoystickAxis::PovX);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::PovY,           libtbag::gui::Window::JoystickAxis::PovY);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Accelerometer,    libtbag::gui::Window::SensorType::Accelerometer);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Gyroscope,        libtbag::gui::Window::SensorType::Gyroscope);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Magnetometer,     libtbag::gui::Window::SensorType::Magnetometer);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Gravity,          libtbag::gui::Window::SensorType::Gravity);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::UserAcceleration, libtbag::gui::Window::SensorType::UserAcceleration);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Orientation,      libtbag::gui::Window::SensorType::Orientation);
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#include <iostream>
#endif

#include <cstdlib>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

struct Window::Impl
{
    sf::Color clear_color;
};

// ---------------------
// Window implementation
// ---------------------

Window::Window(Params const & params)
        : SfNative(SfType::RENDER_WINDOW, no_init),
          _impl(std::make_unique<Impl>())
{
    assert(static_cast<bool>(_impl));
    assert(ptr == nullptr);

    auto const MODE = sf::VideoMode(params.width, params.height, params.bpp);
    auto const STYLE = sf::Style::Default;
    auto const CONTEXT = sf::ContextSettings();

    ptr = new sf::RenderWindow(MODE, params.title, STYLE, CONTEXT);
    assert(ptr != nullptr);
}

Window::Window() : Window(Params())
{
    // EMPTY.
}

Window::~Window()
{
    // EMPTY.
}

int Window::run()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    assert(static_cast<bool>(_impl));

#if defined(USE_GUI)
    sf::Event e = sf::Event{};
    onBegin();
    while (window->isOpen()) {
        onPollEventBegin();
        while (window->pollEvent(e)) {
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
        }
        onPollEventEnd();
        onIdle();
    }
    onEnd();
    return 0;
#else
    std::cerr << "Unsupported features." << std::endl;
    return EXIT_FAILURE;
#endif
}

bool Window::isOpen() const
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    return window->isOpen();
}

void Window::close()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    window->close();
}

void Window::clear()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    assert(static_cast<bool>(_impl));
    window->clear(_impl->clear_color);
}

void Window::display()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    window->display();
}

void Window::setClearColor(Rgb24 const & color)
{
    setClearColor(color.r, color.g, color.b);
}

void Window::setClearColor(Rgb32 const & color)
{
    setClearColor(color.r, color.g, color.b, color.a);
}

void Window::setClearColor(Channel r, Channel g, Channel b, Channel a)
{
    assert(static_cast<bool>(_impl));
    _impl->clear_color = sf::Color(r, g, b, a);
}

Window::Rgb32 Window::getClearColor() const
{
    assert(static_cast<bool>(_impl));
    auto const & COLOR = _impl->clear_color;
    return Rgb32{COLOR.r, COLOR.g, COLOR.b, COLOR.a};
}

void Window::onBegin()
{
    // EMPTY.
}

void Window::onEnd()
{
    // EMPTY.
}

void Window::onPollEventBegin()
{
    // EMPTY.
}

void Window::onPollEventEnd()
{
    // EMPTY.
}

void Window::onClosed()
{
    close();
}

void Window::onResized(unsigned int width, unsigned int height)
{
    // EMPTY.
}

void Window::onLostFocus()
{
    // EMPTY.
}

void Window::onGainedFocus()
{
    // EMPTY.
}

void Window::onKeyPressed(Key code, bool alt, bool control, bool shift, bool system)
{
    // EMPTY.
}

void Window::onKeyReleased(Key code, bool alt, bool control, bool shift, bool system)
{
    // EMPTY.
}

void Window::onTextEntered(unsigned int unicode)
{
    // EMPTY.
}

void Window::onMouseMoved(int x, int y)
{
    // EMPTY.
}

void Window::onMouseEntered()
{
    // EMPTY.
}

void Window::onMouseLeft()
{
    // EMPTY.
}

void Window::onMouseButtonPressed(Button button, int x, int y)
{
    // EMPTY.
}

void Window::onMouseButtonReleased(Button button, int x, int y)
{
    // EMPTY.
}

void Window::onMouseWheelScrolled(Wheel wheel, float delta, int x, int y)
{
    // EMPTY.
}

void Window::onJoystickConnected(unsigned int joystick_id)
{
    // EMPTY.
}

void Window::onJoystickDisconnected(unsigned int joystick_id)
{
    // EMPTY.
}

void Window::onJoystickMoved(unsigned int joystick_id, JoystickAxis axis, float position)
{
    // EMPTY.
}

void Window::onJoystickButtonPressed(unsigned int joystick_id, unsigned int button)
{
    // EMPTY.
}

void Window::onJoystickButtonReleased(unsigned int joystick_id, unsigned int button)
{
    // EMPTY.
}

void Window::onTouchBegan(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void Window::onTouchMoved(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void Window::onTouchEnded(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void Window::onSensorChanged(SensorType type, float x, float y, float z)
{
    // EMPTY.
}

void Window::onIdle()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

