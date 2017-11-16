/**
 * @file   Font.cpp
 * @brief  Font class implementation.
 * @author zer0
 * @date   2017-11-16
 */

#include <libtbag/typography/Font.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/archive/ZipBase.hpp>
#include <libtbag/encrypt/Md5.hpp>

#include <cassert>
#include <sstream>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace typography {

Font::Font()
{
    // EMPTY.
}

Font::~Font()
{
    // EMPTY.
}

Err readTruetype(std::string const & path, util::Buffer & buffer)
{
    return Err::E_ENOSYS;
}

Err saveTruetype(std::string const & path, util::Buffer const & buffer)
{
    return Err::E_ENOSYS;
}

std::string drawTrueType(util::Buffer const & true_type, char c, int scale)
{
    unsigned char const * TTF_BUFFER = (unsigned char const *)true_type.data();

    stbtt_fontinfo font;
    stbtt_InitFont(&font, TTF_BUFFER, stbtt_GetFontOffsetForIndex(TTF_BUFFER, 0));

    int width  = 0;
    int height = 0;

    float scale_x = 0;
    float scale_y = stbtt_ScaleForPixelHeight(&font, scale);

    unsigned char * bitmap = stbtt_GetCodepointBitmap(&font, scale_x, scale_y, c, &width, &height, nullptr, nullptr);
    char const * const ASCII_FONT_TABLE = " .:ioVM@";

    std::stringstream ss;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char bit = bitmap[y * width + x];
            ss << ASCII_FONT_TABLE[bit >> 5];
        }
        ss << std::endl;
    }
    return ss.str();
}

/**
 * ProggyClean.ttf font binary.
 *
 * @remarks
 *  This file is Zip compressed and then Base64 encoded.
 *
 *  MIT license:
 *  @code
 *   Copyright (c) 2004, 2005 Tristan Grimmer
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the "Software"),
 *   to deal in the Software without restriction, including without limitation
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons to whom the
 *   Software is furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *   DEALINGS IN THE SOFTWARE.
 *  @endcode
 *
 * @see <http://upperbounds.net>
 * @see <http://www.upperbounds.net/download/ProggyClean.ttf.zip>
 */
TBAG_CONSTEXPR static char const * const PROGGY_CLEAN_FONT_ZIP_AND_BASE64 =
        "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";
TBAG_CONSTEXPR static char const * const PROGGY_CLEAN_FONT_MD5 = "361ffa4ca82d2a1841d178464353e955";

std::string getCompressedProggyClean()
{
    return PROGGY_CLEAN_FONT_ZIP_AND_BASE64;
}

util::Buffer getProggyCleanData()
{
    util::Buffer result;
    if (archive::decodeZipBase64(PROGGY_CLEAN_FONT_ZIP_AND_BASE64, result)) {
        result.clear();
    }
    return result;
}

bool testProggyCleanAsMd5()
{
    util::Buffer buffer = getProggyCleanData();
    std::string md5;
    if (encrypt::encryptMd5(std::string(buffer.begin(), buffer.end()), md5)) {
        return (md5 == PROGGY_CLEAN_FONT_MD5);
    }
    return false;
}

} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

