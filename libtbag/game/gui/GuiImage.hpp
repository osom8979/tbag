/**
 * @file   GuiImage.hpp
 * @brief  GuiImage class prototype.
 * @author zer0
 * @date   2019-04-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUIIMAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUIIMAGE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/game/GameCommon.hpp>

#include <cstdint>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {
namespace gui  {

/**
 * GuiImage class prototype.
 *
 * @author zer0
 * @date   2019-04-07
 */
class TBAG_API GuiImage : public GameCommonTypes
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    GuiImage();
    GuiImage(std::nullptr_t) TBAG_NOEXCEPT;
    GuiImage(GuiImage const & obj) TBAG_NOEXCEPT;
    GuiImage(GuiImage && obj) TBAG_NOEXCEPT;
    ~GuiImage();

public:
    GuiImage & operator =(GuiImage const & obj) TBAG_NOEXCEPT;
    GuiImage & operator =(GuiImage && obj) TBAG_NOEXCEPT;

public:
    void copy(GuiImage const & obj) TBAG_NOEXCEPT;
    void swap(GuiImage & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(GuiImage & lh, GuiImage & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

    inline Impl       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline Impl const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline Impl       & ref()       TBAG_NOEXCEPT { return *get(); }
    inline Impl const & ref() const TBAG_NOEXCEPT { return *get(); }

    inline Impl       & operator *()       TBAG_NOEXCEPT { return ref(); }
    inline Impl const & operator *() const TBAG_NOEXCEPT { return ref(); }

public:
    friend inline bool operator <(GuiImage const & x, GuiImage const & y) TBAG_NOEXCEPT
    { return x.get() < y.get(); }

    friend inline bool operator >(GuiImage const & x, GuiImage const & y) TBAG_NOEXCEPT
    { return x.get() > y.get(); }

    friend inline bool operator <=(GuiImage const & x, GuiImage const & y) TBAG_NOEXCEPT
    { return x.get() <= y.get(); }

    friend inline bool operator >=(GuiImage const & x, GuiImage const & y) TBAG_NOEXCEPT
    { return x.get() >= y.get(); }

    inline bool operator ==(GuiImage const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(GuiImage const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

public:
    void reset();

public:
    void create(unsigned int width, unsigned int height,
                unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void create(unsigned int width, unsigned int height, std::uint8_t const * pixels);

public:
    bool loadFromFile(std::string const & filename);
    bool loadFromMemory(void const * data, std::size_t size);
    bool saveToFile(std::string const & filename) const;

public:
    void getSize(unsigned int * width, unsigned int * height) const;
    void createMaskFromColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a,
                             unsigned char assign_alpha = 0);
    void copyFrom(GuiImage const & source, unsigned int dest_x, unsigned int dest_y,
              int x, int y, int w, int h, bool apply_alpha = false);
    void setPixel(unsigned int x, unsigned int y,
                  unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void getPixel(unsigned int x, unsigned int y,
                  unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const;
    unsigned char const * getPixelsPtr() const;

public:
    void flipHorizontally();
    void flipVertically();

public:
    void create(Size2u const & size, Rgb32 const & color);
    Size2u getSize() const;
    void createMaskFromColor(Rgb32 const & color, unsigned char assign_alpha = 0);
    void copyFrom(GuiImage const & source, Point2u const & dest = Point2u(),
              Rect2i const & pos = Rect2i(), bool apply_alpha = false);
    void setPixel(Point2u const & pos, Rgb32 const & color);
    Rgb32 getPixel(Point2u const & pos) const;
};

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUIIMAGE_HPP__

