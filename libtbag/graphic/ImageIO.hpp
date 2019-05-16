/**
 * @file   ImageIO.hpp
 * @brief  Image Input/Output function prototype.
 * @author zer0
 * @date   2017-06-10
 * @date   2019-02-20 (Rename: Image -> ImageIO)
 */

//#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGEIO_HPP__
//#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGEIO_HPP__
//
//// MS compatible compilers support #pragma once
//#if defined(_MSC_VER) && (_MSC_VER >= 1020)
//#pragma once
//#endif
//
//#include <libtbag/config.h>
//#include <libtbag/predef.hpp>
//#include <libtbag/Err.hpp>
//
//#include <libtbag/graphic/Color.hpp>
//#include <libtbag/container/Array2d.hpp>
//#include <libtbag/util/BufferInfo.hpp>
//
//#include <string>
//#include <vector>
//#include <algorithm>
//#include <type_traits>
//
//// -------------------
//NAMESPACE_LIBTBAG_OPEN
//// -------------------
//
//namespace graphic {
//
//using ImageGray  = libtbag::container::Array2d<Channel>;
//using ImageRgb24 = libtbag::container::Array2d<Color>;
//
//using Image = ImageRgb24;
//
//// clang-format off
//template <typename ImageType> struct GetChannels : public std::false_type
//{ TBAG_CONSTEXPR static int const channels = 0; };
//
//template <> struct GetChannels<ImageGray> : public std::true_type
//{ TBAG_CONSTEXPR static int const channels = 1; };
//
//template <> struct GetChannels<ImageRgb24> : public std::true_type
//{ TBAG_CONSTEXPR static int const channels = 3; };
//// clang-format off
//
//enum class ImageFileFormat
//{
//    IFF_PNG,
//    IFF_JPG,
//    IFF_BMP,
//    IFF_TGA,
//};
//
//TBAG_CONSTEXPR char const * const PNG_LOWER_EXT = ".png";
//TBAG_CONSTEXPR char const * const JPG_LOWER_EXT = ".jpg";
//TBAG_CONSTEXPR char const * const BMP_LOWER_EXT = ".bmp";
//TBAG_CONSTEXPR char const * const TGA_LOWER_EXT = ".tga";
//
//TBAG_CONSTEXPR int const MAX_JPG_QUALITY = 100;
//TBAG_CONSTEXPR int const MIN_JPG_QUALITY = 1;
//TBAG_CONSTEXPR int const DEFAULT_JPG_QUALITY = 80;
//
//TBAG_API bool writePng(char const * path, int width, int height, int channels, char const * data, int stride_bytes);
//TBAG_API bool writePng(char const * path, int width, int height, int channels, char const * data);
//TBAG_API bool writeJpg(char const * path, int width, int height, int channels, char const * data, int jpeg_quality);
//TBAG_API bool writeJpg(char const * path, int width, int height, int channels, char const * data);
//TBAG_API bool writeBmp(char const * path, int width, int height, int channels, char const * data);
//TBAG_API bool writeTga(char const * path, int width, int height, int channels, char const * data);
//
///**
// * Read image file.
// *
// * @warning
// *  Ignore alpha channel.
// */
//TBAG_API Err readImage(std::string const & path, ImageRgb24 & image);
//TBAG_API Err readImage(std::string const & path, libtbag::util::Buffer & image,
//                       int * width = nullptr, int * height = nullptr, int * channels = nullptr);
//
///** Write RGB24 image file. */
//TBAG_API Err writeImage(std::string const & path, ImageRgb24 const & image);
//
///** Write GRAY image file. */
//TBAG_API Err writeImage(std::string const & path, ImageGray const & image);
//
///** Write RGB24 image buffer. */
//TBAG_API Err writeImage(util::Buffer & buffer, ImageRgb24 const & image, ImageFileFormat format = ImageFileFormat::IFF_PNG);
//
///** Write GRAY image buffer. */
//TBAG_API Err writeImage(util::Buffer & buffer, ImageGray const & image, ImageFileFormat format = ImageFileFormat::IFF_PNG);
//
///** Convert RBG24 to GRAY. */
//TBAG_API Err convert(ImageRgb24 const & source, ImageGray & destination);
//
///** Convert GRAY to RGB24. */
//TBAG_API Err convert(ImageGray const & source, ImageRgb24 & destination);
//
//} // namespace graphic
//
//// --------------------
//NAMESPACE_LIBTBAG_CLOSE
//// --------------------
//
//#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGEIO_HPP__

