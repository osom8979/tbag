/**
 * @file   PrimitiveType.hpp
 * @brief  PrimitiveType class prototype.
 * @author zer0
 * @date   2019-02-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_PRIMITIVETYPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_PRIMITIVETYPE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

enum class PrimitiveType
{
    PT_POINTS,
    PT_LINES,
    PT_LINE_STRIP,
    PT_TRIANGLES,
    PT_TRIANGLE_STRIP,
    PT_TRIANGLE_FAN,
    PT_QUADS,
};

TBAG_API char const * const getPrimitiveTypeName(PrimitiveType type) TBAG_NOEXCEPT;

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_PRIMITIVETYPE_HPP__

