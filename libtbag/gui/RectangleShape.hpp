/**
 * @file   RectangleShape.hpp
 * @brief  RectangleShape class prototype.
 * @author zer0
 * @date   2019-02-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_RECTANGLESHAPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_RECTANGLESHAPE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/gui/Shape.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * RectangleShape class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API RectangleShape : public Shape
{
public:
    RectangleShape(Sizef const & size = Sizef());
    RectangleShape(void * handle, no_init_no_ref_t);
    RectangleShape(RectangleShape && obj) TBAG_NOEXCEPT;
    virtual ~RectangleShape();

public:
    RectangleShape & operator =(RectangleShape && obj) TBAG_NOEXCEPT;

public:
    void swap(RectangleShape & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(RectangleShape & lh, RectangleShape & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    void setSize(Sizef const & size);
    Sizef getSize() const;

public:
    virtual std::size_t getPointCount() const override;
    virtual Pointf getPoint(std::size_t index) const override;
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_RECTANGLESHAPE_HPP__

