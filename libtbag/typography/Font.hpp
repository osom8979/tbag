/**
 * @file   Font.hpp
 * @brief  Font class prototype.
 * @author zer0
 * @date   2017-11-16
 *
 * @see <https://en.wikipedia.org/wiki/TrueType>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/graphic/Image.hpp>

#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/geometry/Rect.hpp>

#include <array>
#include <string>
#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace typography {

TBAG_CONSTEXPR int  const ASCII_FONT_TABLE_SIZE = 8;
TBAG_CONSTEXPR char const DEFAULT_ASCII_FONT_TABLE[ASCII_FONT_TABLE_SIZE + 1/*NULL*/] = " .:ioVM@";

TBAG_API std::string getAsciiImage(unsigned char const * true_type, std::size_t size, char c,
                                   char const * table, std::size_t table_size, int scale = 12);
TBAG_API std::string getAsciiImage(util::Buffer const & true_type, char c,
                                   std::string const & table = std::string(DEFAULT_ASCII_FONT_TABLE), int scale = 12);
TBAG_API std::string getAsciiImage(util::Buffer const & true_type, std::string const & text,
                                   std::string const & table = std::string(DEFAULT_ASCII_FONT_TABLE), int scale = 12);

/**
 * TrueType information class prototype.
 *
 * @author zer0
 * @date   2018-07-03
 */
class TBAG_API TrueType : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl  = std::unique_ptr<Impl>;
    using Buffer      = libtbag::util::Buffer;
    using BoundingBox = libtbag::geometry::Recti;
    using Size        = libtbag::geometry::Sizei;
    using ImageGray   = libtbag::graphic::ImageGray;
    using CodePoints  = std::vector<int>;

public:
    /**
     * Vertical metrics information.
     *
     * @remarks
     *  So you should advance the vertical position by @n
     *  @code{.cpp}
     *   ascent - descent + line_gap
     *  @endcode
     */
    struct VerticalMetrics
    {
        int ascent;     ///< Coordinate above the baseline the font extends.
        int descent;    ///< Coordinate below the baseline the font extends (i.e. it is typically negative).
        int line_gap;   ///< Spacing between one row's descent and the next row's ascent.
    };

    /**
     * Horizontal metrics information.
     *
     * @remarks
     *  These are expressed in unscaled coordinates.
     */
    struct HorizontalMetrics
    {
        int advance_width;     ///< Offset from the current horizontal position to the left edge of the character.
        int left_side_bearing; ///< Offset from the current horizontal position to the next horizontal position.
    };

public:
    TBAG_CONSTEXPR static int const DEFAULT_LINE_HEIGHT = 64;

private:
    UniqueImpl _impl;

public:
    TrueType();
    ~TrueType();

public:
    Err loadFromMemory(char const * buffer, std::size_t size);
    Err loadFromMemory(Buffer const & buffer);
    Err loadFromFile(std::string const & file_path);

public:
    void clear();

public:
    BoundingBox getBoundingBox() const;

public:
    Size calcSize(CodePoints const & code_points, int line_height = DEFAULT_LINE_HEIGHT) const;

public:
    Err draw(CodePoints const & code_points, ImageGray & bitmap, int line_height = DEFAULT_LINE_HEIGHT) const;
    Err drawAscii(std::string const & ascii, ImageGray & bitmap, int line_height = DEFAULT_LINE_HEIGHT) const;

private:
    void drawAndUpdate(unsigned char * bitmap, int code_point, int ascent, float scale, int image_width, int & update_next_x) const;
};

} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_HPP__

