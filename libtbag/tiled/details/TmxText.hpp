/**
 * @file   TmxText.hpp
 * @brief  TmxText class prototype.
 * @author zer0
 * @date   2019-06-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTEXT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTEXT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/dom/xml/XmlHelper.hpp>
#include <libtbag/graphic/Color.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

/**
 * TmxText class prototype.
 *
 * @author zer0
 * @date   2019-06-23
 *
 * @remarks
 *  Used to mark an object as a text object.
 *  Contains the actual text as character data.
 */
struct TBAG_API TmxText : protected libtbag::dom::xml::XmlHelper
{
    using Color = libtbag::graphic::Color;

    TBAG_CONSTEXPR static char const * const TAG_NAME = "text";

    /** The font family used (default: "sans-serif") */
    TBAG_CONSTEXPR static char const * const ATT_FONTFAMILY = "fontfamily";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_FONTFAMILY = "sans-serif";

    /**
     * The size of the font in pixels
     * (not using points, because other sizes in the TMX format are also using pixels)
     * (default: 16)
     */
    TBAG_CONSTEXPR static char const * const ATT_PIXELSIZE = "pixelsize";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_PIXELSIZE = 16;

    /**
     * Whether word wrapping is enabled (1) or disabled (0).
     * Defaults to 0.
     */
    TBAG_CONSTEXPR static char const * const ATT_WRAP = "wrap";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_WRAP = 0;

    /**
     * Color of the text in \#AARRGGBB or \#RRGGBB format
     * (default: \#000000)
     */
    TBAG_CONSTEXPR static char const * const ATT_COLOR = "color";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_COLOR = "#000000";

    /**
     * Whether the font is bold (1) or not (0).
     * Defaults to 0.
     */
    TBAG_CONSTEXPR static char const * const ATT_BOLD = "bold";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_BOLD = 0;

    /**
     * Whether the font is italic (1) or not (0).
     * Defaults to 0.
     */
    TBAG_CONSTEXPR static char const * const ATT_ITALIC = "italic";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_ITALIC = 0;

    /**
     * Whether a line should be drawn below the text (1) or not (0).
     * Defaults to 0.
     */
    TBAG_CONSTEXPR static char const * const ATT_UNDERLINE = "underline";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_UNDERLINE = 0;

    /**
     * Whether a line should be drawn through the text (1) or not (0).
     * Defaults to 0.
     */
    TBAG_CONSTEXPR static char const * const ATT_STRIKEOUT = "strikeout";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_STRIKEOUT = 0;

    /**
     * Whether kerning should be used while rendering the text (1) or not (0).
     * Default to 1.
     */
    TBAG_CONSTEXPR static char const * const ATT_KERNING = "kerning";
    TBAG_CONSTEXPR static int const VAL_DEFAULT_KERNING = 1;

    /**
     * Horizontal alignment of the text within the object
     *
     * @remarks
     *  left (default), center, right or justify (since Tiled 1.2.1)
     */
    TBAG_CONSTEXPR static char const * const ATT_HALIGN = "halign";
    TBAG_CONSTEXPR static char const * const VAL_LEFT = "left";
    TBAG_CONSTEXPR static char const * const VAL_CENTER = "center";
    TBAG_CONSTEXPR static char const * const VAL_RIGHT = "right";
    TBAG_CONSTEXPR static char const * const VAL_JUSTIFY = "justify";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_HALIGN = VAL_LEFT;

    /**
     * Vertical alignment of the text within the object
     *
     * @remarks
     *  top (default), center or bottom
     */
    TBAG_CONSTEXPR static char const * const ATT_VALIGN = "valign";
    TBAG_CONSTEXPR static char const * const VAL_TOP = "top";
    TBAG_CONSTEXPR static char const * const VAL_BOTTOM = "bottom";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_VALIGN = VAL_TOP;

    enum class HorizontalAlignment
    {
        NONE, LEFT, CENTER, RIGHT, JUSTIFY
    };

    enum class VerticalAlignment
    {
        NONE, TOP, CENTER, BOTTOM
    };

    std::string fontfamily;
    int pixelsize;
    int wrap;
    Color color;
    int bold;
    int italic;
    int underline;
    int strikeout;
    int kerning;
    HorizontalAlignment halign;
    VerticalAlignment valign;

    std::string data;

    TmxText();
    TmxText(std::string const & fontfamily,
            int pixelsize,
            int wrap,
            Color const & color,
            int bold,
            int italic,
            int underline,
            int strikeout,
            int kerning,
            HorizontalAlignment halign,
            VerticalAlignment valign);
    ~TmxText();

    static HorizontalAlignment getHorizontalAlignment(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getHorizontalAlignmentName(HorizontalAlignment h) TBAG_NOEXCEPT;

    static VerticalAlignment getVerticalAlignment(std::string const & text) TBAG_NOEXCEPT;
    static char const * const getVerticalAlignmentName(VerticalAlignment v) TBAG_NOEXCEPT;

    Err read(Element const & elem);
    Err read(std::string const & xml);

    Err write(Element & elem) const;
    Err write(std::string & xml) const;
};

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TILED_DETAILS_TMXTEXT_HPP__

