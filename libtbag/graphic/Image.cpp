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

bool writePng(char const * path, int width, int height, int channels, char const * data, int stride_bytes)
{
    return stbi_write_png(path, width, height, channels, data, stride_bytes) != 0;
}

bool writePng(char const * path, int width, int height, int channels, char const * data)
{
    return writePng(path, width, height, channels, data, width * channels);
}

bool writeJpg(char const * path, int width, int height, int channels, char const * data, int jpeg_quality)
{
    return stbi_write_jpg(path, width, height, channels, data, jpeg_quality) != 0;
}

bool writeJpg(char const * path, int width, int height, int channels, char const * data)
{
    return writeJpg(path, width, height, channels, data, DEFAULT_JPG_QUALITY);
}

bool writeBmp(char const * path, int width, int height, int channels, char const * data)
{
    return stbi_write_bmp(path, width, height, channels, data) != 0;
}

bool writeTga(char const * path, int width, int height, int channels, char const * data)
{
    return stbi_write_tga(path, width, height, channels, data) != 0;
}

Err readImage(std::string const & path, ImageRgb24 & image)
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

// ---------------
namespace __impl {
// ---------------

template <typename ImageType>
static Err writeImageToFile(std::string const & path, ImageType const & image, int if_jpeg_quality)
{
    auto const PATH = filesystem::Path(path);
    if (PATH.exists() == true) {
        return Err::E_ALREADY;
    }

    auto const WIDTH     = image.width();
    auto const HEIGHT    = image.height();
    auto const CHANNELS  = GetChannels<ImageType>::channels;
    auto const * DATA    = image.data();
    auto const LOWER_EXT = string::lower(PATH.getExtensionName());

    int result = 0;
    if (LOWER_EXT == PNG_LOWER_EXT) {
        result = stbi_write_png(path.c_str(), WIDTH, HEIGHT, CHANNELS, DATA, WIDTH * CHANNELS);
    } else if (LOWER_EXT == JPG_LOWER_EXT) {
        result = stbi_write_jpg(path.c_str(), WIDTH, HEIGHT, CHANNELS, DATA, if_jpeg_quality);
    } else if (LOWER_EXT == BMP_LOWER_EXT) {
        result = stbi_write_bmp(path.c_str(), WIDTH, HEIGHT, CHANNELS, DATA);
    } else if (LOWER_EXT == TGA_LOWER_EXT) {
        result = stbi_write_tga(path.c_str(), WIDTH, HEIGHT, CHANNELS, DATA);
    } else {
        return Err::E_ILLARGS;
    }

    return result != 0 ? Err::E_SUCCESS : Err::E_UNKNOWN;
}

static void __write_image_cb__(void * context, void * data, int size)
{
    assert(context != nullptr);
    assert(data != nullptr);
    assert(size > 0);

    auto * buffer = static_cast<util::Buffer*>(context);
    assert(buffer != nullptr);

    buffer->resize(static_cast<std::size_t>(size));
    for (int i = 0; i < size; ++i) {
        (*buffer)[i] = ((util::Buffer::const_pointer)data)[i];
    }
}

template <typename ImageType>
static Err writeImageToBuffer(util::Buffer & buffer, ImageType const & image, ImageFileFormat format, int if_jpeg_quality)
{
    auto const WIDTH    = image.width();
    auto const HEIGHT   = image.height();
    auto const CHANNELS = GetChannels<ImageType>::channels;
    auto const * DATA   = image.data();

    int result = 0;
    switch (format) {
    case ImageFileFormat::IFF_PNG:
        result = stbi_write_png_to_func(&__write_image_cb__, &buffer, WIDTH, HEIGHT, CHANNELS, DATA, WIDTH * CHANNELS);
        break;
    case ImageFileFormat::IFF_JPG:
        result = stbi_write_jpg_to_func(&__write_image_cb__, &buffer, WIDTH, HEIGHT, CHANNELS, DATA, if_jpeg_quality);
        break;
    case ImageFileFormat::IFF_BMP:
        result = stbi_write_bmp_to_func(&__write_image_cb__, &buffer, WIDTH, HEIGHT, CHANNELS, DATA);
        break;
    case ImageFileFormat::IFF_TGA:
        result = stbi_write_tga_to_func(&__write_image_cb__, &buffer, WIDTH, HEIGHT, CHANNELS, DATA);
        break;
    default:
        return Err::E_ILLARGS;
    }
    return result != 0 ? Err::E_SUCCESS : Err::E_UNKNOWN;
}

// ------------------
} // namespace __impl
// ------------------

Err writeImage(std::string const & path, ImageRgb24 const & image)
{
    return __impl::writeImageToFile(path, image, DEFAULT_JPG_QUALITY);
}

Err writeImage(std::string const & path, ImageGray const & image)
{
    return __impl::writeImageToFile(path, image, DEFAULT_JPG_QUALITY);
}

Err writeImage(util::Buffer & buffer, ImageRgb24 const & image, ImageFileFormat format)
{
    return __impl::writeImageToBuffer(buffer, image, format, DEFAULT_JPG_QUALITY);
}

Err writeImage(util::Buffer & buffer, ImageGray const & image, ImageFileFormat format)
{
    return __impl::writeImageToBuffer(buffer, image, format, DEFAULT_JPG_QUALITY);
}

Err convert(ImageRgb24 const & source, ImageGray & destination)
{
    destination.resize(source.width(), source.height());
    auto const ITR = std::transform(source.begin(), source.end(), destination.begin(), [](Color const & color) -> Channel {
        return static_cast<Channel>((color.r + color.g + color.b) / 3);
    });
    if (ITR == destination.begin()) {
        return Err::E_ILLARGS;
    }
    return Err::E_SUCCESS;
}

Err convert(ImageGray const & source, ImageRgb24 & destination)
{
    destination.resize(source.width(), source.height());
    auto const ITR = std::transform(source.begin(), source.end(), destination.begin(), [](Channel const & channel) -> Color {
        return Color{channel, channel, channel};
    });
    if (ITR == destination.begin()) {
        return Err::E_ILLARGS;
    }
    return Err::E_SUCCESS;
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

