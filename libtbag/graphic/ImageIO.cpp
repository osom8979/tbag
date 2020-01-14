/**
 * @file   ImageIO.cpp
 * @brief  Image Input/Output function implementation.
 * @author zer0
 * @date   2017-06-10
 * @date   2019-02-20 (Rename: Image -> ImageIO)
 */

#include <libtbag/graphic/ImageIO.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
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

static void __write_to_buffer(void * context, void * data, int size)
{
    using Buffer = libtbag::util::Buffer;
    using Value = Buffer::value_type;
    auto * buffer = (libtbag::util::Buffer*)context;
    assert(buffer != nullptr);
    auto const * begin = (Value const *)data;
    auto const * end = begin + size;
    buffer->insert(buffer->end(), begin, end);
}

bool writePng(int width, int height, int channels, char const * data, int stride_bytes, libtbag::util::Buffer & buffer)
{
    int len;
    auto * png = stbi_write_png_to_mem((unsigned char *)data, stride_bytes, width, height, channels, &len);
    if (png == nullptr) {
        return false;
    }
    buffer.assign(png, png + len);
    STBIW_FREE(png);
    return true;
}

bool writePng(int width, int height, int channels, char const * data, libtbag::util::Buffer & buffer)
{
    return writePng(width, height, channels, data, width * channels, buffer);
}

bool writeJpg(int width, int height, int channels, char const * data, int jpeg_quality, libtbag::util::Buffer & buffer)
{
    stbi__write_context s;
    s.func = &__write_to_buffer;
    s.context = (void*)&buffer;
    return stbi_write_jpg_core(&s, width, height, channels, data, jpeg_quality);
}

bool writeJpg(int width, int height, int channels, char const * data, libtbag::util::Buffer & buffer)
{
    return writeJpg(width, height, channels, data, DEFAULT_JPG_QUALITY, buffer);
}

bool writeBmp(int width, int height, int channels, char const * data, libtbag::util::Buffer & buffer)
{
    stbi__write_context s;
    s.func = &__write_to_buffer;
    s.context = (void*)&buffer;
    return stbi_write_bmp_core(&s, width, height, channels, data);
}

bool writeTga(int width, int height, int channels, char const * data, libtbag::util::Buffer & buffer)
{
    stbi__write_context s;
    s.func = &__write_to_buffer;
    s.context = (void*)&buffer;
    return stbi_write_tga_core(&s, width, height, channels, (void*)data);
}

template <typename Predicated>
static Err __read_image_cb(std::string const & path, Predicated predicated)
{
    if (!filesystem::Path(path).exists()) {
        return E_EEXIST;
    }

    int width = 0;
    int height = 0;
    int channels = 0;

    // process data if not NULL ...
    // x = width, y = height, n = # 8-bit components per pixel ...
    // replace '0' with '1'..'4' to force that many components per pixel
    // but 'n' will always be the number that it would have been if you said 0
    unsigned char * data = ::stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
        return E_RDERR;
    }

    // 1: grey
    // 2: grey, alpha
    // 3: red, green, blue
    // 4: red, green, blue, alpha
    assert(1 <= COMPARE_AND(channels) <= 4);
    assert(width >= 1);
    assert(height >= 1);

    predicated(width, height, channels, data);

    stbi_image_free(data);
    return E_SUCCESS;
}

Err readImage(std::string const & path, Box & image)
{
    return __read_image_cb(path, [&](int w, int h, int c, unsigned char * d){
        image.resize<uint8_t>(h, w, c);
        memcpy(image.data(), d, image.size());
    });
}

