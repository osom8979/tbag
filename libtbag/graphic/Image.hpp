/**
 * @file   Image.hpp
 * @brief  Image class prototype.
 * @author zer0
 * @date   2017-06-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/graphic/Color.hpp>
#include <libtbag/container/Array2d.hpp>

#include <string>
#include <vector>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

using ImageGray  = container::Array2d<Channel>;
using ImageRgb24 = container::Array2d<Color>;

using Image = ImageRgb24;

/**
 * Read image file.
 *
 * @warning
 *  Ignore alpha channel.
 */
TBAG_API Err readImage(std::string const & path, ImageRgb24 & image);

/** Save RGB24 image file. */
TBAG_API Err saveImage(std::string const & path, ImageRgb24 const & image);

/** Save GRAY image file. */
TBAG_API Err saveImage(std::string const & path, ImageGray const & image);

/** Convert RBG24 to GRAY. */
TBAG_API Err convert(ImageRgb24 const & source, ImageGray & destination);

/** Convert GRAY to RGB24. */
TBAG_API Err convert(ImageGray const & source, ImageRgb24 & destination);

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_IMAGE_HPP__

