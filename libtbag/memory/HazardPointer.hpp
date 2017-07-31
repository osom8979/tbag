/**
 * @file   HazardPointer.hpp
 * @brief  HazardPointer class prototype.
 * @author zer0
 * @date   2017-07-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_HAZARDPOINTER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_HAZARDPOINTER_HPP__

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

namespace memory {

// Forward declaration.

/**
 * HazardPointer class prototype.
 *
 * @author zer0
 * @date   2017-07-31
 */
class TBAG_API HazardPointer : private Noncopyable
{
private:
    // Insert member variables.

public:
    HazardPointer();
    virtual ~HazardPointer();
};

} // namespace memory

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MEMORY_HAZARDPOINTER_HPP__

