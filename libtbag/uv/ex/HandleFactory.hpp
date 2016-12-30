/**
 * @file   HandleFactory.hpp
 * @brief  HandleFactory class prototype.
 * @author zer0
 * @date   2016-12-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_HANDLEFACTORY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_HANDLEFACTORY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uv/Type.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// Forward declaration.
class Loop;
class Handle;

namespace ex {

TBAG_API Handle * createHandle(Loop & loop, UvHandleType type);

} // namespace ex
} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_HANDLEFACTORY_HPP__

