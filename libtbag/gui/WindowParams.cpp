/**
 * @file   WindowParams.cpp
 * @brief  WindowParams class implementation.
 * @author zer0
 * @date   2019-03-24
 */

#include <libtbag/gui/WindowParams.hpp>
#include <libtbag/Type.hpp>

#include <SFML/Window.hpp>

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

// ------------------------
// VideoMode implementation
// ------------------------

VideoMode::VideoMode()
{
    // EMPTY.
}

VideoMode::VideoMode(unsigned int w, unsigned int h, unsigned int b)
        : width(w), height(h), bpp(b)
{
    // EMPTY.
}

VideoMode::~VideoMode()
{
    // EMPTY.
}

// ---------------------------
// WindowParams implementation
// ---------------------------

WindowParams::WindowParams()
{
    // EMPTY.
}

WindowParams::~WindowParams()
{
    // EMPTY.
}

// ------------------------
// Utilities implementation
// ------------------------

VideoMode getDesktopMode()
{
    auto const mode = sf::VideoMode::getDesktopMode();
    return VideoMode(mode.width, mode.height, mode.bitsPerPixel);
}

std::vector<VideoMode> getFullscreenModes()
{
    std::vector<VideoMode> result;
    for (auto const & mode : sf::VideoMode::getFullscreenModes()) {
        result.emplace_back(mode.width, mode.height, mode.bitsPerPixel);
    }
    return result;
}

bool readWindowParams(libtbag::res::Storage & storage, WindowParams & params)
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
    __READ_WINDOW_PARAMS(width);
    __READ_WINDOW_PARAMS(height);
    __READ_WINDOW_PARAMS(bpp);
    __READ_WINDOW_PARAMS(style);
    __READ_WINDOW_PARAMS(depth_bits);
    __READ_WINDOW_PARAMS(stencil_bits);
    __READ_WINDOW_PARAMS(antialiasing_level);
    __READ_WINDOW_PARAMS(major_version);
    __READ_WINDOW_PARAMS(minor_version);
    __READ_WINDOW_PARAMS(attribute_flags);
    __READ_WINDOW_PARAMS(srgb_capable);
    __READ_WINDOW_PARAMS(clear_red);
    __READ_WINDOW_PARAMS(clear_green);
    __READ_WINDOW_PARAMS(clear_blue);
    __READ_WINDOW_PARAMS(clear_alpha);
#undef __READ_WINDOW_PARAMS
    return true;
}

bool saveWindowParams(libtbag::res::Storage & storage, WindowParams const & params)
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
    __SAVE_WINDOW_PARAMS(width);
    __SAVE_WINDOW_PARAMS(height);
    __SAVE_WINDOW_PARAMS(bpp);
    __SAVE_WINDOW_PARAMS(style);
    __SAVE_WINDOW_PARAMS(depth_bits);
    __SAVE_WINDOW_PARAMS(stencil_bits);
    __SAVE_WINDOW_PARAMS(antialiasing_level);
    __SAVE_WINDOW_PARAMS(major_version);
    __SAVE_WINDOW_PARAMS(minor_version);
    __SAVE_WINDOW_PARAMS(attribute_flags);
    __SAVE_WINDOW_PARAMS(srgb_capable);
    __SAVE_WINDOW_PARAMS(clear_red);
    __SAVE_WINDOW_PARAMS(clear_green);
    __SAVE_WINDOW_PARAMS(clear_blue);
    __SAVE_WINDOW_PARAMS(clear_alpha);
#undef __SAVE_WINDOW_PARAMS
    return true;
}

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

