/**
 * @file   Image.hpp
 * @brief  Image class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_IMAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_IMAGE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gui/SfNative.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * Image class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API Image : public SfNative
{
public:
    Image();
    Image(void * handle, no_init_no_ref_t);
    Image(Image && obj) TBAG_NOEXCEPT;
    ~Image();

public:
    Image & operator =(Image && obj) TBAG_NOEXCEPT;

public:
    void swap(Image & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Image & lh, Image & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    void create(unsigned int width, unsigned int height, Rgb32 const & color = Rgb32());
    void create(unsigned int width, unsigned int height, std::uint8_t const * pixels);

public:
    bool loadFromFile(std::string const & filename);
    bool loadFromMemory(void const * data, std::size_t size);
    bool saveToFile(std::string const & filename) const;

public:
    Sizeu getSize() const;

public:
    void createMaskFromColor(Rgb32 const & color, std::uint8_t alpha = 0);

public:
    void copy(Image const & source, unsigned int dest_x, unsigned int dest_y,
              Recti const & source_rect = Recti(), bool apply_alpha = false);

public:
    void setPixel(unsigned int x, unsigned int y, Rgb32 const & color);
    Rgb32 getPixel(unsigned int x, unsigned int y) const;
    std::uint8_t const * getPixelsPtr() const;

public:
    void flipHorizontally();
    void flipVertically();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_IMAGE_HPP__

