/**
 * @file   GuiSprite.hpp
 * @brief  GuiSprite class prototype.
 * @author zer0
 * @date   2019-04-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUISPRITE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUISPRITE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/game/GameCommon.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace game {
namespace gui  {

// Forward declaration.
class GuiTexture;

/**
 * GuiSprite class prototype.
 *
 * @author zer0
 * @date   2019-04-07
 */
class TBAG_API GuiSprite : public GameCommonTypes
{
public:
    struct Impl;
    friend struct Impl;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    GuiSprite();
    GuiSprite(std::nullptr_t) TBAG_NOEXCEPT;
    GuiSprite(GuiSprite const & obj) TBAG_NOEXCEPT;
    GuiSprite(GuiSprite && obj) TBAG_NOEXCEPT;
    ~GuiSprite();

public:
    GuiSprite & operator =(GuiSprite const & obj) TBAG_NOEXCEPT;
    GuiSprite & operator =(GuiSprite && obj) TBAG_NOEXCEPT;

public:
    void copy(GuiSprite const & obj) TBAG_NOEXCEPT;
    void swap(GuiSprite & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(GuiSprite & lh, GuiSprite & rh) TBAG_NOEXCEPT
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
    friend inline bool operator <(GuiSprite const & x, GuiSprite const & y) TBAG_NOEXCEPT
    { return x.get() < y.get(); }

    friend inline bool operator >(GuiSprite const & x, GuiSprite const & y) TBAG_NOEXCEPT
    { return x.get() > y.get(); }

    friend inline bool operator <=(GuiSprite const & x, GuiSprite const & y) TBAG_NOEXCEPT
    { return x.get() <= y.get(); }

    friend inline bool operator >=(GuiSprite const & x, GuiSprite const & y) TBAG_NOEXCEPT
    { return x.get() >= y.get(); }

    inline bool operator ==(GuiSprite const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(GuiSprite const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

public:
    void reset();

public:
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
    void setTexture(GuiTexture const & texture, bool reset_rect = false);
    void setTextureRect(int x, int y, int w, int h);
    void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    GuiTexture const & getTexture() const;
    void getTextureRect(int * x, int * y, int * w, int * h) const;
    void getColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a) const;

public:
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

    void setTextureRect(Rect2i const & rect);
    void setColor(Rgb32 const & color);

    Rect2i getTextureRect() const;
    Rgb32 getColor() const;
};

} // namespace gui
} // namespace game

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GAME_GUI_GUISPRITE_HPP__

