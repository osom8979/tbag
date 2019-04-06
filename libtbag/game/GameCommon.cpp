/**
 * @file   GameCommon.cpp
 * @brief  GameCommon class implementation.
 * @author zer0
 * @date   2019-04-06
 */

#include <libtbag/game/GameCommon.hpp>
#include <libtbag/log/Log.hpp>

#include <SFML/Window.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {

STATIC_ASSERT_INTEGER_EQUAL(sf::Style::None,       GameParams::STYLE_NONE);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::Titlebar,   GameParams::STYLE_TITLEBAR);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::Resize,     GameParams::STYLE_RESIZE);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::Close,      GameParams::STYLE_CLOSE);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::Fullscreen, GameParams::STYLE_FULL_SCREEN);
STATIC_ASSERT_INTEGER_EQUAL(sf::Style::Default,    GameParams::STYLE_DEFAULT);

STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::Unknown, GameKey::Unknown);
STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::A,       GameKey::A);
STATIC_ASSERT_INTEGER_EQUAL(sf::Keyboard::Key::Pause,   GameKey::Pause);

STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Left,     GameButton::Left);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Right,    GameButton::Right);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::Middle,   GameButton::Middle);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::XButton1, GameButton::XButton1);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Button::XButton2, GameButton::XButton2);

STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Wheel::VerticalWheel,   GameWheel::VerticalWheel);
STATIC_ASSERT_INTEGER_EQUAL(sf::Mouse::Wheel::HorizontalWheel, GameWheel::HorizontalWheel);

STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::X,    GameJoystickAxis::X);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::Y,    GameJoystickAxis::Y);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::Z,    GameJoystickAxis::Z);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::R,    GameJoystickAxis::R);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::U,    GameJoystickAxis::U);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::V,    GameJoystickAxis::V);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::PovX, GameJoystickAxis::PovX);
STATIC_ASSERT_INTEGER_EQUAL(sf::Joystick::Axis::PovY, GameJoystickAxis::PovY);

STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Accelerometer,    GameSensorType::Accelerometer);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Gyroscope,        GameSensorType::Gyroscope);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Magnetometer,     GameSensorType::Magnetometer);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Gravity,          GameSensorType::Gravity);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::UserAcceleration, GameSensorType::UserAcceleration);
STATIC_ASSERT_INTEGER_EQUAL(sf::Sensor::Type::Orientation,      GameSensorType::Orientation);

// ------------------------
// Utilities implementation
// ------------------------

GameVideoMode getDesktopMode()
{
    auto const mode = sf::VideoMode::getDesktopMode();
    return GameVideoMode(mode.width, mode.height, mode.bitsPerPixel);
}

std::vector<GameVideoMode> getFullscreenModes()
{
    std::vector<GameVideoMode> result;
    for (auto const & mode : sf::VideoMode::getFullscreenModes()) {
        result.emplace_back(mode.width, mode.height, mode.bitsPerPixel);
    }
    return result;
}

bool readWindowParams(libtbag::res::Storage & storage, GameParams & params)
{
    std::string const CONFIG_GUI = storage->config_gui;
    if (CONFIG_GUI.empty()) {
        return false;
    }

    std::string value;
#ifndef __READ_WINDOW_PARAMS
#define __READ_WINDOW_PARAMS(name) \
        if (storage.readConfig(CONFIG_GUI, #name, value)) { \
            params.name = libtbag::string::toValue<decltype(params.name)>(value); \
        } else { \
            return false; \
        }
#endif
    __READ_WINDOW_PARAMS(width)
    __READ_WINDOW_PARAMS(height)
    __READ_WINDOW_PARAMS(bpp)
    __READ_WINDOW_PARAMS(style)
    __READ_WINDOW_PARAMS(depth_bits)
    __READ_WINDOW_PARAMS(stencil_bits)
    __READ_WINDOW_PARAMS(antialiasing_level)
    __READ_WINDOW_PARAMS(major_version)
    __READ_WINDOW_PARAMS(minor_version)
    __READ_WINDOW_PARAMS(attribute_flags)
    __READ_WINDOW_PARAMS(srgb_capable)
    __READ_WINDOW_PARAMS(clear_red)
    __READ_WINDOW_PARAMS(clear_green)
    __READ_WINDOW_PARAMS(clear_blue)
    __READ_WINDOW_PARAMS(clear_alpha)
#undef __READ_WINDOW_PARAMS
    return true;
}

bool saveWindowParams(libtbag::res::Storage & storage, GameParams const & params)
{
    std::string const CONFIG_GUI = storage->config_gui;
    if (CONFIG_GUI.empty()) {
        return false;
    }
#ifndef __SAVE_WINDOW_PARAMS
#define __SAVE_WINDOW_PARAMS(name) \
        if (!storage.saveConfig(CONFIG_GUI, #name, libtbag::string::toString(params.name))) { \
            return false; \
        }
#endif
    __SAVE_WINDOW_PARAMS(width)
    __SAVE_WINDOW_PARAMS(height)
    __SAVE_WINDOW_PARAMS(bpp)
    __SAVE_WINDOW_PARAMS(style)
    __SAVE_WINDOW_PARAMS(depth_bits)
    __SAVE_WINDOW_PARAMS(stencil_bits)
    __SAVE_WINDOW_PARAMS(antialiasing_level)
    __SAVE_WINDOW_PARAMS(major_version)
    __SAVE_WINDOW_PARAMS(minor_version)
    __SAVE_WINDOW_PARAMS(attribute_flags)
    __SAVE_WINDOW_PARAMS(srgb_capable)
    __SAVE_WINDOW_PARAMS(clear_red)
    __SAVE_WINDOW_PARAMS(clear_green)
    __SAVE_WINDOW_PARAMS(clear_blue)
    __SAVE_WINDOW_PARAMS(clear_alpha)
#undef __SAVE_WINDOW_PARAMS
    return true;
}

} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

