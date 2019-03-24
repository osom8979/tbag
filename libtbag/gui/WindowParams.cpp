/**
 * @file   WindowParams.cpp
 * @brief  WindowParams class implementation.
 * @author zer0
 * @date   2019-03-24
 */

#include <libtbag/gui/WindowParams.hpp>

#include <SFML/Window.hpp>

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

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

