/**
 * @file   Image.cpp
 * @brief  Image class implementation.
 * @author zer0
 * @date   2017-06-10
 */

#include <libtbag/graphic/Image.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

Err readImage(std::string const & path, Image & image)
{
    if (filesystem::Path(path).exists() == false) {
        return Err::E_EEXIST;
    }
    //if (filesystem::Path(path).isReadable() == false) {
    //    return Err::E_RDERR;
    //}

    int   width = 0;
    int  height = 0;
    int channel = 0;

    // process data if not NULL ...
    // x = width, y = height, n = # 8-bit components per pixel ...
    // replace '0' with '1'..'4' to force that many components per pixel
    // but 'n' will always be the number that it would have been if you said 0
    unsigned char * data = ::stbi_load(path.c_str(), &width, &height, &channel, 0);
    if (data == nullptr) {
        return Err::E_RDERR;
    }

    // 1: grey
    // 2: grey, alpha
    // 3: red, green, blue
    // 4: red, green, blue, alpha
    assert(1 <= COMPARE_AND(channel) <= 4);
    assert(width > 0);
    assert(height > 0);
    image.resize(static_cast<unsigned>(width), static_cast<unsigned>(height));

    if (channel == 1) {
        std::size_t const SIZE = static_cast<std::size_t>(width * height);
        for (std::size_t i = 0; i < SIZE; ++i) {
            image[i].r = *(data + i);
            image[i].g = *(data + i);
            image[i].b = *(data + i);
        }
    } else if (channel == 2) {
        std::size_t const SIZE = static_cast<std::size_t>(width * height);
        for (std::size_t i = 0; i < SIZE; ++i) {
            image[i].r = *(data + (2 * i));
            image[i].g = *(data + (2 * i));
            image[i].b = *(data + (2 * i));
        }
    } else if (channel == 3 || channel == 4) {
        std::size_t const SIZE = static_cast<std::size_t>(width * height);
        for (std::size_t i = 0; i < SIZE; ++i) {
            image[i].r = *(data + (channel * i) + 0);
            image[i].g = *(data + (channel * i) + 1);
            image[i].b = *(data + (channel * i) + 2);
            /* image[i].a = *(data + (channel * i) + 3); */
        }
    } else {
        assert(false && "Inaccessible block.");
    }

    stbi_image_free(data);
    return Err::E_SUCCESS;
}

TBAG_CONSTEXPR static char const * const PNG_EXT = ".png";
TBAG_CONSTEXPR static char const * const BMP_EXT = ".bmp";
TBAG_CONSTEXPR static char const * const TGA_EXT = ".tga";

Err saveImage(std::string const & path, Image const & image)
{
    auto const PATH = filesystem::Path(path);
    if (PATH.exists() == true) {
        return Err::E_ALREADY;
    }

    int result = 0;
    if (string::lower(PATH.getExtensionName()) == PNG_EXT) {
        result = stbi_write_png(path.c_str(), image.width(), image.height(), 3, image.data(), image.width() * 3);
    } else if (string::lower(PATH.getExtensionName()) == BMP_EXT) {
        result = stbi_write_bmp(path.c_str(), image.width(), image.height(), 3, image.data());
    } else if (string::lower(PATH.getExtensionName()) == TGA_EXT) {
        result = stbi_write_tga(path.c_str(), image.width(), image.height(), 3, image.data());
    } else {
        return Err::E_ILLARGS;
    }

    return result != 0 ? Err::E_SUCCESS : Err::E_UNKNOWN;
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

