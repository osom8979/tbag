/**
 * @file   SfView.hpp
 * @brief  SfView class prototype.
 * @author zer0
 * @date   2019-02-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFVIEW_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFVIEW_HPP__

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
class SfTransform;

/**
 * SfView class prototype.
 *
 * @author zer0
 * @date   2019-02-17
 */
class TBAG_API SfView : public SfNative
{
public:
    SfView();
    explicit SfView(void * handle, no_init_t, bool ref = true);
    SfView(Rectf const & rectangle);
    SfView(Pointf const & center, Sizef const & size);
    SfView(SfView && obj) TBAG_NOEXCEPT;
    virtual ~SfView();

public:
    SfView & operator =(SfView && obj) TBAG_NOEXCEPT;

public:
    void swap(SfView & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(SfView & lh, SfView & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    void setCenter(float x, float y);
    void setCenter(Pointf const & center);

public:
    void setSize(float width, float height);
    void setSize(Sizef const & size);

public:
    void setRotation(float angle);
    void setViewport(Rectf const & viewport);

public:
    void reset(Rectf const & rectangle);
    Pointf getCenter() const;
    Sizef getSize() const;
    float getRotation() const;
    Rectf getViewport() const;

public:
    void move(float offset_x, float offset_y);
    void move(Pointf const & offset);
    void rotate(float angle);
    void zoom(float factor);

public:
    SfTransform getTransform() const;
    SfTransform getInverseTransform() const;
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_SFVIEW_HPP__

