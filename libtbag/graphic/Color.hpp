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
#include <sstream>
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
inline TBAG_CONSTEXPR Channel channel_min() TBAG_NOEXCEPT
{
#if defined(min)
    TBAG_PUSH_MACRO(min);
#undef min
#define __RESTORE_MIN__
#endif
    return std::numeric_limits<Channel>::min();
#if defined(__RESTORE_MIN__)
    TBAG_POP_MACRO(min);
#undef __RESTORE_MIN__
#endif
}

/**
 * Maximum channel value.
 */
inline TBAG_CONSTEXPR Channel channel_max() TBAG_NOEXCEPT
{
#if defined(max)
    TBAG_PUSH_MACRO(max);
#undef max
#define __RESTORE_MAX__
#endif
    return std::numeric_limits<Channel>::max();
#if defined(__RESTORE_MAX__)
    TBAG_POP_MACRO(max);
#undef __RESTORE_MAX__
#endif
}

/**
 * Half channel value.
 */
TBAG_CONSTEXPR Channel channel_half() TBAG_NOEXCEPT
{
    return static_cast<Channel>((channel_min() + channel_max()) / 2);
}

/**
 * RGB Color structure.
 *
 * @author zer0
 * @date   2015-06-10
 * @date   2015-08-23 (Move the world library)
 * @date   2015-10-18 (Change to the trivial type)
 * @date   2016-06-10 (Remove alpha channel)
 * @date   2016-06-11 (Rename: Color -> Rgb24)
 */
struct Rgb24
{
    Channel r, g, b;
};

/** General color structure. */
using Color = Rgb24;

static_assert(std::is_trivial<Color>::value, "Color is not trivial type.");

/**
 * @defgroup __DOXYGEN_GROUP__PREDEFINED_COLORS__ Predefined colors
 * @brief Predefined colors.
 * @{
 */

Color const BLACK_COLOR = {channel_min(), channel_min(), channel_min()};
Color const WHITE_COLOR = {channel_max(), channel_max(), channel_max()};

Color const RED_COLOR   = {channel_max(), channel_min(), channel_min()};
Color const GREEN_COLOR = {channel_min(), channel_max(), channel_min()};
Color const BLUE_COLOR  = {channel_min(), channel_min(), channel_max()};

Color const GRAY_COLOR  = {211, 211, 211};

// ------------
// Flat colors.
// ------------

Color const TURQUOISE_COLOR     = { 26, 188, 156};
Color const EMERALD_COLOR       = { 46, 204, 113};
Color const PETER_RIVER_COLOR   = { 52, 152, 219};
Color const AMETHYST_COLOR      = {155,  89, 182};
Color const WET_ASPHALT_COLOR   = { 52,  73,  94};

Color const GREEN_SEA_COLOR     = { 22, 160, 133};
Color const NEPHRITIS_COLOR     = { 39, 174,  96};
Color const BELIZE_HOLE_COLOR   = { 41, 128, 185};
Color const WISTERIA_COLOR      = {142,  68, 173};
Color const MIDNIGHT_BLUE_COLOR = { 44,  62,  80};

Color const SUN_FLOWER_COLOR    = {241, 196,  15};
Color const CARROT_COLOR        = {230, 126,  34};
Color const ALIZARIN_COLOR      = {231,  76,  60};
Color const CLOUDS_COLOR        = {236, 240, 241};
Color const CONCRETE_COLOR      = {149, 165, 166};

Color const ORANGE_COLOR        = {243, 156,  18};
Color const PUMPKIN_COLOR       = {211,  84,   0};
Color const POMEGRANATE_COLOR   = {192,  57,  43};
Color const SILVER_COLOR        = {189, 195, 199};
Color const ASBESTOS_COLOR      = {127, 140, 141};

/**
 * @}
 */

// --------
// Utility.
// --------

/** The Color are equal? */
inline bool operator ==(Color const & c1, Color const & c2) TBAG_NOEXCEPT
{
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

inline bool operator !=(Color const & c1, Color const & c2) TBAG_NOEXCEPT
{
    return !(c1 == c2);
}

inline Color & operator +=(Color & c1, Color const & c2) TBAG_NOEXCEPT
{
    c1.r += c2.r;
    c1.g += c2.g;
    c1.b += c2.b;
    return c1;
}

inline Color & operator +=(Color & c1, Channel c) TBAG_NOEXCEPT
{
    c1.r += c;
    c1.g += c;
    c1.b += c;
    return c1;
}

inline Color operator +(Color const & c1, Color const & c2) TBAG_NOEXCEPT
{
    Color color = c1;
    color += c2;
    return color;
}

inline Color operator +(Color const & c1, Channel c) TBAG_NOEXCEPT
{
    Color color = c1;
    color += c;
    return color;
}

inline Color & operator -=(Color & c1, Color const & c2) TBAG_NOEXCEPT
{
    c1.r -= c2.r;
    c1.g -= c2.g;
    c1.b -= c2.b;
    return c1;
}

inline Color & operator -=(Color & c1, Channel c) TBAG_NOEXCEPT
{
    c1.r -= c;
    c1.g -= c;
    c1.b -= c;
    return c1;
}

inline Color operator -(Color const & c1, Color const & c2) TBAG_NOEXCEPT
{
    Color color = c1;
    color -= c2;
    return color;
}

inline Color operator -(Color const & c1, Channel c) TBAG_NOEXCEPT
{
    Color color = c1;
    color -= c;
    return color;
}

/** Create color instance. */
TBAG_CONSTEXPR Color makeColor(Channel r = channel_max(),
                               Channel g = channel_max(),
                               Channel b = channel_max()) TBAG_NOEXCEPT
{
    return Color{r, g, b};
}

/** Inverse color. */
inline Color inverse(Color const & c) TBAG_NOEXCEPT
{
    return Color{static_cast<Channel>(channel_max() ^ c.r),
                 static_cast<Channel>(channel_max() ^ c.g),
                 static_cast<Channel>(channel_max() ^ c.b)};
}

// -------------------
// Stream overloading.
// -------------------

TBAG_CONSTEXPR char const __COLOR_PREFIX_CHAR             = 'C';
TBAG_CONSTEXPR char const __COLOR_STREAM_BRACE_OPEN       = '{';
TBAG_CONSTEXPR char const __COLOR_STREAM_VALUE_SEPARATOR  = ',';
TBAG_CONSTEXPR char const __COLOR_STREAM_BRACE_CLOSE      = '}';

inline std::string toString(Color const & color)
{
    std::stringstream ss;
    ss << __COLOR_PREFIX_CHAR << __COLOR_STREAM_BRACE_OPEN
       << color.r << __COLOR_STREAM_VALUE_SEPARATOR
       << color.g << __COLOR_STREAM_VALUE_SEPARATOR
       << color.b << __COLOR_STREAM_BRACE_CLOSE;
    return ss.str();
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_COLOR_HPP__

