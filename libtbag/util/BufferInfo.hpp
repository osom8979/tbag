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
#include <vector>

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
template <typename BaseType, typename SizeType = std::size_t>
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

    BufferInfo(Base * b = nullptr, Size s = 0) TBAG_NOEXCEPT : buffer(b), size(s)
    { /* EMPTY. */ }

    template <typename T>
    BufferInfo(std::vector<T> & b) TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(b.data()) &&
                                                           TBAG_NOEXCEPT_OPERATOR(b.size()))
            : buffer((Base*)b.data()), size(b.size())
    { /* EMPTY. */ }

    ~BufferInfo()
    { /* EMPTY. */ }

    inline void set(Base * b, Size s) TBAG_NOEXCEPT
    {
        buffer = b;
        size   = s;
    }

    template <typename T>
    inline void set(std::vector<T> & b) TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(b.data()) &&
                                                                TBAG_NOEXCEPT_OPERATOR(b.size()))
    {
        buffer = (Base*)b.data();
        size   = b.size();
    }
};

/** Short name of BufferInfo type. */
using  binf = BufferInfo<char      , std::size_t>;
using cbinf = BufferInfo<char const, std::size_t>;

} // namespace util

/** Common buffer container. */
using Buffer = std::vector<char>;

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BUFFERINFO_HPP__

