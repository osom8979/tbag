/**
 * @file   SfWindow.cpp
 * @brief  SfWindow class implementation.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> SfRenderWindow)
 */

#include <libtbag/gui/SfRenderWindow.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#ifndef _STATIC_ASSERT_EQUAL
#define _STATIC_ASSERT_EQUAL(x, y) STATIC_ASSERT_CHECK_IS_EQUALS((int)(x), (int)(y))
#endif

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
_STATIC_ASSERT_EQUAL(sf::Keyboard::Key::Unknown,         libtbag::gui::SfRenderWindow::Key::Unknown);
_STATIC_ASSERT_EQUAL(sf::Keyboard::Key::A,               libtbag::gui::SfRenderWindow::Key::A);
_STATIC_ASSERT_EQUAL(sf::Keyboard::Key::Pause,           libtbag::gui::SfRenderWindow::Key::Pause);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::Left,            libtbag::gui::SfRenderWindow::Button::Left);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::Right,           libtbag::gui::SfRenderWindow::Button::Right);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::Middle,          libtbag::gui::SfRenderWindow::Button::Middle);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::XButton1,        libtbag::gui::SfRenderWindow::Button::XButton1);
_STATIC_ASSERT_EQUAL(sf::Mouse::Button::XButton2,        libtbag::gui::SfRenderWindow::Button::XButton2);
_STATIC_ASSERT_EQUAL(sf::Mouse::Wheel::VerticalWheel,    libtbag::gui::SfRenderWindow::Wheel::VerticalWheel);
_STATIC_ASSERT_EQUAL(sf::Mouse::Wheel::HorizontalWheel,  libtbag::gui::SfRenderWindow::Wheel::HorizontalWheel);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::X,              libtbag::gui::SfRenderWindow::JoystickAxis::X);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::Y,              libtbag::gui::SfRenderWindow::JoystickAxis::Y);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::Z,              libtbag::gui::SfRenderWindow::JoystickAxis::Z);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::R,              libtbag::gui::SfRenderWindow::JoystickAxis::R);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::U,              libtbag::gui::SfRenderWindow::JoystickAxis::U);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::V,              libtbag::gui::SfRenderWindow::JoystickAxis::V);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::PovX,           libtbag::gui::SfRenderWindow::JoystickAxis::PovX);
_STATIC_ASSERT_EQUAL(sf::Joystick::Axis::PovY,           libtbag::gui::SfRenderWindow::JoystickAxis::PovY);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Accelerometer,    libtbag::gui::SfRenderWindow::SensorType::Accelerometer);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Gyroscope,        libtbag::gui::SfRenderWindow::SensorType::Gyroscope);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Magnetometer,     libtbag::gui::SfRenderWindow::SensorType::Magnetometer);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Gravity,          libtbag::gui::SfRenderWindow::SensorType::Gravity);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::UserAcceleration, libtbag::gui::SfRenderWindow::SensorType::UserAcceleration);
_STATIC_ASSERT_EQUAL(sf::Sensor::Type::Orientation,      libtbag::gui::SfRenderWindow::SensorType::Orientation);
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

struct SfRenderWindow::Impl
{
    sf::Color clear_color;
};

// -----------------------
// SfRenderWindow implementation
// -----------------------

SfRenderWindow::SfRenderWindow(Params const & params)
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

SfRenderWindow::SfRenderWindow() : SfRenderWindow(Params())
{
    // EMPTY.
}

SfRenderWindow::~SfRenderWindow()
{
    // EMPTY.
}

int SfRenderWindow::run()
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

bool SfRenderWindow::isOpen() const
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    return window->isOpen();
}

void SfRenderWindow::close()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    window->close();
}

void SfRenderWindow::clear()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    assert(static_cast<bool>(_impl));
    window->clear(_impl->clear_color);
}

void SfRenderWindow::display()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);
    window->display();
}

void SfRenderWindow::setClearColor(Rgb24 const & color)
{
    setClearColor(color.r, color.g, color.b);
}

void SfRenderWindow::setClearColor(Rgb32 const & color)
{
    setClearColor(color.r, color.g, color.b, color.a);
}

void SfRenderWindow::setClearColor(Channel r, Channel g, Channel b, Channel a)
{
    assert(static_cast<bool>(_impl));
    _impl->clear_color = sf::Color(r, g, b, a);
}

SfRenderWindow::Rgb32 SfRenderWindow::getClearColor() const
{
    assert(static_cast<bool>(_impl));
    auto const & COLOR = _impl->clear_color;
    return Rgb32{COLOR.r, COLOR.g, COLOR.b, COLOR.a};
}

void SfRenderWindow::onBegin()
{
    // EMPTY.
}

void SfRenderWindow::onEnd()
{
    // EMPTY.
}

void SfRenderWindow::onPollEventBegin()
{
    // EMPTY.
}

void SfRenderWindow::onPollEventEnd()
{
    // EMPTY.
}

void SfRenderWindow::onClosed()
{
    close();
}

void SfRenderWindow::onResized(unsigned int width, unsigned int height)
{
    // EMPTY.
}

void SfRenderWindow::onLostFocus()
{
    // EMPTY.
}

void SfRenderWindow::onGainedFocus()
{
    // EMPTY.
}

void SfRenderWindow::onKeyPressed(Key code, bool alt, bool control, bool shift, bool system)
{
    // EMPTY.
}

void SfRenderWindow::onKeyReleased(Key code, bool alt, bool control, bool shift, bool system)
{
    // EMPTY.
}

void SfRenderWindow::onTextEntered(unsigned int unicode)
{
    // EMPTY.
}

void SfRenderWindow::onMouseMoved(int x, int y)
{
    // EMPTY.
}

void SfRenderWindow::onMouseEntered()
{
    // EMPTY.
}

void SfRenderWindow::onMouseLeft()
{
    // EMPTY.
}

void SfRenderWindow::onMouseButtonPressed(Button button, int x, int y)
{
    // EMPTY.
}

void SfRenderWindow::onMouseButtonReleased(Button button, int x, int y)
{
    // EMPTY.
}

void SfRenderWindow::onMouseWheelScrolled(Wheel wheel, float delta, int x, int y)
{
    // EMPTY.
}

void SfRenderWindow::onJoystickConnected(unsigned int joystick_id)
{
    // EMPTY.
}

void SfRenderWindow::onJoystickDisconnected(unsigned int joystick_id)
{
    // EMPTY.
}

void SfRenderWindow::onJoystickMoved(unsigned int joystick_id, JoystickAxis axis, float position)
{
    // EMPTY.
}

void SfRenderWindow::onJoystickButtonPressed(unsigned int joystick_id, unsigned int button)
{
    // EMPTY.
}

void SfRenderWindow::onJoystickButtonReleased(unsigned int joystick_id, unsigned int button)
{
    // EMPTY.
}

void SfRenderWindow::onTouchBegan(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void SfRenderWindow::onTouchMoved(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void SfRenderWindow::onTouchEnded(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void SfRenderWindow::onSensorChanged(SensorType type, float x, float y, float z)
{
    // EMPTY.
}

void SfRenderWindow::onIdle()
{
    // EMPTY.
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

