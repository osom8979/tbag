/**
 * @file   VertexBuffer.hpp
 * @brief  VertexBuffer class prototype.
 * @author zer0
 * @date   2019-02-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_VERTEXBUFFER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_VERTEXBUFFER_HPP__

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
 * VertexBuffer class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API VertexBuffer : public Drawable
{
public:
    VertexBuffer();
    VertexBuffer(VertexBuffer && obj) TBAG_NOEXCEPT;
    virtual ~VertexBuffer();

public:
    VertexBuffer & operator =(VertexBuffer && obj) TBAG_NOEXCEPT;

public:
    void swap(VertexBuffer & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(VertexBuffer & lh, VertexBuffer & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_VERTEXBUFFER_HPP__

