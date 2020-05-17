/**
 * @file   ByteString.hpp
 * @brief  ByteString class prototype.
 * @author zer0
 * @date   2020-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTESTRING_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTESTRING_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

TBAG_API std::size_t toByteSize(std::string const & str);
TBAG_API std::string toUpperByteText(std::size_t byte);

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BYTESTRING_HPP__

