/**
 * @file   GuiText.hpp
 * @brief  GuiText class prototype.
 * @author zer0
 * @date   2019-04-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUITEXT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUITEXT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/game/GameCommon.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {
namespace gui  {

// Forward declaration.
class GuiFont;

/**
 * GuiText class prototype.
 *
 * @author zer0
 * @date   2019-04-07
 */
class TBAG_API GuiText : public GameCommonTypes
{
public:
    // clang-format off
    TBAG_CONSTEXPR static unsigned const STYLE_REGULAR       = (0);
    TBAG_CONSTEXPR static unsigned const STYLE_BOLD          = (1<<0);
    TBAG_CONSTEXPR static unsigned const STYLE_ITALIC        = (1<<1);
    TBAG_CONSTEXPR static unsigned const STYLE_UNDERLINED    = (1<<2);
    TBAG_CONSTEXPR static unsigned const STYLE_STRIKETHROUGH = (1<<3);
    // clang-format on

public:
    TBAG_CONSTEXPR static unsigned const DEFAULT_CHARACTER_SIZE = 32;

public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    GuiText();
    GuiText(std::nullptr_t) TBAG_NOEXCEPT;
    GuiText(GuiText const & obj) TBAG_NOEXCEPT;
    GuiText(GuiText && obj) TBAG_NOEXCEPT;
    ~GuiText();

public:
    GuiText & operator =(GuiText const & obj) TBAG_NOEXCEPT;
    GuiText & operator =(GuiText && obj) TBAG_NOEXCEPT;

public:
    void copy(GuiText const & obj) TBAG_NOEXCEPT;
    void swap(GuiText & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(GuiText & lh, GuiText & rh) TBAG_NOEXCEPT
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
    friend inline bool operator <(GuiText const & x, GuiText const & y) TBAG_NOEXCEPT
    { return x.get() < y.get(); }

    friend inline bool operator >(GuiText const & x, GuiText const & y) TBAG_NOEXCEPT
    { return x.get() > y.get(); }

    friend inline bool operator <=(GuiText const & x, GuiText const & y) TBAG_NOEXCEPT
    { return x.get() <= y.get(); }

    friend inline bool operator >=(GuiText const & x, GuiText const & y) TBAG_NOEXCEPT
    { return x.get() >= y.get(); }

    inline bool operator ==(GuiText const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(GuiText const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

public:
    void reset();

public:
    void setFillColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void setOutlineColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void setOutlineThickness(float thickness);

    void getFillColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const;
    void getOutlineColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const;
    void getOutlineThickness(float * thickness) const;

    void getLocalBounds(float * x, float * y, float * w, float * h) const;
    void getGlobalBounds(float * x, float * y, float * w, float * h) const;

public:
    void setPosition(float x, float y);
    void setRotation(float angle);
    void setScale(float factor_x, float factor_y);
    void setOrigin(float x, float y);

    void getPosition(float * x, float * y) const;
    void getRotation(float * angle) const;
    void getScale(float * x, float * y) const;
    void getOrigin(float * x, float * y) const;

    void move(float offset_x, float offset_y);
    void rotate(float angle);
    void scale(float factor_x, float factor_y);

public:
    void setString(std::string const & string);
    void setFont(GuiFont const & font);
    void setCharacterSize(unsigned int size);
    void setLineSpacing(float spacing_factor);
    void setLetterSpacing(float spacing_factor);
    void setStyle(unsigned style);

    std::string geString() const;
    GuiFont getFont() const;
    unsigned int getCharacterSize() const;
    float getLetterSpacing() const;
    float getLineSpacing() const;
    unsigned getStyle() const;

    void findCharacterPos(std::size_t index, float * x, float * y) const;

public:
    void setFillColor(Rgb32 const & color);
    void setOutlineColor(Rgb32 const & color);

    Rgb32 getFillColor() const;
    Rgb32 getOutlineColor() const;

    Rect2f getLocalBounds() const;
    Rect2f getGlobalBounds() const;

    void setPosition(Point2f const & pos);
    void setScale(Point2f const & factor);
    void setOrigin(Point2f const & origin);

    Point2f getPosition() const;
    Point2f getScale() const;
    Point2f getOrigin() const;

    void move(Point2f const & offset);
    void scale(Point2f const & factor);
};

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUITEXT_HPP__

