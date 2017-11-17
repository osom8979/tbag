/**
 * @file   Font.cpp
 * @brief  Font class implementation.
 * @author zer0
 * @date   2017-11-16
 */

#include <libtbag/typography/Font.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
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

} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

