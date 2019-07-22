/**
 * @file   Font.cpp
 * @brief  Font class implementation.
 * @author zer0
 * @date   2017-11-16
 */

#include <libtbag/typography/Font.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <cstring>
#include <sstream>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace typography {

std::string getAsciiImage(unsigned char const * true_type, std::size_t UNUSED_PARAM(size), char c,
                          char const * table, std::size_t table_size, int scale)
{
    if (table_size < ASCII_FONT_TABLE_SIZE) {
        tDLogE("getAsciiImage() Table size error: table_size({}) < ASCII_FONT_TABLE_SIZE({})",
               table_size, ASCII_FONT_TABLE_SIZE);
        return std::string();
    }

    assert(true_type != nullptr);
    assert(table != nullptr);

    stbtt_fontinfo font;
    stbtt_InitFont(&font, true_type, stbtt_GetFontOffsetForIndex(true_type, 0));

    int width  = 0;
    int height = 0;

    float scale_x = 0;
    float scale_y = stbtt_ScaleForPixelHeight(&font, scale);

    unsigned char * bitmap = stbtt_GetCodepointBitmap(&font, scale_x, scale_y, c, &width, &height, nullptr, nullptr);

    std::stringstream ss;
    unsigned char cursor = 0;
    int table_index = 0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cursor = bitmap[y * width + x];
            table_index = (cursor >> 5);
            ss << table[table_index];
        }
        ss << std::endl;
    }

    return ss.str();
}

std::string getAsciiImage(util::Buffer const & true_type, char c, std::string const & table, int scale)
{
    return getAsciiImage((unsigned char const *)true_type.data(), true_type.size(), c, table.data(), table.size(), scale);
}

std::string getAsciiImage(util::Buffer const & true_type, std::string const & text, std::string const & table, int scale)
{
    std::stringstream ss;
    std::vector<std::string> spelling;
    for (auto & c : text) {
        spelling.push_back(getAsciiImage(true_type, c, table, scale));
    }

    std::vector<std::string> line_container;
    std::string const LF = "\n";
    for (auto & str : spelling) {
        auto tokens = string::splitTokens(str, LF);

        std::size_t const SIZE = tokens.size();
        for (std::size_t i = 0; i < SIZE; ++i) {
            if (i >= line_container.size()) {
                line_container.emplace_back();
            }
            line_container[i] += tokens[i];
        }
    }

    for (auto & line : line_container) {
        ss << line << std::endl;
    }
    return ss.str();
}

// ------------------------------
// TrueType::Impl implementation.
// ------------------------------

struct TrueType::Impl : private Noncopyable
{
    TrueType * parent;

    Buffer data;
    stbtt_fontinfo info;

    Impl(TrueType * p) : parent(p)
    { /* EMPTY. */ }
    ~Impl()
    { /* EMPTY. */ }

    bool init(char const * buffer, std::size_t size)
    {
        data.assign(buffer, buffer + size);
        auto const FONT_OFFSET = stbtt_GetFontOffsetForIndex((unsigned char const *)data.data(), 0);
        auto const INIT_RESULT = stbtt_InitFont(&info, (unsigned char const *)data.data(), FONT_OFFSET);
        if (INIT_RESULT == 0) {
            return false;
        }
        return true;
    }

    void clear()
    {
        data.clear();
        memset(&info, 0x00, sizeof(info));
    }

    /** Calculate font scaling. */
    float getScale(float line_height) const
    {
        return stbtt_ScaleForPixelHeight(&info, line_height);
    }

    /** Get vertical metrics. */
    VerticalMetrics getVerticalMetrics() const
    {
        VerticalMetrics result;
        stbtt_GetFontVMetrics(&info, &result.ascent, &result.descent, &result.line_gap);
        return result;
    }

    /** Get horizontal metrics. */
    HorizontalMetrics getHorizontalMetrics(int code_point) const
    {
        HorizontalMetrics result;
        stbtt_GetCodepointHMetrics(&info, code_point, &result.advance_width, &result.left_side_bearing);
        return result;
    }

    /** an additional amount to add to the 'advance' value between ch1 and ch2 */
    int getKernAdvance(int ch1, int ch2) const
    {
        return stbtt_GetCodepointKernAdvance(&info, ch1, ch2);
    }

    /** Get bounding box for character. */
    BoundingBox getBoundingBox(int code_point, float scale_x, float scale_y) const
    {
        int x1, y1, x2, y2;
        stbtt_GetCodepointBitmapBox(&info, code_point, scale_x, scale_y, &x1, &y1, &x2, &y2);
        return BoundingBox(x1, y1, x2 - x1, y2 - y1);
    }

    /** The bounding box around all possible characters. */
    BoundingBox getBoundingBox() const
    {
        int x1, y1, x2, y2;
        stbtt_GetFontBoundingBox(&info, &x1, &y1, &x2, &y2);
        return BoundingBox(x1, y1, x2 - x1, y2 - y1);
    }

