/**
 * @file   GuiTexture.hpp
 * @brief  GuiTexture class prototype.
 * @author zer0
 * @date   2019-04-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUITEXTURE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUITEXTURE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/game/GameCommon.hpp>

#include <cstdint>
#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {
namespace gui  {

// Forward declaration.
class GuiImage;

/**
 * GuiTexture class prototype.
 *
 * @author zer0
 * @date   2019-04-07
 */
class TBAG_API GuiTexture : public GameCommonTypes
{
public:
    enum class CoordinateType
    {
        CT_NORMALIZED, ///< Texture coordinates in range [0 .. 1]
        CT_PIXELS      ///< Texture coordinates in range [0 .. size]
    };

public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    GuiTexture();
    GuiTexture(std::nullptr_t) TBAG_NOEXCEPT;
    GuiTexture(GuiTexture const & obj) TBAG_NOEXCEPT;
    GuiTexture(GuiTexture && obj) TBAG_NOEXCEPT;
    ~GuiTexture();

public:
    GuiTexture & operator =(GuiTexture const & obj) TBAG_NOEXCEPT;
    GuiTexture & operator =(GuiTexture && obj) TBAG_NOEXCEPT;

public:
    void copy(GuiTexture const & obj) TBAG_NOEXCEPT;
    void swap(GuiTexture & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(GuiTexture & lh, GuiTexture & rh) TBAG_NOEXCEPT
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
    friend inline bool operator <(GuiTexture const & x, GuiTexture const & y) TBAG_NOEXCEPT
    { return x.get() < y.get(); }

    friend inline bool operator >(GuiTexture const & x, GuiTexture const & y) TBAG_NOEXCEPT
    { return x.get() > y.get(); }

    friend inline bool operator <=(GuiTexture const & x, GuiTexture const & y) TBAG_NOEXCEPT
    { return x.get() <= y.get(); }

    friend inline bool operator >=(GuiTexture const & x, GuiTexture const & y) TBAG_NOEXCEPT
    { return x.get() >= y.get(); }

    inline bool operator ==(GuiTexture const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(GuiTexture const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

public:
    void reset();

public:
    bool create(unsigned int width, unsigned int height);

public:
    bool loadFromFile(std::string const & filename, int x, int y, int w, int h);
    bool loadFromMemory(void const * data, std::size_t size, int x, int y, int w, int h);
    bool loadFromImage(GuiImage const & image, int x, int y, int w, int h);

    bool loadFromFile(std::string const & filename, Rect2i const & area);
    bool loadFromMemory(void const * data, std::size_t size, Rect2i const & area);
    bool loadFromImage(GuiImage const & image, Rect2i const & area);

    bool loadFromFile(std::string const & filename);
    bool loadFromMemory(void const * data, std::size_t size);
    bool loadFromImage(GuiImage const & image);

public:
    void getSize(int * w, int * h) const;
    Size2i getSize() const;

public:
    GuiImage copyToImage() const;

public:
    void update(std::uint8_t const * pixels);
    void update(std::uint8_t const * pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y);
    void update(GuiTexture const & texture);
    void update(GuiTexture const & texture, unsigned int x, unsigned int y);
    void update(GuiImage const & image);
    void update(GuiImage const & image, unsigned int x, unsigned int y);

public:
    void setSmooth(bool smooth);
    bool isSmooth() const;

    void setSrgb(bool srgb);
    bool isSrgb() const;

    void setRepeated(bool repeated);
    bool isRepeated() const;

    bool generateMipmap();
    unsigned int getNativeHandle() const;

public:
    static void bind(GuiTexture const & texture,
                     CoordinateType coordinate = CoordinateType::CT_NORMALIZED);
    static unsigned int getMaximumSize();
};

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUITEXTURE_HPP__

