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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Type.hpp>

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

STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::Unknown, libtbag::gui::Window::Key::Unknown);
STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::A,       libtbag::gui::Window::Key::A);
STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::Pause,   libtbag::gui::Window::Key::Pause);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Left,     libtbag::gui::Window::Button::Left);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Right,    libtbag::gui::Window::Button::Right);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Middle,   libtbag::gui::Window::Button::Middle);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::XButton1, libtbag::gui::Window::Button::XButton1);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::XButton2, libtbag::gui::Window::Button::XButton2);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Wheel::VerticalWheel,   libtbag::gui::Window::Wheel::VerticalWheel);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Wheel::HorizontalWheel, libtbag::gui::Window::Wheel::HorizontalWheel);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::X,    libtbag::gui::Window::JoystickAxis::X);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::Y,    libtbag::gui::Window::JoystickAxis::Y);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::Z,    libtbag::gui::Window::JoystickAxis::Z);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::R,    libtbag::gui::Window::JoystickAxis::R);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::U,    libtbag::gui::Window::JoystickAxis::U);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::V,    libtbag::gui::Window::JoystickAxis::V);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::PovX, libtbag::gui::Window::JoystickAxis::PovX);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::PovY, libtbag::gui::Window::JoystickAxis::PovY);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Accelerometer,    libtbag::gui::Window::SensorType::Accelerometer);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Gyroscope,        libtbag::gui::Window::SensorType::Gyroscope);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Magnetometer,     libtbag::gui::Window::SensorType::Magnetometer);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Gravity,          libtbag::gui::Window::SensorType::Gravity);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::UserAcceleration, libtbag::gui::Window::SensorType::UserAcceleration);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Orientation,      libtbag::gui::Window::SensorType::Orientation);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::None,       libtbag::gui::WindowParams::STYLE_NONE);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::Titlebar,   libtbag::gui::WindowParams::STYLE_TITLEBAR);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::Resize,     libtbag::gui::WindowParams::STYLE_RESIZE);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::Close,      libtbag::gui::WindowParams::STYLE_CLOSE);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::Fullscreen, libtbag::gui::WindowParams::STYLE_FULL_SCREEN);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::Default,    libtbag::gui::WindowParams::STYLE_DEFAULT);

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

using Pointi = Window::Pointi;
using Pointu = Window::Pointu;
using Pointf = Window::Pointf;
using Sizeu  = Window::Sizeu;
using Recti  = Window::Recti;

/**
 * Window backend implementation.
 *
 * @author zer0
 * @date   2019-03-23
 */
struct Window::Backend : private Noncopyable
{
    sf::RenderWindow window;

    Backend()
    {
        // EMPTY.
    }

    Backend(sf::VideoMode const mode, std::string const & title, sf::Uint32 style, sf::ContextSettings const context)
            : window(mode, title, style, context)
    {
        // EMPTY.
    }

    ~Backend()
    {
        // EMPTY.
    }
};

// ---------------------
// Window implementation
// ---------------------

Window::Window()
{
    // EMPTY.
}

Window::Window(WindowParams const & params)
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
    _impl = std::make_unique<Backend>(MODE, params.title, STYLE, CONTEXT);
    assert(static_cast<bool>(_impl));
}

Window::~Window()
{
    // EMPTY.
}

#ifndef _self_sf
#define _self_sf() _impl->window
#endif

static void __process_sf_event__(Window * window, sf::Event & e)
{
    window->onPreEvent();

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
        window->onKeyPressed((Window::Key)e.key.code, e.key.alt, e.key.control, e.key.shift, e.key.system);
        break;
    case sf::Event::KeyReleased:
        window->onKeyReleased((Window::Key)e.key.code, e.key.alt, e.key.control, e.key.shift, e.key.system);
        break;
    case sf::Event::MouseWheelMoved:
        // DEPRECATED!
        break;
    case sf::Event::MouseWheelScrolled:
        window->onMouseWheelScrolled((Window::Wheel)e.mouseWheelScroll.wheel, e.mouseWheelScroll.delta, e.mouseWheelScroll.x, e.mouseWheelScroll.y);
        break;
    case sf::Event::MouseButtonPressed:
        window->onMouseButtonPressed((Window::Button)e.mouseButton.button, e.mouseButton.x, e.mouseButton.y);
        break;
    case sf::Event::MouseButtonReleased:
        window->onMouseButtonReleased((Window::Button)e.mouseButton.button, e.mouseButton.x, e.mouseButton.y);
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
        window->onJoystickMoved(e.joystickMove.joystickId, (Window::JoystickAxis)e.joystickMove.axis, e.joystickMove.position);
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
        window->onSensorChanged((Window::SensorType)e.sensor.type, e.sensor.x, e.sensor.y, e.sensor.z);
        break;
    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    // @formatter:on

    window->onPostEvent();
}

