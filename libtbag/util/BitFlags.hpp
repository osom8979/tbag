/**
 * @file   BitFlags.hpp
 * @brief  BitFlags class prototype.
 * @author zer0
 * @date   2017-04-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BITFLAGS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BITFLAGS_HPP__

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

namespace util {

// Forward declaration.

/**
 * BitFlags class prototype.
 *
 * @author zer0
 * @date   2017-04-05
 */
class TBAG_API BitFlags : public Noncopyable
{
private:
    // Insert member variables.

public:
    BitFlags();
    virtual ~BitFlags();
};

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_BITFLAGS_HPP__

