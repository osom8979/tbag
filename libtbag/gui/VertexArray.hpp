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
#include <libtbag/Noncopyable.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

// Forward declaration.

/**
 * VertexArray class prototype.
 *
 * @author zer0
 * @date   2019-02-23
 */
class TBAG_API VertexArray : private Noncopyable
{
private:
    // Insert member variables.

public:
    VertexArray();
    virtual ~VertexArray();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_VERTEXARRAY_HPP__