bool Window::runDefault()
{
    auto & window = _self_sf();
    if (!onCreate()) {
        return false;
    }

    sf::Clock delta;
    sf::Event event;

    ImGui::SFML::Init(window);

    while (isOpen()) {
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            __process_sf_event__(this, event);
        }

        ImGui::SFML::Update(window, delta.restart());
        onUpdate();

        onPreDraw();
        ImGui::SFML::Render(window);
        onPostDraw();

        window.display();
    }

    onDestroy();
    return true;
}

void Window::onClosed()
{
    close();
}

void Window::close()
{
    _self_sf().close();
}

bool Window::isOpen() const
{
    return _self_sf().isOpen();
}

Pointi Window::getPosition() const
{
    auto const POS = _self_sf().getPosition();
    return Pointi(POS.x, POS.y);
}

void Window::setPosition(Pointi const & position)
{
    _self_sf().setPosition(sf::Vector2i(position.x, position.y));
}

void Window::setSize(Sizeu const & size)
{
    _self_sf().setSize(sf::Vector2u(size.width, size.height));
}

void Window::setTitle(std::string const & title)
{
    _self_sf().setTitle(sf::String(title));
}

void Window::setIcon(unsigned int width, unsigned int height, std::uint8_t const * pixels)
{
    _self_sf().setIcon(width, height, pixels);
}

void Window::setVisible(bool visible)
{
    _self_sf().setVisible(visible);
}

void Window::setVerticalSyncEnabled(bool enabled)
{
    _self_sf().setVerticalSyncEnabled(enabled);
}

void Window::setMouseCursorVisible(bool visible)
{
    _self_sf().setMouseCursorVisible(visible);
}

void Window::setMouseCursorGrabbed(bool grabbed)
{
    _self_sf().setMouseCursorGrabbed(grabbed);
}

void Window::setKeyRepeatEnabled(bool enabled)
{
    _self_sf().setKeyRepeatEnabled(enabled);
}

void Window::setFramerateLimit(unsigned int limit)
{
    _self_sf().setFramerateLimit(limit);
}

void Window::setJoystickThreshold(float threshold)
{
    _self_sf().setJoystickThreshold(threshold);
}

//void Window::setMouseCursor(Cursor const & cursor)
//{
//    _self_sf().setMouseCursor(*cursor.cast<sf::Cursor>());
//}

void Window::requestFocus()
{
    _self_sf().requestFocus();
}

bool Window::hasFocus() const
{
    return _self_sf().hasFocus();
}

void Window::display()
{
    _self_sf().display();
}

void Window::clear(Channel r, Channel g, Channel b, Channel a)
{
    _self_sf().clear(sf::Color(r, g, b, a));
}

void Window::clear(Rgb24 const & color)
{
    clear(color.r, color.g, color.b);
}

void Window::clear(Rgb32 const & color)
{
    clear(color.r, color.g, color.b, color.a);
}

void Window::clear()
{
    clear(CHANNEL_MIN, CHANNEL_MIN, CHANNEL_MIN);
}

Pointf Window::mapPixelToCoords(Pointi const & point) const
{
    auto const COORDS = _self_sf().mapPixelToCoords(sf::Vector2i(point.x, point.y));
    return Pointf(COORDS.x, COORDS.y);
}

Pointi Window::mapCoordsToPixel(Pointf const & point) const
{
    auto const PIXEL = _self_sf().mapCoordsToPixel(sf::Vector2f(point.x, point.y));
    return Pointi(PIXEL.x, PIXEL.y);
}

Sizeu Window::getSize() const
{
    auto const SIZE = _self_sf().getSize();
    return Sizeu(SIZE.x, SIZE.y);
}

bool Window::setActive(bool active)
{
    return _self_sf().setActive(active);
}

void Window::pushGLStates()
{
    _self_sf().pushGLStates();
}

void Window::popGLStates()
{
    _self_sf().popGLStates();
}

void Window::resetGLStates()
{
    _self_sf().resetGLStates();
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

