/**
 * @file   Vertex.hpp
 * @brief  Vertex class prototype.
 * @author zer0
 * @date   2019-02-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_VERTEX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_VERTEX_HPP__

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
 * Vertex class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API Vertex : public SfNative
{
public:
    Vertex();
    Vertex(Pointf const & position);
    Vertex(Pointf const & position, Rgb32 const & color);
    Vertex(Pointf const & position, Pointf const & tex_coords);
    Vertex(Pointf const & position, Rgb32 const & color, Pointf const & tex_coords);
    Vertex(void * handle, no_init_no_ref_t);
    Vertex(Vertex const & obj);
    Vertex(Vertex && obj) TBAG_NOEXCEPT;
    virtual ~Vertex();

public:
    Vertex & operator =(Vertex const & obj);
    Vertex & operator =(Vertex && obj) TBAG_NOEXCEPT;

public:
    void swap(Vertex & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Vertex & lh, Vertex & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    Pointf getPosition() const;
    Rgb32  getColor() const;
    Pointf getTexCoords() const;

public:
    void setPosition(Pointf const & position) const;
    void setColor(Rgb32 const & color) const;
    void setTexCoords(Pointf const & tex_coords) const;
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_VERTEX_HPP__

