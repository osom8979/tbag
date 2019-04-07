/**
 * @file   GuiFont.hpp
 * @brief  GuiFont class prototype.
 * @author zer0
 * @date   2019-04-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUIFONT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUIFONT_HPP__

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

/**
 * GuiFont class prototype.
 *
 * @author zer0
 * @date   2019-04-07
 */
class TBAG_API GuiFont : public GameCommonTypes
{
public:
    struct Glyph
    {
        /** Offset to move horizontally to the next character */
        float advance = 0;

        /** Bounding rectangle of the glyph, in coordinates relative to the baseline */
        Rect2f bounds;

        /** Texture coordinates of the glyph inside the font's texture */
        Rect2i texture_rect;

        Glyph() { /* EMPTY. */ }
        ~Glyph() { /* EMPTY. */ }
    };

public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    GuiFont();
    GuiFont(std::nullptr_t) TBAG_NOEXCEPT;
    GuiFont(GuiFont const & obj) TBAG_NOEXCEPT;
    GuiFont(GuiFont && obj) TBAG_NOEXCEPT;
    ~GuiFont();

public:
    GuiFont & operator =(GuiFont const & obj) TBAG_NOEXCEPT;
    GuiFont & operator =(GuiFont && obj) TBAG_NOEXCEPT;

public:
    void copy(GuiFont const & obj) TBAG_NOEXCEPT;
    void swap(GuiFont & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(GuiFont & lh, GuiFont & rh) TBAG_NOEXCEPT
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
    friend inline bool operator <(GuiFont const & x, GuiFont const & y) TBAG_NOEXCEPT
    { return x.get() < y.get(); }

    friend inline bool operator >(GuiFont const & x, GuiFont const & y) TBAG_NOEXCEPT
    { return x.get() > y.get(); }

    friend inline bool operator <=(GuiFont const & x, GuiFont const & y) TBAG_NOEXCEPT
    { return x.get() <= y.get(); }

    friend inline bool operator >=(GuiFont const & x, GuiFont const & y) TBAG_NOEXCEPT
    { return x.get() >= y.get(); }

    inline bool operator ==(GuiFont const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(GuiFont const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

public:
    void reset();

public:
    bool loadFromFile(std::string const & filename);
    bool loadFromMemory(void const * data, std::size_t size);

public:
    bool loadNanumGothicCodingBold();

public:
    std::string getFamily() const;
    Glyph getGlyph(std::uint32_t code_point, unsigned int character_size, bool bold, float outline_thickness = 0) const;
    float getKerning(std::uint32_t first, std::uint32_t second, unsigned int character_size) const;
    float getLineSpacing(unsigned int character_size) const;
    float getUnderlinePosition(unsigned int character_size) const;
    float getUnderlineThickness(unsigned int character_size) const;
};

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUIFONT_HPP__

