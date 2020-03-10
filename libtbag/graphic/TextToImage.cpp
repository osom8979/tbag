/**
 * @file   TextToImage.cpp
 * @brief  TextToImage class implementation.
 * @author zer0
 * @date   2019-07-18
 */

#include <libtbag/graphic/TextToImage.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/typography/font/Ngc.hpp>
#include <cstdint>

#include <blend2d.h>

#define _REVERSE_RGBA_ORDER

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

Err renderCenteredText(std::string const & text,
                       int width, int height, int channels, float font_size,
                       libtbag::graphic::Color const & text_color,
                       libtbag::graphic::Color const & background_color,
                       libtbag::box::Box & output)
{
    if (width <= 0 || height <= 0) {
        return E_ILLARGS;
    }
    if (channels != 4 && channels != 3 && channels != 1) {
        return E_ILLARGS;
    }
    if (font_size <= 0.0f) {
        return E_ILLARGS;
    }

    uint32_t format_type;
    if (channels == 1) {
        format_type = BL_FORMAT_XRGB32;
    } else if (channels == 3) {
        format_type = BL_FORMAT_XRGB32;
    } else {
        assert(channels == 4);
        format_type = BL_FORMAT_PRGB32;
    }

    BLRgba32 bg;
    BLRgba32 fg;
#if defined(_REVERSE_RGBA_ORDER)
    bg.reset(background_color.b, background_color.g, background_color.r, background_color.a);
    fg.reset(text_color.b, text_color.g, text_color.r, text_color.a);
#else
    bg.reset(background_color.r, background_color.g, background_color.b, background_color.a);
    fg.reset(text_color.r, text_color.g, text_color.b, text_color.a);
#endif

    BLImage img(width, height, format_type);
    BLContext ctx(img);
    ctx.setCompOp(BL_COMP_OP_SRC_COPY);

    // Fill the background.
    ctx.setFillStyle(bg);
    ctx.fillAll();

    // Set the text color.
    ctx.setFillStyle(fg);

    if (!text.empty()) {
        BLFontLoader loader;
        auto const NGC_NORMAL = libtbag::typography::font::getNgcNormal();
        auto const create_font_result = loader.createFromData(NGC_NORMAL.data(), NGC_NORMAL.size());
        assert(create_font_result == 0);

        BLFontFace face;
        auto const create_face_result = face.createFromLoader(loader, 0);
        assert(create_face_result == 0);

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
    }
    ctx.end();

    BLImageData data = {};
    auto const obtain_result = img.getData(&data);
    assert(obtain_result == 0);

    if (channels == 1) {
        output.resize<std::uint8_t>(height, width, 1);
        auto const * source = static_cast<std::uint8_t const *>(data.pixelData);
        auto * destination = output.data<std::uint8_t>();
        auto const loop_size = height*width;
        for (auto i = 0; i < loop_size; ++i) {
            *destination = static_cast<std::uint8_t>((source[0] + source[1] + source[2]) / 3);
            destination += 1;
            source += 4;
        }
    } else if (channels == 3) {
        output.resize<std::uint8_t>(height, width, 3);
        auto const * source = static_cast<std::uint8_t const *>(data.pixelData);
        auto * destination = output.data<std::uint8_t>();
        auto const loop_size = height*width;
        for (auto i = 0; i < loop_size; ++i) {
            destination[0] = source[0];
            destination[1] = source[1];
            destination[2] = source[2];
            // destination[-] = source[3]; // Alpha value!
            destination += 3;
            source += 4;
        }
    } else {
        assert(channels == 4);
        output.resize<std::uint8_t>(height, width, 4);
        memcpy(output.data(), data.pixelData, (height*width*4));
    }
    return E_SUCCESS;
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

