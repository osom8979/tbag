/**
 * @file   BufferInfo.hpp
 * @brief  BufferInfo class prototype.
 * @author zer0
 * @date   2017-06-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BUFFERINFO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BUFFERINFO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdlib>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

/**
 * Buffer information structure.
 *
 * @author zer0
 * @date   2016-12-27
 */
template <typename BaseType, typename SizeType>
struct BufferInfo
{
    static_assert(std::is_pointer<BaseType>::value == false,
                  "The BaseType must not be a pointer type.");
    static_assert(std::is_reference<BaseType>::value == false,
                  "The BaseType must not be a reference type.");

    using Base = BaseType;
    using Size = SizeType;

    Base * buffer;
    Size size;

    BufferInfo(Base * b = nullptr, Size s = 0) : buffer(b), size(s)
    { /* EMPTY. */ }

    ~BufferInfo()
    { /* EMPTY. */ }

    inline void set(Base * b, Size s) TBAG_NOEXCEPT
    { buffer = b; size = s; }
};

/** Short name of BufferInfo type. */
using  binf = BufferInfo<char      , std::size_t>;
using cbinf = BufferInfo<char const, std::size_t>;

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BUFFERINFO_HPP__

