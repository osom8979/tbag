/**
 * @file   BitFlags.hpp
 * @brief  BitFlags class prototype.
 * @author zer0
 * @date   2017-04-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BITFLAGS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BITFLAGS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace bitwise {

template <typename ValueType>
ValueType setFlag(ValueType source, ValueType value, bool flag = true) TBAG_NOEXCEPT
{
    if (flag) {
        source |= value;
    } else {
        source &= ~value;
    }
    return source;
}

} // namespace bitwise

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BITFLAGS_HPP__

