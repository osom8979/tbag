/**
 * @file   SfWindow.cpp
 * @brief  SfWindow class implementation.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 */

#include <libtbag/gui/SfWindow.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#ifndef _STATIC_ASSERT_EQUAL
#define _STATIC_ASSERT_EQUAL(x, y) STATIC_ASSERT_CHECK_IS_EQUALS((int)(x), (int)(y))
#endif

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
_STATIC_ASSERT_EQUAL(sf::Keyboard::Key::Unknown,         libtbag::gui::SfWindow::Key::Unknown);
_STATIC_ASSERT_EQUAL(sf::Keyboard::Key::A,               libtbag::gui::SfWindow::Key::A);
_STATIC_ASSERT_EQUAL(sf::Keyboard::Key::Pause,           libtbag::gui::SfWindow::Key::Pause);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::Left,            libtbag::gui::SfWindow::Button::Left);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::Right,           libtbag::gui::SfWindow::Button::Right);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::Middle,          libtbag::gui::SfWindow::Button::Middle);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::XButton1,        libtbag::gui::SfWindow::Button::XButton1);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::XButton2,        libtbag::gui::SfWindow::Button::XButton2);
_STATIC_ASSERT_EQUAL(sf::Mouse::Wheel::VerticalWheel,    libtbag::gui::SfWindow::Wheel::VerticalWheel);
_STATIC_ASSERT_EQUAL(sf::Mouse::Wheel::HorizontalWheel,  libtbag::gui::SfWindow::Wheel::HorizontalWheel);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::X,              libtbag::gui::SfWindow::JoystickAxis::X);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::Y,              libtbag::gui::SfWindow::JoystickAxis::Y);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::Z,              libtbag::gui::SfWindow::JoystickAxis::Z);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::R,              libtbag::gui::SfWindow::JoystickAxis::R);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::U,              libtbag::gui::SfWindow::JoystickAxis::U);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::V,              libtbag::gui::SfWindow::JoystickAxis::V);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::PovX,           libtbag::gui::SfWindow::JoystickAxis::PovX);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::PovY,           libtbag::gui::SfWindow::JoystickAxis::PovY);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Accelerometer,    libtbag::gui::SfWindow::SensorType::Accelerometer);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Gyroscope,        libtbag::gui::SfWindow::SensorType::Gyroscope);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Magnetometer,     libtbag::gui::SfWindow::SensorType::Magnetometer);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Gravity,          libtbag::gui::SfWindow::SensorType::Gravity);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::UserAcceleration, libtbag::gui::SfWindow::SensorType::UserAcceleration);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Orientation,      libtbag::gui::SfWindow::SensorType::Orientation);
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

struct SfWindow::Impl
{
    sf::Color clear_color;
};

// -----------------------
// SfWindow implementation
// -----------------------

SfWindow::SfWindow(Params const & params)
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

SfWindow::SfWindow() : SfWindow(Params())
{
    // EMPTY.
}

SfWindow::~SfWindow()
{
    // EMPTY.
}

int SfWindow::run()
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

bool SfWindow::isOpen() const
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    return window->isOpen();
}

void SfWindow::close()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    window->close();
}

void SfWindow::clear()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    assert(static_cast<bool>(_impl));
    window->clear(_impl->clear_color);
}

void SfWindow::display()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    window->display();
}

void SfWindow::setClearColor(Rgb24 const & color)
{
    setClearColor(color.r, color.g, color.b);
}

void SfWindow::setClearColor(Rgb32 const & color)
{
    setClearColor(color.r, color.g, color.b, color.a);
}

void SfWindow::setClearColor(Channel r, Channel g, Channel b, Channel a)
{
    assert(static_cast<bool>(_impl));
    _impl->clear_color = sf::Color(r, g, b, a);
}

SfWindow::Rgb32 SfWindow::getClearColor() const
{
    assert(static_cast<bool>(_impl));
    auto const & COLOR = _impl->clear_color;
    return Rgb32{COLOR.r, COLOR.g, COLOR.b, COLOR.a};
}

void SfWindow::onBegin()
{
    // EMPTY.
}

void SfWindow::onEnd()
{
    // EMPTY.
}

void SfWindow::onPollEventBegin()
{
    // EMPTY.
}

void SfWindow::onPollEventEnd()
{
    // EMPTY.
}

void SfWindow::onClosed()
{
    close();
}

void SfWindow::onResized(unsigned int width, unsigned int height)
{
    // EMPTY.
}

void SfWindow::onLostFocus()
{
    // EMPTY.
}

void SfWindow::onGainedFocus()
{
    // EMPTY.
}

void SfWindow::onKeyPressed(Key code, bool alt, bool control, bool shift, bool system)
{
    // EMPTY.
}

void SfWindow::onKeyReleased(Key code, bool alt, bool control, bool shift, bool system)
{
    // EMPTY.
}

void SfWindow::onTextEntered(unsigned int unicode)
{
    // EMPTY.
}

void SfWindow::onMouseMoved(int x, int y)
{
    // EMPTY.
}

void SfWindow::onMouseEntered()
{
    // EMPTY.
}

void SfWindow::onMouseLeft()
{
    // EMPTY.
}

void SfWindow::onMouseButtonPressed(Button button, int x, int y)
{
    // EMPTY.
}

void SfWindow::onMouseButtonReleased(Button button, int x, int y)
{
    // EMPTY.
}

void SfWindow::onMouseWheelScrolled(Wheel wheel, float delta, int x, int y)
{
    // EMPTY.
}

void SfWindow::onJoystickConnected(unsigned int joystick_id)
{
    // EMPTY.
}

void SfWindow::onJoystickDisconnected(unsigned int joystick_id)
{
    // EMPTY.
}

void SfWindow::onJoystickMoved(unsigned int joystick_id, JoystickAxis axis, float position)
{
    // EMPTY.
}

void SfWindow::onJoystickButtonPressed(unsigned int joystick_id, unsigned int button)
{
    // EMPTY.
}

void SfWindow::onJoystickButtonReleased(unsigned int joystick_id, unsigned int button)
{
    // EMPTY.
}

void SfWindow::onTouchBegan(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void SfWindow::onTouchMoved(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void SfWindow::onTouchEnded(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void SfWindow::onSensorChanged(SensorType type, float x, float y, float z)
{
    // EMPTY.
}

void SfWindow::onIdle()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