Err readRgbaImage(std::string const & path, Box & image)
{
    return __read_image_cb(path, [&](int w, int h, int c, unsigned char * d){
        image.resize<uint8_t>(h, w, 4);
        auto * dest = image.data<uint8_t>();
        auto const size = w*h;
        switch (c) {
        case 1:
            for (auto i = 0; i < size; ++i) {
                dest[(i*4)+0] = d[i];
                dest[(i*4)+1] = d[i];
                dest[(i*4)+2] = d[i];
                dest[(i*4)+3] = 0xFF;
            }
            break;
        case 2:
            for (auto i = 0; i < size; ++i) {
                dest[(i*4)+0] = d[(i*2)+0];
                dest[(i*4)+1] = d[(i*2)+0];
                dest[(i*4)+2] = d[(i*2)+0];
                dest[(i*4)+3] = d[(i*2)+1];
            }
            break;
        case 3:
            for (auto i = 0; i < size; ++i) {
                dest[(i*4)+0] = d[(i*3)+0];
                dest[(i*4)+1] = d[(i*3)+1];
                dest[(i*4)+2] = d[(i*3)+2];
                dest[(i*4)+3] = 0xFF;
            }
            break;
        case 4:
            memcpy(image.data(), d, image.size());
            break;
        }
    });
}

Err writeImage(std::string const & path, Box const & image)
{
    auto const PATH = filesystem::Path(path);
    if (PATH.exists()) {
        return E_ALREADY;
    }

    auto const HEIGHT    = image.dim(0);
    auto const WIDTH     = image.dim(1);
    auto const CHANNELS  = image.dim(2);
    auto const * DATA    = (char const *)image.data();
    auto const LOWER_EXT = string::lower(PATH.getExtensionName());

    int result = 0;
    if (LOWER_EXT == PNG_LOWER_EXT) {
        result = writePng(path.c_str(), WIDTH, HEIGHT, CHANNELS, DATA, WIDTH * CHANNELS);
    } else if (LOWER_EXT == JPG_LOWER_EXT) {
        result = writeJpg(path.c_str(), WIDTH, HEIGHT, CHANNELS, DATA, DEFAULT_JPG_QUALITY);
    } else if (LOWER_EXT == BMP_LOWER_EXT) {
        result = writeBmp(path.c_str(), WIDTH, HEIGHT, CHANNELS, DATA);
    } else if (LOWER_EXT == TGA_LOWER_EXT) {
        result = writeTga(path.c_str(), WIDTH, HEIGHT, CHANNELS, DATA);
    } else {
        return E_ILLARGS;
    }

    return result != 0 ? E_SUCCESS : E_UNKNOWN;
}

static void __write_image_cb(void * context, void * data, int size)
{
    assert(context != nullptr);
    assert(data != nullptr);
    assert(size > 0);

    auto * buffer = static_cast<libtbag::util::Buffer*>(context);
    assert(buffer != nullptr);

    buffer->resize(static_cast<std::size_t>(size));
    for (int i = 0; i < size; ++i) {
        (*buffer)[i] = ((libtbag::util::Buffer::const_pointer)data)[i];
    }
}

Err writeImage(util::Buffer & buffer, Box const & image, ImageFileFormat format)
{
    auto const height = image.dim(0);
    auto const width = image.dim(1);
    auto const channels = image.dim(2);
    auto const * data = (char const *)image.data();

    int result = 0;
    switch (format) {
    case ImageFileFormat::IFF_PNG:
        result = stbi_write_png_to_func(&__write_image_cb, &buffer, width, height, channels, data, width * channels);
        break;
    case ImageFileFormat::IFF_JPG:
        result = stbi_write_jpg_to_func(&__write_image_cb, &buffer, width, height, channels, data, DEFAULT_JPG_QUALITY);
        break;
    case ImageFileFormat::IFF_BMP:
        result = stbi_write_bmp_to_func(&__write_image_cb, &buffer, width, height, channels, data);
        break;
    case ImageFileFormat::IFF_TGA:
        result = stbi_write_tga_to_func(&__write_image_cb, &buffer, width, height, channels, data);
        break;
    default:
        return E_ILLARGS;
    }
    return result != 0 ? E_SUCCESS : E_UNKNOWN;
}

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

