/**
 * @file   RenderWindow.cpp
 * @brief  RenderWindow class implementation.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> RenderWindow)
 */

#include <libtbag/gui/RenderWindow.hpp>
#include <libtbag/gui/View.hpp>
#include <libtbag/gui/Cursor.hpp>
#include <libtbag/gui/Drawable.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/Type.hpp>

#if defined(USE_GUI)
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::Unknown,         libtbag::gui::RenderWindow::Key::Unknown);
STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::A,               libtbag::gui::RenderWindow::Key::A);
STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::Pause,           libtbag::gui::RenderWindow::Key::Pause);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Left,            libtbag::gui::RenderWindow::Button::Left);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Right,           libtbag::gui::RenderWindow::Button::Right);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Middle,          libtbag::gui::RenderWindow::Button::Middle);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::XButton1,        libtbag::gui::RenderWindow::Button::XButton1);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::XButton2,        libtbag::gui::RenderWindow::Button::XButton2);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Wheel::VerticalWheel,    libtbag::gui::RenderWindow::Wheel::VerticalWheel);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Wheel::HorizontalWheel,  libtbag::gui::RenderWindow::Wheel::HorizontalWheel);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::X,              libtbag::gui::RenderWindow::JoystickAxis::X);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::Y,              libtbag::gui::RenderWindow::JoystickAxis::Y);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::Z,              libtbag::gui::RenderWindow::JoystickAxis::Z);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::R,              libtbag::gui::RenderWindow::JoystickAxis::R);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::U,              libtbag::gui::RenderWindow::JoystickAxis::U);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::V,              libtbag::gui::RenderWindow::JoystickAxis::V);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::PovX,           libtbag::gui::RenderWindow::JoystickAxis::PovX);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::PovY,           libtbag::gui::RenderWindow::JoystickAxis::PovY);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Accelerometer,    libtbag::gui::RenderWindow::SensorType::Accelerometer);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Gyroscope,        libtbag::gui::RenderWindow::SensorType::Gyroscope);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Magnetometer,     libtbag::gui::RenderWindow::SensorType::Magnetometer);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Gravity,          libtbag::gui::RenderWindow::SensorType::Gravity);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::UserAcceleration, libtbag::gui::RenderWindow::SensorType::UserAcceleration);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Orientation,      libtbag::gui::RenderWindow::SensorType::Orientation);
#else
#include <libtbag/dummy/Sfml.hpp>
using namespace libtbag::dummy;
#endif

#include <cstdlib>
#include <cassert>

#include <iostream>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

#ifndef _self_sf
#define _self_sf() Pointer::cast<sf::RenderWindow>()
#endif

using Pointi = RenderWindow::Pointi;
using Pointu = RenderWindow::Pointu;
using Pointf = RenderWindow::Pointf;
using Sizeu  = RenderWindow::Sizeu;
using Recti  = RenderWindow::Recti;

RenderWindow::RenderWindow() : RenderWindow(Params())
{
    assert(ptr != nullptr);
}

RenderWindow::RenderWindow(void * handle, no_init_no_ref_t)
        : RenderTarget(SfType::ST_RENDER_WINDOW, no_init_no_ref)
{
    assert(ptr == nullptr);
    ptr = handle;
    assert(ptr != nullptr);
}

RenderWindow::RenderWindow(Params const & params)
        : RenderTarget(SfType::ST_RENDER_WINDOW, no_init)
{
    auto const MODE = sf::VideoMode(params.width, params.height, params.bpp);
    auto const STYLE = sf::Style::Default;
    auto const CONTEXT = sf::ContextSettings();

    assert(ptr == nullptr);
    ptr = new sf::RenderWindow(MODE, params.title, STYLE, CONTEXT);
    assert(ptr != nullptr);
}

RenderWindow::RenderWindow(RenderWindow && obj) TBAG_NOEXCEPT
        : RenderTarget(SfType::ST_RENDER_WINDOW, no_init)
{
    *this = std::move(obj);
}

RenderWindow::~RenderWindow()
{
    // EMPTY.
}

