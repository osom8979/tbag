/**
 * @file   Color.hpp
 * @brief  Color class prototype.
 * @author zer0
 * @date   2016-04-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_COLOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_COLOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdint>

#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

/** Color channel type. */
typedef uint8_t Channel;

static_assert(std::is_pod<Channel>::value, "Channel is not POD type.");

/**
 * Minimum channel value.
 */
inline constexpr Channel channel_min() noexcept
{
    return std::numeric_limits<Channel>::min();
}

/**
 * Maximum channel value.
 */
inline constexpr Channel channel_max() noexcept
{
    return std::numeric_limits<Channel>::max();
}

/**
 * Half channel value.
 */
inline constexpr Channel channel_half() noexcept
{
    static_assert((channel_min() + channel_max()) != 0, "Divide by zero is NaN.");
    return static_cast<Channel>((channel_min() + channel_max()) / 2);
}

/**
 * RGBA Color structure.
 *
 * @author zer0
 * @date   2015-06-10
 * @date   2015-08-23 (Move the world library)
 * @date   2015-10-18 (Change to the trivial type)
 */
struct Color
{
    Channel r, g, b, a;
};

static_assert(std::is_trivial<Color>::value, "Color is not trivial type.");

/**
 * @defgroup __DOXYGEN_GROUP__PREDEFINED_COLORS__ Predefined colors
 * @brief Predefined colors.
 * @{
 */

Color const BLACK_COLOR = {channel_min(), channel_min(), channel_min(), channel_max()};
Color const WHITE_COLOR = {channel_max(), channel_max(), channel_max(), channel_max()};

Color const RED_COLOR   = {channel_max(), channel_min(), channel_min(), channel_max()};
Color const GREEN_COLOR = {channel_min(), channel_max(), channel_min(), channel_max()};
Color const BLUE_COLOR  = {channel_min(), channel_min(), channel_max(), channel_max()};

Color const GRAY_COLOR  = {211, 211, 211, channel_max()};

// ------------
// Flat colors.
// ------------

Color const TURQUOISE_COLOR     = { 26, 188, 156, channel_max()};
Color const EMERALD_COLOR       = { 46, 204, 113, channel_max()};
Color const PETER_RIVER_COLOR   = { 52, 152, 219, channel_max()};
Color const AMETHYST_COLOR      = {155,  89, 182, channel_max()};
Color const WET_ASPHALT_COLOR   = { 52,  73,  94, channel_max()};

Color const GREEN_SEA_COLOR     = { 22, 160, 133, channel_max()};
Color const NEPHRITIS_COLOR     = { 39, 174,  96, channel_max()};
Color const BELIZE_HOLE_COLOR   = { 41, 128, 185, channel_max()};
Color const WISTERIA_COLOR      = {142,  68, 173, channel_max()};
Color const MIDNIGHT_BLUE_COLOR = { 44,  62,  80, channel_max()};

Color const SUN_FLOWER_COLOR    = {241, 196,  15, channel_max()};
Color const CARROT_COLOR        = {230, 126,  34, channel_max()};
Color const ALIZARIN_COLOR      = {231,  76,  60, channel_max()};
Color const CLOUDS_COLOR        = {236, 240, 241, channel_max()};
Color const CONCRETE_COLOR      = {149, 165, 166, channel_max()};

Color const ORANGE_COLOR        = {243, 156,  18, channel_max()};
Color const PUMPKIN_COLOR       = {211,  84,   0, channel_max()};
Color const POMEGRANATE_COLOR   = {192,  57,  43, channel_max()};
Color const SILVER_COLOR        = {189, 195, 199, channel_max()};
Color const ASBESTOS_COLOR      = {127, 140, 141, channel_max()};

/**
 * @}
 */

// --------
// Utility.
// --------

/** The Rect are equal? */
inline bool operator == (Color const & c1, Color const & c2) noexcept
{
    if (/**/c1.r != c2.r
         || c1.g != c2.g
         || c1.b != c2.b
         || c1.a != c2.a) {
        return false;
    }
    return true;
}

inline bool operator != (Color const & c1, Color const & c2) noexcept
{
    return !(c1 == c2);
}

/** Create color instance. */
constexpr Color makeColor(Channel r = channel_max()
                        , Channel g = channel_max()
                        , Channel b = channel_max()
                        , Channel a = channel_max()) noexcept
{
    return Color{r, g, b, a};
}

/** Inverse color. */
inline Color inverse(Color const & c) noexcept
{
    return Color{static_cast<Channel>(channel_max() ^ c.r)
               , static_cast<Channel>(channel_max() ^ c.g)
               , static_cast<Channel>(channel_max() ^ c.b)
               , c.a};
}


// -------------------
// Stream overloading.
// -------------------

char const __COLOR_PREFIX_CHAR             = 'C';
char const __COLOR_STREAM_BRACE_OPEN       = '{';
char const __COLOR_STREAM_VALUE_SEPARATOR  = ',';
char const __COLOR_STREAM_BRACE_CLOSE      = '}';

inline std::string
toString(Color const & color)
{
    std::stringstream ss;
    ss << __COLOR_PREFIX_CHAR << __COLOR_STREAM_BRACE_OPEN
       << color.r << __COLOR_STREAM_VALUE_SEPARATOR
       << color.g << __COLOR_STREAM_VALUE_SEPARATOR
       << color.b << __COLOR_STREAM_VALUE_SEPARATOR
       << color.a << __COLOR_STREAM_BRACE_CLOSE;
    return ss.str();
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_COLOR_HPP__

