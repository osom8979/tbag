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

struct Rgb24;
struct Rgb32;

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
    Channel r;
    Channel g;
    Channel b;

    Rgb24() TBAG_NOEXCEPT : r(channel_min()), g(channel_min()), b(channel_min())
    { /* EMPTY. */ }

    Rgb24(Channel r_, Channel g_, Channel b_) TBAG_NOEXCEPT : r(r_), g(g_), b(b_)
    { /* EMPTY. */ }

    Rgb24(Rgb24 const & obj) TBAG_NOEXCEPT : r(obj.r), g(obj.g), b(obj.b)
    { /* EMPTY. */ }

    ~Rgb24()
    { /* EMPTY. */ }

    Rgb24 & operator =(Rgb24 const & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            r = obj.r;
            g = obj.g;
            b = obj.b;
        }
        return *this;
    }

    void swap(Rgb24 & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(r, obj.r);
            std::swap(g, obj.g);
            std::swap(b, obj.b);
        }
    }

    friend void swap(Rgb24 & lh, Rgb24 & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

    inline bool operator ==(Rgb24 const & obj) const TBAG_NOEXCEPT
    {
        return r == obj.r && g == obj.g && b == obj.b;
    }

    inline bool operator !=(Rgb24 const & obj) const TBAG_NOEXCEPT
    {
        return !((*this) == obj);
    }

    inline Rgb24 inverse() const TBAG_NOEXCEPT
    {
        return Rgb24(static_cast<Channel>(channel_max() ^ r),
                     static_cast<Channel>(channel_max() ^ g),
                     static_cast<Channel>(channel_max() ^ b));
    }
};

/**
 * RGBA Color structure.
 *
 * @author zer0
 * @date   2019-02-10
 */
struct Rgb32
{
    Channel r;
    Channel g;
    Channel b;
    Channel a;

    Rgb32() TBAG_NOEXCEPT : r(channel_min()), g(channel_min()), b(channel_min()), a(channel_max())
    { /* EMPTY. */ }

    Rgb32(Channel r_, Channel g_, Channel b_, Channel a_ = channel_max()) TBAG_NOEXCEPT : r(r_), g(g_), b(b_), a(a_)
    { /* EMPTY. */ }

    Rgb32(Rgb32 const & obj) TBAG_NOEXCEPT : r(obj.r), g(obj.g), b(obj.b), a(obj.a)
    { /* EMPTY. */ }

    ~Rgb32()
    { /* EMPTY. */ }

    Rgb32 & operator =(Rgb32 const & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            r = obj.r;
            g = obj.g;
            b = obj.b;
            a = obj.a;
        }
        return *this;
    }

    void swap(Rgb32 & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(r, obj.r);
            std::swap(g, obj.g);
            std::swap(b, obj.b);
            std::swap(a, obj.a);
        }
    }

    friend void swap(Rgb32 & lh, Rgb32 & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

    bool operator ==(Rgb32 const & obj) const TBAG_NOEXCEPT
    {
        return r == obj.r && g == obj.g && b == obj.b && a == obj.a;
    }

    bool operator !=(Rgb32 const & obj) const TBAG_NOEXCEPT
    {
        return !((*this) == obj);
    }

    inline Rgb32 inverse() const TBAG_NOEXCEPT
    {
        return Rgb32(static_cast<Channel>(channel_max() ^ r),
                     static_cast<Channel>(channel_max() ^ g),
                     static_cast<Channel>(channel_max() ^ b));
    }
};

/** General color structure. */
using Color = Rgb24;
using Rgb   = Rgb24;
using Rgba  = Rgb32;

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

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_COLOR_HPP__

