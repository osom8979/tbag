/**
 * @file   TextToImage.hpp
 * @brief  TextToImage class prototype.
 * @author zer0
 * @date   2019-07-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_TEXTTOIMAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_TEXTTOIMAGE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/box/Box.hpp>
#include <libtbag/graphic/Color.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graphic {

TBAG_API bool renderCenteredText(std::string const & text,
                                 int width, int height, float font_size,
                                 libtbag::graphic::Color const & text_color,
                                 libtbag::graphic::Color const & background_color,
                                 libtbag::box::Box & output /* ARGB format */);

} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_TEXTTOIMAGE_HPP__

