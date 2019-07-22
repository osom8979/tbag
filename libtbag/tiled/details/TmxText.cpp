/**
 * @file   TmxText.cpp
 * @brief  TmxText class implementation.
 * @author zer0
 * @date   2019-06-23
 */

#include <libtbag/tiled/details/TmxText.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tiled   {
namespace details {

TmxText::TmxText()
        : fontfamily(),
          pixelsize(),
          wrap(),
          color(),
          bold(),
          italic(),
          underline(),
          strikeout(),
          kerning(),
          halign(HorizontalAlignment::NONE),
          valign(VerticalAlignment::NONE),
          data()
{
    // EMPTY.
}

TmxText::TmxText(std::string const & f,
                 int p,
                 int w,
                 Color const & c,
                 int b,
                 int i,
                 int u,
                 int s,
                 int k,
                 HorizontalAlignment h,
                 VerticalAlignment v)
        : fontfamily(f),
          pixelsize(p),
          wrap(w),
          color(c),
          bold(b),
          italic(i),
          underline(u),
          strikeout(s),
          kerning(k),
          halign(h),
          valign(v),
          data()
{
    // EMPTY.
}

TmxText::~TmxText()
{
    // EMPTY.
}

TmxText::HorizontalAlignment TmxText::getHorizontalAlignment(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_LEFT) {
        return HorizontalAlignment::LEFT;
    } else if (text == VAL_CENTER) {
        return HorizontalAlignment::CENTER;
    } else if (text == VAL_RIGHT) {
        return HorizontalAlignment::RIGHT;
    } else if (text == VAL_JUSTIFY) {
        return HorizontalAlignment::JUSTIFY;
    } else {
        return HorizontalAlignment::NONE;
    }
}

char const * const TmxText::getHorizontalAlignmentName(HorizontalAlignment h) TBAG_NOEXCEPT
{
    // clang-format off
    switch (h) {
    case HorizontalAlignment::LEFT:    return VAL_LEFT;
    case HorizontalAlignment::CENTER:  return VAL_CENTER;
    case HorizontalAlignment::RIGHT:   return VAL_RIGHT;
    case HorizontalAlignment::JUSTIFY: return VAL_JUSTIFY;
    case HorizontalAlignment::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

TmxText::VerticalAlignment TmxText::getVerticalAlignment(std::string const & text) TBAG_NOEXCEPT
{
    if (text == VAL_TOP) {
        return VerticalAlignment::TOP;
    } else if (text == VAL_CENTER) {
        return VerticalAlignment::CENTER;
    } else if (text == VAL_BOTTOM) {
        return VerticalAlignment::BOTTOM;
    } else {
        return VerticalAlignment::NONE;
    }
}

char const * const TmxText::getVerticalAlignmentName(VerticalAlignment v) TBAG_NOEXCEPT
{
    // clang-format off
    switch (v) {
    case VerticalAlignment::TOP:    return VAL_TOP;
    case VerticalAlignment::CENTER: return VAL_CENTER;
    case VerticalAlignment::BOTTOM: return VAL_BOTTOM;
    case VerticalAlignment::NONE:
        TBAG_FALLTHROUGH
    default:
        return "";
    }
    // clang-format on
}

Err TmxText::read(Element const & elem)
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }

    optAttr(elem, ATT_FONTFAMILY, fontfamily, VAL_DEFAULT_FONTFAMILY);
    optAttr(elem, ATT_PIXELSIZE, pixelsize, VAL_DEFAULT_PIXELSIZE);
    optAttr(elem, ATT_WRAP, wrap, VAL_DEFAULT_WRAP);

    std::string color_text;
    optAttr(elem, ATT_COLOR, color_text, VAL_DEFAULT_COLOR);
    color.fromArgbString(color_text);

    optAttr(elem, ATT_BOLD, bold, VAL_DEFAULT_BOLD);
    optAttr(elem, ATT_ITALIC, italic, VAL_DEFAULT_ITALIC);
    optAttr(elem, ATT_UNDERLINE, underline, VAL_DEFAULT_UNDERLINE);
    optAttr(elem, ATT_STRIKEOUT, strikeout, VAL_DEFAULT_STRIKEOUT);
    optAttr(elem, ATT_KERNING, kerning, VAL_DEFAULT_KERNING);

    std::string halign_text;
    optAttr(elem, ATT_HALIGN, halign_text, VAL_DEFAULT_HALIGN);
    halign = getHorizontalAlignment(halign_text);
    if (halign == HorizontalAlignment::NONE) {
        // Unknown HorizontalAlignment value.
    }

    std::string valign_text;
    optAttr(elem, ATT_VALIGN, valign_text, VAL_DEFAULT_VALIGN);
    valign = getVerticalAlignment(valign_text);
    if (valign == VerticalAlignment::NONE) {
        // Unknown VerticalAlignment value.
    }

    data = text(elem);

    return E_SUCCESS;
}

Err TmxText::read(std::string const & xml)
{
    Document doc;
    auto const CODE = readFromXmlText(doc, xml);
    if (isFailure(CODE)) {
        return CODE;
    }
    auto const * elem = doc.FirstChildElement(TAG_NAME);
    assert(elem != nullptr);
    return read(*elem);
}

Err TmxText::write(Element & elem) const
{
    if (strncmp(elem.Name(), TAG_NAME, libtbag::string::string_length(TAG_NAME)) != 0) {
        return E_ILLARGS;
    }
    setAttr(elem, ATT_FONTFAMILY, fontfamily);
    setAttr(elem, ATT_PIXELSIZE, pixelsize);
    setAttr(elem, ATT_WRAP, wrap);
    setAttr(elem, ATT_COLOR, color.toArgbString());
    setAttr(elem, ATT_BOLD, bold);
    setAttr(elem, ATT_ITALIC, italic);
    setAttr(elem, ATT_UNDERLINE, underline);
    setAttr(elem, ATT_STRIKEOUT, strikeout);
    setAttr(elem, ATT_KERNING, kerning);
    setAttr(elem, ATT_HALIGN, getHorizontalAlignmentName(halign));
    setAttr(elem, ATT_VALIGN, getVerticalAlignmentName(valign));
    text(elem, data);
    return E_SUCCESS;
}

Err TmxText::write(std::string & xml) const
{
    Document doc;
    auto * new_elem = newElement(doc, TAG_NAME);
    assert(new_elem != nullptr);
    auto const CODE = write(*new_elem);
    if (isFailure(CODE)) {
        return CODE;
    }
    insertElement(doc, new_elem);
    return writeToXmlText(doc, xml);
}

} // namespace details
} // namespace tiled

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

