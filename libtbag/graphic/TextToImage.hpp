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

/**
 * Render the center aligned text as an image.
 *
 * @param[in] text
 *      The text to render.
 * @param[in] width
 *      Output image width.
 * @param[in] channels
 *      Output image channels. (4 or 3 or 1)
 * @param[in] height
 *      Output image height.
 * @param[in] font_size
 *      Text font size.
 * @param[in] text_color
 *      Text color.
 * @param[in] background_color
 *      Background image color.
 * @param[out] output
 *      Output box container.
 *
 * @return
 *      Error code.
 */
TBAG_API Err renderCenteredText(std::string const & text,
                                int width, int height, int channels, float font_size,
                                libtbag::graphic::Color const & text_color,
                                libtbag::graphic::Color const & background_color,
                                libtbag::box::Box & output);
} // namespace graphic

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPHIC_TEXTTOIMAGE_HPP__

