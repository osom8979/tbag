/**
 * @file   Texture.hpp
 * @brief  Texture class prototype.
 * @author zer0
 * @date   2019-02-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_TEXTURE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_TEXTURE_HPP__

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

// Forward declaration.
class Image;
class RenderWindow;

/**
 * Texture class prototype.
 *
 * @author zer0
 * @date   2019-02-19
 */
class TBAG_API Texture : public SfNative
{
public:
    enum class CoordinateType
    {
        CT_NORMALIZED,
        CT_PIXELS
    };

public:
    Texture();
    Texture(void * handle, no_init_no_ref_t);
    Texture(Texture const & obj);
    Texture(Texture && obj) TBAG_NOEXCEPT;
    ~Texture();

public:
    Texture & operator =(Texture const & obj);
    Texture & operator =(Texture && obj) TBAG_NOEXCEPT;

public:
    void swap(Texture & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Texture & lh, Texture & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    bool create(unsigned int width, unsigned int height);

public:
    bool loadFromFile(std::string const & filename, Recti const & area = Recti());
    bool loadFromMemory(void const * data, std::size_t size, Recti const & area = Recti());
    bool loadFromImage(Image const & image, Recti const & area = Recti());

public:
    Sizeu getSize() const;
    Image copyToImage() const;

public:
    void update(std::uint8_t const * pixels);
    void update(std::uint8_t const * pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y);

    void update(Texture const & texture);
    void update(Texture const & texture, unsigned int x, unsigned int y);

    void update(Image const & image);
    void update(Image const & image, unsigned int x, unsigned int y);

    void update(RenderWindow const & window);
    void update(RenderWindow const & window, unsigned int x, unsigned int y);

public:
    void setSmooth(bool smooth);
    bool isSmooth() const;

    void setSrgb(bool srgb);
    bool isSrgb() const;

    void setRepeated(bool repeated);
    bool isRepeated() const;

public:
    bool generateMipmap();
    unsigned int getNativeHandle();

public:
    static void bind(Texture const & texture, CoordinateType type = CoordinateType::CT_NORMALIZED);
    static unsigned int getMaximumSize();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_TEXTURE_HPP__

