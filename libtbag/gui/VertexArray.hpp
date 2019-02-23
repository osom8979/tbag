/**
 * @file   VertexArray.hpp
 * @brief  VertexArray class prototype.
 * @author zer0
 * @date   2019-02-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_VERTEXARRAY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_VERTEXARRAY_HPP__

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

/**
 * VertexArray class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API VertexArray : public Drawable
{
public:
    VertexArray();
    VertexArray(VertexArray && obj) TBAG_NOEXCEPT;
    virtual ~VertexArray();

public:
    VertexArray & operator =(VertexArray && obj) TBAG_NOEXCEPT;

public:
    void swap(VertexArray & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(VertexArray & lh, VertexArray & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_VERTEXARRAY_HPP__

