/**
 * @file   WindowParams.hpp
 * @brief  WindowParams class prototype.
 * @author zer0
 * @date   2019-03-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_WINDOWPARAMS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_WINDOWPARAMS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdint>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

struct VideoMode
{
    TBAG_CONSTEXPR static unsigned int DEFAULT_WINDOW_WIDTH  = 600;
    TBAG_CONSTEXPR static unsigned int DEFAULT_WINDOW_HEIGHT = 480;
    TBAG_CONSTEXPR static unsigned int DEFAULT_WINDOW_BPP    = 32;

    /** Window width. */
    unsigned int width = DEFAULT_WINDOW_WIDTH;

    /** Window height. */
    unsigned int height = DEFAULT_WINDOW_HEIGHT;

    /** Bits Per Pixel. */
    unsigned int bpp = DEFAULT_WINDOW_BPP;

    VideoMode()
    { /* EMPTY. */ }

    ~VideoMode()
    { /* EMPTY. */ }
};

/**
 * Window parameters.
 *
 * @author zer0
 * @date   2019-03-23
 */
struct WindowParams : public VideoMode
{
    using ContextAttributeFlagType = unsigned int;

    /** Non-debug, compatibility context (this and the core attribute are mutually exclusive) */
    TBAG_CONSTEXPR static unsigned int const ATTRIBUTE_DEFAULT_FLAG = 0;

    /** Core attribute. */
    TBAG_CONSTEXPR static unsigned int const ATTRIBUTE_CORE_FLAG = (1<<0);

    /** Debug attribute. */
    TBAG_CONSTEXPR static unsigned int const ATTRIBUTE_DEBUG_FLAG = (1<<2);

    /** No border / title bar (this flag and all others are mutually exclusive) */
    TBAG_CONSTEXPR static std::uint32_t STYLE_NONE = 0;

    /** Title bar + fixed border */
    TBAG_CONSTEXPR static std::uint32_t STYLE_TITLEBAR = 1 << 0;

    /** Title bar + resizable border + maximize button */
    TBAG_CONSTEXPR static std::uint32_t STYLE_RESIZE = 1 << 1;

    /** Title bar + close button */
    TBAG_CONSTEXPR static std::uint32_t STYLE_CLOSE = 1 << 2;

    /** Fullscreen mode (this flag and all others are mutually exclusive) */
    TBAG_CONSTEXPR static std::uint32_t STYLE_FULL_SCREEN = 1 << 3;

    /** Default window style */
    TBAG_CONSTEXPR static std::uint32_t STYLE_DEFAULT = STYLE_TITLEBAR | STYLE_RESIZE | STYLE_CLOSE;

    /** Window title. */
    std::string title = LIBTBAG_MAIN_TITLE;

    /** Window style. */
    std::uint32_t style = STYLE_DEFAULT;

    /** Bits of the depth buffer. */
    unsigned int depth_bits = 0;

    /** Bits of the stencil buffer. */
    unsigned int stencil_bits = 0;

    /** Level of antialiasing. */
    unsigned int antialiasing_level = 0;

    /** Major number of the context version to create. */
    unsigned int major_version = 1;

    /** Minor number of the context version to create. */
    unsigned int minor_version = 1;

    /** The attribute flags to create the context with. */
    ContextAttributeFlagType attribute_flags = ATTRIBUTE_DEFAULT_FLAG;

    /** Whether the context framebuffer is sRGB capable. */
    bool srgb_capable = false;

    WindowParams()
    { /* EMPTY. */ }

    ~WindowParams()
    { /* EMPTY. */ }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_WINDOWPARAMS_HPP__

