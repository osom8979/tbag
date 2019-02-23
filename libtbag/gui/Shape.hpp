/**
 * @file   Shape.hpp
 * @brief  Shape class prototype.
 * @author zer0
 * @date   2019-02-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_SHAPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_SHAPE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gui/Drawable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

// Forward declaration.
class Texture;

/**
 * Shape class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API Shape : public Drawable
{
public:
    explicit Shape(SfType type, no_init_no_ref_t);
    explicit Shape(SfType type, no_init_t, bool ref = true);
    explicit Shape(SfType type, bool ref = true);
    virtual ~Shape();

public:
    void setTexture(Texture const * texture, bool resetRect = false);
    void setTextureRect(Recti const & rect);
    void setFillColor(Rgb32 const & color);
    void setOutlineColor(Rgb32 const & color);
    void setOutlineThickness(float thickness);

public:
    void * getTexturePtr() const;
    Texture getTexture() const;

public:
    Recti getTextureRect() const;
    Rgb32 getFillColor() const;
    Rgb32 getOutlineColor() const;
    float getOutlineThickness() const;

public:
    virtual std::size_t getPointCount() const;
    virtual Pointf getPoint(std::size_t index) const;

public:
    Rectf getLocalBounds() const;
    Rectf getGlobalBounds() const;
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_SHAPE_HPP__

