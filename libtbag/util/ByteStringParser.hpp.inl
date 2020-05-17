/**
 * @file   ByteStringParser.hpp
 * @brief  ByteStringParser class prototype.
 * @author zer0
 * @date   2020-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTESTRINGPARSER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTESTRINGPARSER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

/**
 * ByteStringParser class prototype.
 *
 * @author zer0
 * @date   2020-05-17
 */
struct TBAG_API ByteStringParser : private Noncopyable
{
};

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTESTRINGPARSER_HPP__