RenderWindow & RenderWindow::operator =(RenderWindow && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void RenderWindow::swap(RenderWindow & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

int RenderWindow::run()
{
    auto * window = cast<sf::RenderWindow>();
    assert(window != nullptr);

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

void RenderWindow::onBegin()
{
    // EMPTY.
}

void RenderWindow::onEnd()
{
    // EMPTY.
}

void RenderWindow::onPollEventBegin()
{
    // EMPTY.
}

void RenderWindow::onPollEventEnd()
{
    // EMPTY.
}

void RenderWindow::onClosed()
{
    close();
}

void RenderWindow::onResized(unsigned int width, unsigned int height)
{
    // EMPTY.
}

void RenderWindow::onLostFocus()
{
    // EMPTY.
}

void RenderWindow::onGainedFocus()
{
    // EMPTY.
}

void RenderWindow::onKeyPressed(Key code, bool alt, bool control, bool shift, bool system)
{
    // EMPTY.
}

void RenderWindow::onKeyReleased(Key code, bool alt, bool control, bool shift, bool system)
{
    // EMPTY.
}

void RenderWindow::onTextEntered(unsigned int unicode)
{
    // EMPTY.
}

void RenderWindow::onMouseMoved(int x, int y)
{
    // EMPTY.
}

void RenderWindow::onMouseEntered()
{
    // EMPTY.
}

void RenderWindow::onMouseLeft()
{
    // EMPTY.
}

void RenderWindow::onMouseButtonPressed(Button button, int x, int y)
{
    // EMPTY.
}

void RenderWindow::onMouseButtonReleased(Button button, int x, int y)
{
    // EMPTY.
}

void RenderWindow::onMouseWheelScrolled(Wheel wheel, float delta, int x, int y)
{
    // EMPTY.
}

void RenderWindow::onJoystickConnected(unsigned int joystick_id)
{
    // EMPTY.
}

void RenderWindow::onJoystickDisconnected(unsigned int joystick_id)
{
    // EMPTY.
}

void RenderWindow::onJoystickMoved(unsigned int joystick_id, JoystickAxis axis, float position)
{
    // EMPTY.
}

void RenderWindow::onJoystickButtonPressed(unsigned int joystick_id, unsigned int button)
{
    // EMPTY.
}

void RenderWindow::onJoystickButtonReleased(unsigned int joystick_id, unsigned int button)
{
    // EMPTY.
}

void RenderWindow::onTouchBegan(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void RenderWindow::onTouchMoved(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void RenderWindow::onTouchEnded(unsigned int finger, int x, int y)
{
    // EMPTY.
}

void RenderWindow::onSensorChanged(SensorType type, float x, float y, float z)
{
    // EMPTY.
}

void RenderWindow::onIdle()
{
    // EMPTY.
}

void RenderWindow::close()
{
    _self_sf()->close();
}

bool RenderWindow::isOpen() const
{
    return _self_sf()->isOpen();
}

Pointi RenderWindow::getPosition() const
{
    auto const POS = _self_sf()->getPosition();
    return Pointi(POS.x, POS.y);
}

void RenderWindow::setPosition(Pointi const & position)
{
    _self_sf()->setPosition(sf::Vector2i(position.x, position.y));
}

void RenderWindow::setSize(Sizeu const & size)
{
    _self_sf()->setSize(sf::Vector2u(size.width, size.height));
}

void RenderWindow::setTitle(std::string const & title)
{
    _self_sf()->setTitle(sf::String(title));
}

void RenderWindow::setIcon(unsigned int width, unsigned int height, std::uint8_t const * pixels)
{
    _self_sf()->setIcon(width, height, pixels);
}

void RenderWindow::setVisible(bool visible)
{
    _self_sf()->setVisible(visible);
}

void RenderWindow::setVerticalSyncEnabled(bool enabled)
{
    _self_sf()->setVerticalSyncEnabled(enabled);
}

void RenderWindow::setMouseCursorVisible(bool visible)
{
    _self_sf()->setMouseCursorVisible(visible);
}

void RenderWindow::setMouseCursorGrabbed(bool grabbed)
{
    _self_sf()->setMouseCursorGrabbed(grabbed);
}

void RenderWindow::setMouseCursor(Cursor const & cursor)
{
    _self_sf()->setMouseCursor(*cursor.cast<sf::Cursor>());
}

void RenderWindow::setKeyRepeatEnabled(bool enabled)
{
    _self_sf()->setKeyRepeatEnabled(enabled);
}

void RenderWindow::setFramerateLimit(unsigned int limit)
{
    _self_sf()->setFramerateLimit(limit);
}

void RenderWindow::setJoystickThreshold(float threshold)
{
    _self_sf()->setJoystickThreshold(threshold);
}

void RenderWindow::requestFocus()
{
    _self_sf()->requestFocus();
}

bool RenderWindow::hasFocus() const
{
    return _self_sf()->hasFocus();
}

void RenderWindow::display()
{
    _self_sf()->display();
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