    void drawBitmap(unsigned char * bitmap, int width, int height, int bitmap_stride, float scale_x, float scale_y, int code_point) const
    {
        stbtt_MakeCodepointBitmap(&info, bitmap, width, height, bitmap_stride, scale_x, scale_y, code_point);
    }
};

// ------------------------
// TrueType implementation.
// ------------------------

TrueType::TrueType() : _impl(new Impl(this))
{
    assert(static_cast<bool>(_impl));
}

TrueType::~TrueType()
{
    // EMPTY.
}

Err TrueType::loadFromMemory(char const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_impl));
    if (_impl->init(buffer, size)) {
        return E_SUCCESS;
    }
    return E_INIT;
}

Err TrueType::loadFromMemory(Buffer const & buffer)
{
    return loadFromMemory(buffer.data(), buffer.size());
}

Err TrueType::loadFromFile(std::string const & file_path)
{
    Buffer buffer;
    auto const CODE = libtbag::filesystem::readFile(file_path, buffer);
    if (isFailure(CODE)) {
        return CODE;
    }
    return loadFromMemory(buffer);
}

void TrueType::clear()
{
    assert(static_cast<bool>(_impl));
    _impl->clear();
}

TrueType::BoundingBox TrueType::getBoundingBox() const
{
    assert(static_cast<bool>(_impl));
    return _impl->getBoundingBox();
}

TrueType::Size TrueType::calcSize(CodePoints const & code_points, int line_height) const
{
    assert(static_cast<bool>(_impl));
    auto const SCALE    = _impl->getScale(line_height);
    auto const VMETRICS = _impl->getVerticalMetrics();
    auto const ASCENT   = static_cast<int>(VMETRICS.ascent  * SCALE);
    auto const DESCENT  = static_cast<int>(VMETRICS.descent * SCALE);
    auto const SIZE     = code_points.size();

    Size result = {0, ASCENT - DESCENT};
    if (SIZE == 0) {
        return result;
    }

    assert(SIZE >= 1);
    assert(line_height > 0);

    // How wide is this first character.
    result.width += (_impl->getHorizontalMetrics(code_points[0]).advance_width * SCALE);

    for (std::size_t i = 1; i < SIZE; ++i) {
        auto const BBOX = _impl->getBoundingBox(code_points[i], SCALE, SCALE);

        // Add kerning.
        result.width += (_impl->getKernAdvance(code_points[i - 1], code_points[i]) * SCALE);

        // How wide is this character.
        result.width += (_impl->getHorizontalMetrics(code_points[i]).advance_width * SCALE);
    }

    return result;
}

//Err TrueType::draw(CodePoints const & code_points, ImageGray & bitmap, int line_height) const
//{
//    if (code_points.empty() || line_height < 0) {
//        return E_ILLARGS;
//    }
//
//    assert(static_cast<bool>(_impl));
//
//    auto const SCALE      = _impl->getScale(line_height);
//    auto const VMETRICS   = _impl->getVerticalMetrics();
//    auto const ASCENT     = static_cast<int>(VMETRICS.ascent  * SCALE);
//    auto const DESCENT    = static_cast<int>(VMETRICS.descent * SCALE);
//    auto const SIZE       = code_points.size();
//    auto const IMAGE_SIZE = calcSize(code_points, line_height);
//
//    assert(SIZE >= 1);
//    assert(line_height > 0);
//    assert(IMAGE_SIZE.area() >= 1);
//
//    bitmap.resize(IMAGE_SIZE.width, IMAGE_SIZE.height);
//
//    int next_x = 0;
//    int next_y = 0;
//
//    // Draw first character.
//    drawAndUpdate(bitmap.data(), code_points[0], ASCENT, SCALE, IMAGE_SIZE.width, next_x);
//
//    for (std::size_t i = 1; i < SIZE; ++i) {
//        next_x += (_impl->getKernAdvance(code_points[i - 1], code_points[i]) * SCALE);
//        drawAndUpdate(bitmap.data(), code_points[i], ASCENT, SCALE, IMAGE_SIZE.width, next_x);
//    }
//
//    return E_SUCCESS;
//}
//
//Err TrueType::drawAscii(std::string const & ascii, ImageGray & bitmap, int line_height) const
//{
//    return draw(CodePoints(ascii.begin(), ascii.end()), bitmap, line_height);
//}

void TrueType::drawAndUpdate(unsigned char * bitmap, int code_point, int ascent, float scale, int image_width, int & update_next_x) const
{
    // Get bounding box for character.
    // (may be offset to account for chars that dip above or below the line)
    auto const BBOX = _impl->getBoundingBox(code_point, scale, scale);

    // Different characters have different heights.
    auto const DRAW_Y = ascent + BBOX.y;

    // Render character.
    // (stride and offset is important here)
    auto const BYTE_OFFSET = update_next_x + (DRAW_Y * image_width);

    // Draw bitmap.
    _impl->drawBitmap(bitmap + BYTE_OFFSET, BBOX.width, BBOX.height, image_width, scale, scale, code_point);

    // How wide is this character.
    update_next_x += (_impl->getHorizontalMetrics(code_point).advance_width * scale);
}

} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

