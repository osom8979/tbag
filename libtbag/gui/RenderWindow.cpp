/**
 * @file   RenderWindow.cpp
 * @brief  RenderWindow class implementation.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> RenderWindow)
 */

#include <libtbag/gui/RenderWindow.hpp>
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

RenderWindow::RenderWindow(Params const & params)
        : SfNative(SfType::ST_RENDER_WINDOW, no_init)
{
    auto const MODE = sf::VideoMode(params.width, params.height, params.bpp);
    auto const STYLE = sf::Style::Default;
    auto const CONTEXT = sf::ContextSettings();

    assert(ptr == nullptr);
    ptr = new sf::RenderWindow(MODE, params.title, STYLE, CONTEXT);
    assert(ptr != nullptr);
}

RenderWindow::~RenderWindow()
{
    // EMPTY.
}

static void __process_sf_event__(RenderWindow * window, sf::Event & e)
{
#if defined(USE_GUI)
    // @formatter:off
    switch (e.type) {
    case sf::Event::Closed:
        window->onClosed();
        break;
    case sf::Event::Resized:
        window->onResized(e.size.width, e.size.height);
        break;
    case sf::Event::LostFocus:
        window->onLostFocus();
        break;
    case sf::Event::GainedFocus:
        window->onGainedFocus();
        break;
    case sf::Event::TextEntered:
        window->onTextEntered(e.text.unicode);
        break;
    case sf::Event::KeyPressed:
        window->onKeyPressed((RenderWindow::Key)e.key.code, e.key.alt, e.key.control, e.key.shift, e.key.system);
        break;
    case sf::Event::KeyReleased:
        window->onKeyReleased((RenderWindow::Key)e.key.code, e.key.alt, e.key.control, e.key.shift, e.key.system);
        break;
    case sf::Event::MouseWheelMoved:
        // DEPRECATED!
        break;
    case sf::Event::MouseWheelScrolled:
        window->onMouseWheelScrolled((RenderWindow::Wheel)e.mouseWheelScroll.wheel, e.mouseWheelScroll.delta, e.mouseWheelScroll.x, e.mouseWheelScroll.y);
        break;
    case sf::Event::MouseButtonPressed:
        window->onMouseButtonPressed((RenderWindow::Button)e.mouseButton.button, e.mouseButton.x, e.mouseButton.y);
        break;
    case sf::Event::MouseButtonReleased:
        window->onMouseButtonReleased((RenderWindow::Button)e.mouseButton.button, e.mouseButton.x, e.mouseButton.y);
        break;
    case sf::Event::MouseMoved:
        window->onMouseMoved(e.mouseMove.x, e.mouseMove.y);
        break;
    case sf::Event::MouseEntered:
        window->onMouseEntered();
        break;
    case sf::Event::MouseLeft:
        window->onMouseLeft();
        break;
    case sf::Event::JoystickButtonPressed:
        window->onJoystickButtonPressed(e.joystickButton.joystickId, e.joystickButton.button);
        break;
    case sf::Event::JoystickButtonReleased:
        window->onJoystickButtonReleased(e.joystickButton.joystickId, e.joystickButton.button);
        break;
    case sf::Event::JoystickMoved:
        window->onJoystickMoved(e.joystickMove.joystickId, (RenderWindow::JoystickAxis)e.joystickMove.axis, e.joystickMove.position);
        break;
    case sf::Event::JoystickConnected:
        window->onJoystickConnected(e.joystickConnect.joystickId);
        break;
    case sf::Event::JoystickDisconnected:
        window->onJoystickDisconnected(e.joystickConnect.joystickId);
        break;
    case sf::Event::TouchBegan:
        window->onTouchBegan(e.touch.finger, e.touch.x, e.touch.y);
        break;
    case sf::Event::TouchMoved:
        window->onTouchMoved(e.touch.finger, e.touch.x, e.touch.y);
        break;
    case sf::Event::TouchEnded:
        window->onTouchEnded(e.touch.finger, e.touch.x, e.touch.y);
        break;
    case sf::Event::SensorChanged:
        window->onSensorChanged((RenderWindow::SensorType)e.sensor.type, e.sensor.x, e.sensor.y, e.sensor.z);
        break;
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // @formatter:on
#endif
}

bool RenderWindow::pollEvent()
{
    sf::Event e;
    if (_self_sf()->pollEvent(e)) {
        __process_sf_event__(this, e);
        return true;
    }
    return false;
}

bool RenderWindow::waitEvent()
{
    sf::Event e;
    if (_self_sf()->waitEvent(e)) {
        __process_sf_event__(this, e);
        return true;
    }
    return false;
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

//void RenderWindow::setMouseCursor(Cursor const & cursor)
//{
//    _self_sf()->setMouseCursor(*cursor.cast<sf::Cursor>());
//}

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

void RenderWindow::clear(Channel r, Channel g, Channel b, Channel a)
{
    _self_sf()->clear(sf::Color(r, g, b, a));
}

void RenderWindow::clear(Rgb24 const & color)
{
    clear(color.r, color.g, color.b);
}

void RenderWindow::clear(Rgb32 const & color)
{
    clear(color.r, color.g, color.b, color.a);
}

void RenderWindow::clear()
{
    clear(CHANNEL_MIN, CHANNEL_MIN, CHANNEL_MIN);
}

Pointf RenderWindow::mapPixelToCoords(Pointi const & point) const
{
    auto const COORDS = _self_sf()->mapPixelToCoords(sf::Vector2i(point.x, point.y));
    return Pointf(COORDS.x, COORDS.y);
}

Pointi RenderWindow::mapCoordsToPixel(Pointf const & point) const
{
    auto const PIXEL = _self_sf()->mapCoordsToPixel(sf::Vector2f(point.x, point.y));
    return Pointi(PIXEL.x, PIXEL.y);
}

Sizeu RenderWindow::getSize() const
{
    auto const SIZE = _self_sf()->getSize();
    return Sizeu(SIZE.x, SIZE.y);
}

bool RenderWindow::setActive(bool active)
{
    return _self_sf()->setActive(active);
}

void RenderWindow::pushGLStates()
{
    _self_sf()->pushGLStates();
}

void RenderWindow::popGLStates()
{
    _self_sf()->popGLStates();
}

void RenderWindow::resetGLStates()
{
    _self_sf()->resetGLStates();
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

