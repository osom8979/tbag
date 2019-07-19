/**
 * @file   TextToImage.cpp
 * @brief  TextToImage class implementation.
 * @author zer0
 * @date   2019-07-18
 */

#include <libtbag/graphic/TextToImage.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/typography/font/Ngc.hpp>

#define BL_STATIC
#include <blend2d.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

bool renderCenteredText(std::string const & text,
                        int width, int height, float font_size,
                        libtbag::graphic::Color const & text_color,
                        libtbag::graphic::Color const & background_color,
                        libtbag::box::Box & output)
{
    BLImage img(width, height, BL_FORMAT_PRGB32);
    BLContext ctx(img);
    ctx.setCompOp(BL_COMP_OP_SRC_COPY);

    // Fill the background.
    ctx.setFillStyle(BLRgba32(background_color.r, background_color.g, background_color.b, background_color.a));
    ctx.fillAll();

    // Set the text color.
    ctx.setFillStyle(BLRgba32(text_color.r, text_color.g, text_color.b, text_color.a));

    BLFontLoader loader;
    auto const NGC_NORMAL = libtbag::typography::font::getNgcNormal();
    if (loader.createFromData(NGC_NORMAL.data(), NGC_NORMAL.size())) {
        return false;
    }

    BLFontFace face;
    if (face.createFromLoader(loader, 0)) {
        return false;
    }

    BLFont font;
    font.createFromFace(face, font_size);

    BLFontMetrics fm = font.metrics();
    BLTextMetrics tm = {};
    BLGlyphBuffer gb;

    using namespace libtbag::string;
    auto const lines = splitTokens(text, UNIX_NEW_LINE, false);

    auto const all_text_box_height = (fm.ascent+fm.descent+fm.lineGap) * lines.size() - (lines.size()>=1?fm.lineGap:0);
    auto const start_height = (static_cast<double>(height) - all_text_box_height) / 2.0;

    BLPoint p = {0, start_height};
    for (auto & line : lines) {
        gb.setUtf8Text(line.c_str());
        font.shape(gb);
        font.getTextMetrics(gb, tm);

        p.x = (static_cast<double>(width) - (tm.boundingBox.x1 - tm.boundingBox.x0)) / 2.0;
        ctx.fillGlyphRun(p, font, gb.glyphRun());

        p.y += fm.ascent + fm.descent + fm.lineGap;
    }
    ctx.end();

    BLImageData data = {};
    if (img.getData(&data)) {
        return false;
    }

    output.reshape<int>(height, width, 4);
    memcpy(output.data(), data.pixelData, (height*width*4));
    return true;
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

