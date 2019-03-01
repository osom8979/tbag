/**
 * @file   Ngc.hpp
 * @brief  Ngc class prototype.
 * @author zer0
 * @date   2019-02-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_NGC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_NGC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/util/BufferInfo.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace typography {
namespace font       {

TBAG_API libtbag::util::Buffer getNgcNormal();
TBAG_API libtbag::util::Buffer getNgcBold();

} // namespace font
} // namespace typography

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TYPOGRAPHY_FONT_NGC_HPP__

