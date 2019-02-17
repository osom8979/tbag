/**
 * @file   SfRenderWindow.cpp
 * @brief  SfRenderWindow class implementation.
 * @author zer0
 * @date   2019-01-31
 * @date   2019-02-13 (Rename: Window -> SfWindow)
 * @date   2019-02-16 (Rename: SfWindow -> SfRenderWindow)
 */

#include <libtbag/gui/SfRenderWindow.hpp>
#include <libtbag/gui/SfView.hpp>
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

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

SfRenderWindow::SfRenderWindow() : SfRenderWindow(Params())
{
    // EMPTY.
}

SfRenderWindow::SfRenderWindow(Params const & params)
        : SfRenderTarget(SfType::ST_RENDER_WINDOW, no_init)
{
    auto const MODE = sf::VideoMode(params.width, params.height, params.bpp);
    auto const STYLE = sf::Style::Default;
    auto const CONTEXT = sf::ContextSettings();

    assert(ptr == nullptr);
    ptr = new sf::RenderWindow(MODE, params.title, STYLE, CONTEXT);
    assert(ptr != nullptr);
}

SfRenderWindow::SfRenderWindow(SfRenderWindow && obj) TBAG_NOEXCEPT
        : SfRenderTarget(SfType::ST_RENDER_WINDOW, no_init)
{
    *this = std::move(obj);
}

SfRenderWindow::~SfRenderWindow()
{
    // EMPTY.
}

SfRenderWindow & SfRenderWindow::operator =(SfRenderWindow && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void SfRenderWindow::swap(SfRenderWindow & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        SfNative::swap(obj);
    }
}

int SfRenderWindow::run()
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

#ifndef _self_sf
#define _self_sf() Parent::cast<sf::RenderWindow>()
#endif

using Pointi = SfRenderWindow::Pointi;
using Pointu = SfRenderWindow::Pointu;
using Pointf = SfRenderWindow::Pointf;
using Sizeu  = SfRenderWindow::Sizeu;
using Recti  = SfRenderWindow::Recti;

void SfRenderWindow::clear(Channel r, Channel g, Channel b, Channel a)
{
    _self_sf()->clear(sf::Color(r, g, b, a));
}

void SfRenderWindow::clear(Rgb24 const & color)
{
    clear(color.r, color.g, color.b);
}

void SfRenderWindow::clear(Rgb32 const & color)
{
    clear(color.r, color.g, color.b, color.a);
}

void SfRenderWindow::clear()
{
    clear(CHANNEL_MIN, CHANNEL_MIN, CHANNEL_MIN);
}

void SfRenderWindow::setView(SfView const & view)
{
    _self_sf()->setView(*((sf::View*)view.get()));
}

SfView SfRenderWindow::getView() const
{
    return SfView((void*)&(_self_sf()->getView()), no_init, false);
}

SfView SfRenderWindow::getDefaultView() const
{
    return SfView((void*)&(_self_sf()->getDefaultView()), no_init, false);
}

Recti SfRenderWindow::getViewport(SfView const & view) const
{
    auto const RECT = _self_sf()->getViewport(*((sf::View*)view.get()));
    return Recti(RECT.left, RECT.top, RECT.width, RECT.height);
}

Pointf SfRenderWindow::mapPixelToCoords(Pointi const & point) const
{
    auto const COORDS = _self_sf()->mapPixelToCoords(sf::Vector2i(point.x, point.y));
    return Pointf(COORDS.x, COORDS.y);
}

Pointf SfRenderWindow::mapPixelToCoords(Pointi const & point, SfView const & view) const
{
    auto const COORDS = _self_sf()->mapPixelToCoords(sf::Vector2i(point.x, point.y), *((sf::View*)view.get()));
    return Pointf(COORDS.x, COORDS.y);
}

Pointi SfRenderWindow::mapCoordsToPixel(Pointf const & point) const
{
    auto const PIXEL = _self_sf()->mapCoordsToPixel(sf::Vector2f(point.x, point.y));
    return Pointi(PIXEL.x, PIXEL.y);
}

Pointi SfRenderWindow::mapCoordsToPixel(Pointf const & point, SfView const & view) const
{
    auto const PIXEL = _self_sf()->mapCoordsToPixel(sf::Vector2f(point.x, point.y), *((sf::View*)view.get()));
    return Pointi(PIXEL.x, PIXEL.y);
}

Pointi SfRenderWindow::getPosition() const
{
    auto const POS = _self_sf()->getPosition();
    return Pointi(POS.x, POS.y);
}

void SfRenderWindow::setPosition(Pointi const & position)
{
    _self_sf()->setPosition(sf::Vector2i(position.x, position.y));
}

Sizeu SfRenderWindow::getSize() const
{
    auto const SIZE = _self_sf()->getSize();
    return Sizeu(SIZE.x, SIZE.y);
}

void SfRenderWindow::setSize(Sizeu const & size)
{
    _self_sf()->setSize(sf::Vector2u(size.width, size.height));
}

bool SfRenderWindow::setActive(bool active)
{
    return _self_sf()->setActive(active);
}

void SfRenderWindow::pushGLStates()
{
    _self_sf()->pushGLStates();
}

void SfRenderWindow::popGLStates()
{
    _self_sf()->popGLStates();
}

void SfRenderWindow::resetGLStates()
{
    _self_sf()->resetGLStates();
}

void SfRenderWindow::close()
{
    _self_sf()->close();
}

bool SfRenderWindow::isOpen() const
{
    return _self_sf()->isOpen();
}

void SfRenderWindow::setTitle(std::string const & title)
{
    _self_sf()->setTitle(sf::String(title));
}

void SfRenderWindow::setIcon(unsigned int width, unsigned int height, std::uint8_t const * pixels)
{
    _self_sf()->setIcon(width, height, pixels);
}

void SfRenderWindow::setVisible(bool visible)
{
    _self_sf()->setVisible(visible);
}

void SfRenderWindow::setVerticalSyncEnabled(bool enabled)
{
    _self_sf()->setVerticalSyncEnabled(enabled);
}

void SfRenderWindow::setMouseCursorVisible(bool visible)
{
    _self_sf()->setMouseCursorVisible(visible);
}

void SfRenderWindow::setMouseCursorGrabbed(bool grabbed)
{
    _self_sf()->setMouseCursorGrabbed(grabbed);
}

//void SfRenderWindow::setMouseCursor(Cursor const & cursor)
//{
//}

void SfRenderWindow::setKeyRepeatEnabled(bool enabled)
{
    _self_sf()->setKeyRepeatEnabled(enabled);
}

void SfRenderWindow::setFramerateLimit(unsigned int limit)
{
    _self_sf()->setFramerateLimit(limit);
}

void SfRenderWindow::setJoystickThreshold(float threshold)
{
    _self_sf()->setJoystickThreshold(threshold);
}

void SfRenderWindow::requestFocus()
{
    _self_sf()->requestFocus();
}

bool SfRenderWindow::hasFocus() const
{
    return _self_sf()->hasFocus();
}

void SfRenderWindow::display()
{
    _self_sf()->display();
}

#undef _self_sf

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

