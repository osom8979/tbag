/**
 * @file   ImageIO.hpp
 * @brief  Image Input/Output function prototype.
 * @author zer0
 * @date   2017-06-10
 * @date   2019-02-20 (Rename: Image -> ImageIO)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGEIO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGEIO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/box/Box.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

using Box = libtbag::box::Box;

enum class ImageFileFormat
{
    IFF_PNG,
    IFF_JPG,
    IFF_BMP,
    IFF_TGA,
};

TBAG_CONSTEXPR char const * const PNG_LOWER_EXT = ".png";
TBAG_CONSTEXPR char const * const JPG_LOWER_EXT = ".jpg";
TBAG_CONSTEXPR char const * const BMP_LOWER_EXT = ".bmp";
TBAG_CONSTEXPR char const * const TGA_LOWER_EXT = ".tga";

TBAG_CONSTEXPR int const MAX_JPG_QUALITY = 100;
TBAG_CONSTEXPR int const MIN_JPG_QUALITY = 1;
TBAG_CONSTEXPR int const DEFAULT_JPG_QUALITY = 80;

TBAG_API bool writePng(char const * path, int width, int height, int channels, char const * data, int stride_bytes);
TBAG_API bool writePng(char const * path, int width, int height, int channels, char const * data);
TBAG_API bool writeJpg(char const * path, int width, int height, int channels, char const * data, int jpeg_quality);
TBAG_API bool writeJpg(char const * path, int width, int height, int channels, char const * data);
TBAG_API bool writeBmp(char const * path, int width, int height, int channels, char const * data);
TBAG_API bool writeTga(char const * path, int width, int height, int channels, char const * data);

TBAG_API bool writePng(int width, int height, int channels, char const * data, int stride_bytes,
                       libtbag::util::Buffer & buffer);
TBAG_API bool writePng(int width, int height, int channels, char const * data,
                       libtbag::util::Buffer & buffer);
TBAG_API bool writeJpg(int width, int height, int channels, char const * data, int jpeg_quality,
                       libtbag::util::Buffer & buffer);
TBAG_API bool writeJpg(int width, int height, int channels, char const * data,
                       libtbag::util::Buffer & buffer);
TBAG_API bool writeBmp(int width, int height, int channels, char const * data,
                       libtbag::util::Buffer & buffer);
TBAG_API bool writeTga(int width, int height, int channels, char const * data,
                       libtbag::util::Buffer & buffer);

/** Read image file. */
TBAG_API Err readImage(std::string const & path, Box & image);
TBAG_API Err readRgbaImage(std::string const & path, Box & image);

/** Write image file. */
TBAG_API Err writeImage(std::string const & path, Box const & image);

/** Write image buffer. */
TBAG_API Err writeImage(util::Buffer & buffer, Box const & image, ImageFileFormat format = ImageFileFormat::IFF_PNG);

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGEIO_HPP__

